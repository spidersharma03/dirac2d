/*
 *  CatenaryConstraint.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 03/07/12.
 *
 */

#include "CatenaryConstraint.h"
#include "../PhysicalBody.h"
#include "../PhysicalShape.h"
#include <stdio.h>

BEGIN_NAMESPACE_DIRAC2D


CatenaryConstraint::CatenaryConstraint( const CatenaryConstraintInfo& cInfo): Constraint(cInfo)
{
    m_Type = ECT_CATENARY;
    m_FixedLength = cInfo.m_FixedLength;
    m_Anchor = cInfo.m_Anchor;
    m_FixedPoint1 = cInfo.m_FixedPoint1;
    m_FixedPoint2 = cInfo.m_FixedPoint2;
	m_Freqeuncy = 2.0f;
	m_DampingRatio = 0.2f;
}

void CatenaryConstraint::initialize()
{
	m_Distance = m_FixedPoint1.distance(m_FixedPoint2);
	dAssert( m_PhysicalBody1 );
	dAssert( m_Distance > 0.0f );
	dAssert( m_FixedLength > m_Distance );
}

void CatenaryConstraint::buildJacobian(dfloat dt)
{
	PhysicalBody* pBody = m_PhysicalBody1;
		
	Vector2f R =  pBody->m_Centre; // World Anchor Point of Body 1.	
	m_r = m_Anchor;
	
	pBody->m_Transform.transformAsPoint(R);
	
	pBody->m_Transform.transformAsVector(m_r);
	
	R += m_r;
	
	Vector2f l1 = R - m_FixedPoint1;
	Vector2f l2 = R - m_FixedPoint2;
	
	dfloat len1 =  l1.length();
	dfloat len2 =  l2.length();
	
	//printf("Length = %f  %f\n", len1, len2);

	if( len1 > 0.0f )
		l1 /= len1;
	if( len2 > 0.0f )
		l2 /= len2;
	
	m_ImpulseDirection = l1 + l2;
	
	dfloat rcrossX = m_r.cross(m_ImpulseDirection);
	dfloat JInvMJT = pBody->m_InvMass + pBody->m_InvI * rcrossX * rcrossX; 
	
	m_EffectiveMass = JInvMJT != 0.0f ? 1.0f/JInvMJT : JInvMJT;
	
	dfloat angularFrequency = 2.0f * PI * m_Freqeuncy;
	dfloat k = m_EffectiveMass * angularFrequency * angularFrequency;
	dfloat c = 2.0f * m_DampingRatio * m_EffectiveMass * angularFrequency;
	
	dfloat cfm = (c + k * dt) * dt;
	cfm = cfm != 0.0f ? 1.0f/cfm : cfm;
	m_Cfm = cfm;
	
	// Error reduction parameter
	dfloat erp = dt * k * m_Cfm;
	
	m_EffectiveMass = JInvMJT + m_Cfm;
	// Effective mass for the Constraint.
	m_EffectiveMass = m_EffectiveMass != 0.0f ? 1.0f/m_EffectiveMass : m_EffectiveMass;
	
	// Positional Error for Position Stabilization( Baumgarte )
	m_TotalLength = len1 + len2;
	m_PositionError = erp * ( m_TotalLength - m_FixedLength );
	
	// Apply Corrective impulse on the bodies
	if( 1 )
	{
		Vector2f totalImpulse = m_ImpulseDirection * m_ImpulseMagnitude;
		pBody->m_Velocity        += totalImpulse * pBody->m_InvMass;
		pBody->m_AngularVelocity += pBody->m_InvI * Vector2f::cross( m_r, totalImpulse);
	}
	else 
	{
		m_ImpulseMagnitude = 0.0f;
	}
	
}

void CatenaryConstraint::correctVelocities()
{
	PhysicalBody* pBody = m_PhysicalBody1;
	
	Vector2f vel = ( pBody->m_Velocity + Vector2f::cross(pBody->m_AngularVelocity, m_r) );
	dfloat Cdot = vel.dot(m_ImpulseDirection);	// Cdot = J * v		
	//dfloat correctiveImpulseMag = -m_EffectiveMass * ( m_PositionError + Cdot + m_Cfm * m_ImpulseMagnitude );
	//m_ImpulseMagnitude += correctiveImpulseMag;
	
	dfloat correctiveImpulseMag = -m_EffectiveMass * ( Cdot + m_PositionError + m_Cfm * m_ImpulseMagnitude );
	dfloat oldImpulse = m_ImpulseMagnitude;
	m_ImpulseMagnitude = MIN(correctiveImpulseMag + m_ImpulseMagnitude, 0.0f);
	correctiveImpulseMag = m_ImpulseMagnitude - oldImpulse;

	Vector2f correctiveImpulse;
	correctiveImpulse = m_ImpulseDirection * correctiveImpulseMag;
	// Apply Corrective impulse on the bodies due to Normal Impulse
	//if( ( m_TotalLength - m_Length ) > 1e-02f )
	{
		pBody->m_Velocity        += correctiveImpulse * pBody->m_InvMass;
		pBody->m_AngularVelocity += pBody->m_InvI * Vector2f::cross( m_r, correctiveImpulse);
	}
}

END_NAMESPACE_DIRAC2D