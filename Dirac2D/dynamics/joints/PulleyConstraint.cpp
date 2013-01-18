/*
 *  PulleyConstraint.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 14/07/11.
 *
 */

#include "PulleyConstraint.h"
#include "../PhysicalBody.h"


BEGIN_NAMESPACE_DIRAC2D


PulleyConstraint::PulleyConstraint( const PulleyConstraintInfo& cInfo): Constraint(cInfo)
{
    m_Type = ECT_PULLEY;
    m_Anchor1 = cInfo.m_Anchor1;
    m_Anchor2 = cInfo.m_Anchor2;
    m_FixedPoint1 = cInfo.m_FixedPoint1;
    m_FixedPoint2 = cInfo.m_FixedPoint2;
    m_PulleyRatio = cInfo.m_PulleyRatio;
    
	m_Frequency = 60.0f;
	m_Cfm = 0.0f;
}

void PulleyConstraint::initialize()
{
	dAssert(m_PulleyRatio >= 1.0f );
	Vector2f a1 = m_Anchor1;
	Vector2f a2 = m_Anchor2;
	// Add the local offset of the bodies to the anchor points as user defines the anchor point wrt the local centre of the body.
	if( m_PhysicalBody1 )
	{
		a1 += m_PhysicalBody1->m_Centre;
		m_PhysicalBody1->getTransform().transformAsPoint(a1);
	}
	if( m_PhysicalBody2 )
	{
		a2 += m_PhysicalBody2->m_Centre;
		m_PhysicalBody2->getTransform().transformAsPoint(a2);
	}
	
	m_FixedLength = a1.distance(m_FixedPoint1) + a2.distance(m_FixedPoint2);
}

void PulleyConstraint::buildJacobian(dfloat dt)
{
	PhysicalBody* pBody1 = m_PhysicalBody1;
	PhysicalBody* pBody2 = m_PhysicalBody2;
	
	Vector2f R1 =  pBody1->m_Centre; // World Anchor Point of Body 1.	
	m_r1 = m_Anchor1 - pBody1->m_Centre;
	
	pBody1->m_Transform.transformAsPoint(R1);
	pBody1->m_Transform.transformAsVector(m_r1);
	
	R1 += m_r1;

	Vector2f R2 =  pBody2->m_Centre; // World Anchor Point of Body 1.	
	m_r2 = m_Anchor2 - pBody2->m_Centre;
	
	pBody2->m_Transform.transformAsPoint(R2);
	pBody2->m_Transform.transformAsVector(m_r2);
	
	R2 += m_r2;
	
	m_ImpulseDirection1 = R1 - m_FixedPoint1;
	m_ImpulseDirection2 = R2 - m_FixedPoint2;
	
	dfloat len1 =  m_ImpulseDirection1.length();
	dfloat len2 =  m_ImpulseDirection2.length();
	
	//printf("Length = %f  %f\n", len1, len2);
	
	if( len1 > 0.0f )
		m_ImpulseDirection1 /= len1;
	if( len2 > 0.0f )
		m_ImpulseDirection2 /= len2;
		
	dfloat r_cross_l1 = m_r1.cross(m_ImpulseDirection1);
	dfloat r_cross_l2 = m_r2.cross(m_ImpulseDirection2);
	
	dfloat JInvMJT = pBody1->m_InvMass + m_PulleyRatio * m_PulleyRatio * pBody2->m_InvMass 
	                 + pBody1->m_InvI * r_cross_l1 * r_cross_l1 + m_PulleyRatio * m_PulleyRatio * pBody2->m_InvI * r_cross_l2 * r_cross_l2; 
	
	
	// Effective mass for the Constraint.
	m_EffectiveMass = JInvMJT != 0.0f ? 1.0f/JInvMJT : JInvMJT;
	
	dfloat angularFrequency = 2.0f * PI * m_Frequency;
	dfloat k = m_EffectiveMass * angularFrequency * angularFrequency;
	dfloat c = 2.0f * m_DampingRatio * m_EffectiveMass * angularFrequency;
	
	dfloat cfm = (c + k * dt) * dt;
	cfm = cfm != 0.0f ? 1.0f/cfm : cfm;
	m_Cfm = cfm;
	
	// Error reduction parameter
	dfloat erp = dt * k * m_Cfm;
	
	// Positional Error for Position Stabilization( Baumgarte )
	m_TotalLength = len1 + len2 * m_PulleyRatio;
	m_PositionError = erp * ( m_TotalLength - m_FixedLength );
	
	// Apply Corrective impulse on the bodies
	if( 1 )
	{
		if(pBody1)
		{
			pBody1->m_Velocity        += m_ImpulseDirection1 * pBody1->m_InvMass;
			pBody1->m_AngularVelocity += pBody1->m_InvI * Vector2f::cross( m_r1, m_ImpulseDirection1);
		}
		if(pBody2)
		{
			pBody2->m_Velocity        += m_ImpulseDirection2 * m_PulleyRatio * pBody2->m_InvMass;
			pBody2->m_AngularVelocity += pBody2->m_InvI * m_PulleyRatio * Vector2f::cross( m_r2, m_ImpulseDirection2);
		}
	}
	else 
	{
		m_ImpulseMagnitude = 0.0f;
	}
	
}

void PulleyConstraint::correctVelocities()
{
	PhysicalBody* pBody1 = m_PhysicalBody1;
	PhysicalBody* pBody2 = m_PhysicalBody2;
	
	Vector2f vel1 = ( pBody1->m_Velocity + Vector2f::cross(pBody1->m_AngularVelocity, m_r1) );
	Vector2f vel2 = ( pBody2->m_Velocity + Vector2f::cross(pBody2->m_AngularVelocity, m_r2) );

	dfloat Cdot = vel1.dot(m_ImpulseDirection1) + m_PulleyRatio * vel2.dot(m_ImpulseDirection2);	// Cdot = J * v		
	//dfloat correctiveImpulseMag = -m_EffectiveMass * ( m_PositionError + Cdot + m_Cfm * m_ImpulseMagnitude );
	//m_ImpulseMagnitude += correctiveImpulseMag;
	
	dfloat correctiveImpulseMag = -m_EffectiveMass * ( Cdot + m_PositionError );
	m_ImpulseMagnitude += correctiveImpulseMag;
	
	Vector2f correctiveImpulse1, correctiveImpulse2;
	correctiveImpulse1 = m_ImpulseDirection1 * correctiveImpulseMag;
	correctiveImpulse2 = m_ImpulseDirection2 * correctiveImpulseMag * m_PulleyRatio;

	// Apply Corrective impulse on the bodies due to Normal Impulse
	if(pBody1)
	{
		pBody1->m_Velocity        += correctiveImpulse1 * pBody1->m_InvMass;
		pBody1->m_AngularVelocity += pBody1->m_InvI * Vector2f::cross( m_r1, correctiveImpulse1);
	}
	if(pBody2)
	{
		pBody2->m_Velocity        += correctiveImpulse2 * pBody2->m_InvMass;
		pBody2->m_AngularVelocity += pBody2->m_InvI * Vector2f::cross( m_r2, correctiveImpulse2);
	}
	   
}

END_NAMESPACE_DIRAC2D