/*
 *  PrismaticConstraint.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 08/07/11.
 *
 */

#include "dynamics/joints/PrismaticConstraint.h"
#include "dynamics/PhysicalBody.h"
#include <stdio.h>

BEGIN_NAMESPACE_DIRAC2D


PrismaticConstraint::PrismaticConstraint( const PrismaticConstraintInfo& cInfo): Constraint(cInfo)
{
    m_Type = ECT_PRISMATIC;
    m_Anchor = cInfo.m_Anchor;
    m_LocalAxis = cInfo.m_LocalAxis;
	m_LowerLimit = cInfo.m_LowerLimit;
    m_UpperLimit = cInfo.m_UpperLimit;
	m_Frequency = 3.0f;
	m_DampingRatio = 0.2f;
}

void PrismaticConstraint::initialize()
{
	if( m_PhysicalBody1 )
		m_Anchor1 = m_PhysicalBody1->getLocalPoint(m_Anchor);
	if( m_PhysicalBody2 )
		m_Anchor2 = m_PhysicalBody2->getLocalPoint(m_Anchor);
	
	if( m_LocalAxis.lengthSquared() == 0.0f )
		m_LocalAxis.set(0.0f, 1.0f);
	m_LocalAxis.normalize();
	m_LocalPerpendicularAxis.set(-m_LocalAxis.y, m_LocalAxis.x);
	m_ReferenceAngle = m_PhysicalBody2->m_Angle - m_PhysicalBody1->m_Angle;
}

void PrismaticConstraint::buildJacobian(dfloat dt)
{
	PhysicalBody* body1 = m_PhysicalBody1;
	PhysicalBody* body2 = m_PhysicalBody2;		
	
	Vector2f c1, c2;
	
	c1 +=  body1->m_Centre; 
	c2 +=  body2->m_Centre; 
	
	dfloat m1Inv = 0.0f;
	dfloat m2Inv = 0.0f;
	dfloat i1Inv = 0.0f;
	dfloat i2Inv = 0.0f;
	
	{
		m_r1 =  m_Anchor1 - c1;
		body1->m_Transform.transformAsVector(m_r1);
		body1->m_Transform.transformAsPoint(c1);
		m1Inv = body1->m_InvMass;
		i1Inv = body1->m_InvI;
	}
	{
		m_r2 = m_Anchor2 - c2;
		body2->m_Transform.transformAsVector(m_r2);
		body2->m_Transform.transformAsPoint(c2);
		m2Inv = body2->m_InvMass;
		i2Inv = body2->m_InvI;
	}
	
	d = c2 + m_r2 - c1 - m_r1;
	m_WorldPerpendicularAxis = m_LocalPerpendicularAxis;
	body1->m_Transform.transformAsVector(m_WorldPerpendicularAxis);
	Vector2f s = m_r1 + d;
	dfloat aperp_cross_s  = -m_WorldPerpendicularAxis.cross(s);
	dfloat aperp_cross_r2 = -m_WorldPerpendicularAxis.cross(m_r2);
	
	m_WorldAxis = m_LocalAxis;
	body1->m_Transform.transformAsVector(m_WorldAxis);
	dfloat a_cross_s  = -m_WorldAxis.cross(s);
	dfloat a_cross_r2 = -m_WorldAxis.cross(m_r2);
	
	m_EffectiveMassMatrix.col1.x = m1Inv + i1Inv * aperp_cross_s * aperp_cross_s + m2Inv + i2Inv * aperp_cross_r2 * aperp_cross_r2;
	m_EffectiveMassMatrix.col2.x = i1Inv * aperp_cross_s + i2Inv * aperp_cross_r2;
	m_EffectiveMassMatrix.col3.x = i1Inv * aperp_cross_s * a_cross_s + i2Inv * aperp_cross_r2 * a_cross_r2;
	
	m_EffectiveMassMatrix.col1.y = m_EffectiveMassMatrix.col2.x;
	m_EffectiveMassMatrix.col2.y = i1Inv + i2Inv;
	m_EffectiveMassMatrix.col3.y = i1Inv * a_cross_s + i2Inv * a_cross_r2;
	
	m_EffectiveMassMatrix.col1.z = m_EffectiveMassMatrix.col3.x;
	m_EffectiveMassMatrix.col2.z = m_EffectiveMassMatrix.col3.y;
	m_EffectiveMassMatrix.col3.z = m1Inv + i1Inv * a_cross_s * a_cross_s + m2Inv + i2Inv * a_cross_r2 * a_cross_r2;
	
	dfloat effectiveMass = m_EffectiveMassMatrix.col3.z;
	
	dfloat angularFrequency = 2.0f * PI * m_Frequency;
	dfloat k = effectiveMass * angularFrequency * angularFrequency;
	dfloat c = 2.0f * m_DampingRatio * effectiveMass * angularFrequency;
	
	dfloat cfm = (c + k * dt) * dt;
	cfm = cfm != 0.0f ? 1.0f/cfm : cfm;
	m_Cfm = cfm;
	
	m_EffectiveMassMatrix.col3.z += m_Cfm;
	// Error reduction parameter
	dfloat erp = dt * k * m_Cfm;
	
	// Positional Error for Position Stabilization( Baumgarte )
	m_PositionError	= d.dot(m_WorldPerpendicularAxis) * erp;
	m_AngularError	= (m_PhysicalBody2->m_Angle - m_PhysicalBody1->m_Angle - m_ReferenceAngle) * erp;
	
	//printf("Angular Velocity = %f    %f\n", m_PhysicalBody1->m_AngularVelocity, m_PhysicalBody2->m_AngularVelocity);
	
	dfloat constraintDisplacement = d.dot(m_WorldAxis);
	
	if( fabs(m_UpperLimit - m_LowerLimit) < LINEAR_ERROR )
	{
		m_PositionErrorParallel = ( constraintDisplacement - m_LowerLimit ) * erp;
		m_LimitState = ECLS_LOWER_UPPER;
	}
	else if( constraintDisplacement >= m_UpperLimit )
	{
		m_PositionErrorParallel = ( constraintDisplacement - m_UpperLimit ) * erp;
		m_LimitState = ECLS_UPPER;
	}
	else if( constraintDisplacement <= m_LowerLimit )
	{
		m_PositionErrorParallel = ( constraintDisplacement - m_LowerLimit ) * erp;
		m_LimitState = ECLS_LOWER;
	}
	else
	{
		m_Impulse.z = 0.0f;
		m_LimitState = ECLS_NONE;
	}
	
	// Apply Corrective impulse on the bodies
	if( 1 )
	{
		Vector2f impulse =  m_WorldPerpendicularAxis * m_Impulse.x + m_WorldAxis * m_Impulse.z;
		dfloat l1 = Vector2f::cross( m_r1 + d, impulse);
		dfloat l2 = Vector2f::cross( m_r2, impulse );
		
		body1->m_Velocity        -= impulse * m1Inv;
		body1->m_AngularVelocity -= i1Inv * ( l1 + m_Impulse.y );
		
		body2->m_Velocity        += impulse * m2Inv;
		body2->m_AngularVelocity += i2Inv * ( l2 + m_Impulse.y );
	}
	else 
	{
		m_Impulse.setZero();
	}
	
}

void PrismaticConstraint::correctVelocities()
{
	PhysicalBody* body1 = m_PhysicalBody1;
	PhysicalBody* body2 = m_PhysicalBody2;
	
	dfloat m1Inv = 0.0f;
	dfloat m2Inv = 0.0f;
	dfloat i1Inv = 0.0f;
	dfloat i2Inv = 0.0f;
	
	Vector2f v1,v2;
	dfloat w1 = 0.0f, w2 = 0.0f;
	
	m1Inv = body1->m_InvMass;
	i1Inv = body1->m_InvI;
	v1 = body1->m_Velocity;
	w1 = body1->m_AngularVelocity;
	
	m2Inv = body2->m_InvMass;
	i2Inv = body2->m_InvI;
	v2 = body2->m_Velocity;
	w2 = body2->m_AngularVelocity;	
	
	// Relative Velocity = V2 - V1 = v2 + w2xr2 - v1 - w1xr1			
	Vector2f relVel = v2 + Vector2f::cross(w2, m_r2) - v1 - Vector2f::cross(w1, m_r1);
	dfloat Cdot1 = d.dot(Vector2f::cross(body1->m_AngularVelocity, m_WorldPerpendicularAxis))
	+ relVel.dot(m_WorldPerpendicularAxis);
	
	Cdot1 += m_PositionError;
	
	// Relative Velocity = V2 - V1 = v2 + w2xr2 - v1 - w1xr1			
	dfloat Cdot2 = d.dot(Vector2f::cross(body1->m_AngularVelocity, m_WorldAxis))
	+ relVel.dot(m_WorldAxis);
	
	Cdot2 += m_PositionErrorParallel + m_Cfm * m_Impulse.z;
	
	Vector3f correctiveImpulse;
	
	if( m_LimitState != ECLS_NONE )
	{	
		// First Solve for the Complete 3x3 Jacobian Matrix( ie all constraints simultaneously).	
		dfloat dw = m_PhysicalBody2->m_AngularVelocity - m_PhysicalBody1->m_AngularVelocity + m_AngularError;
		Vector3f Cdot = Vector3f( Cdot1, dw ,Cdot2 );
		correctiveImpulse = -( m_EffectiveMassMatrix.solve(Cdot) );
		
		// Both Lower and Upper Limits are Same. we need to only solve the Complete 2x2 Jacobian Matrix.
		if( m_LimitState == ECLS_LOWER_UPPER )
		{
			m_Impulse += correctiveImpulse;
		}
		// Lower Limit.
		else if ( m_LimitState == ECLS_LOWER )
		{
			dfloat newImpulseZ = m_Impulse.z + correctiveImpulse.z;
			// we need to clamp the impulse to 0.0f as this impulse should not effect the Prismatic Constraint. we should solve for 
			// the new corrective impulse again as because of clamping the old solution has become invalid.
			if( newImpulseZ < 0.0f )
			{
				Vector2f rhs = (-Vector2f(Cdot1, dw) + Vector2f(m_EffectiveMassMatrix.col3.x, m_EffectiveMassMatrix.col3.y) * m_Impulse.z);
				Vector2f newCorrectiveImpulse = m_EffectiveMassMatrix.solve22(rhs);
				correctiveImpulse.x = newCorrectiveImpulse.x;
				correctiveImpulse.y = newCorrectiveImpulse.y;
				correctiveImpulse.z = -m_Impulse.z;
				m_Impulse.x += correctiveImpulse.x;
				m_Impulse.y += correctiveImpulse.y;
				// clamp the impulse
				m_Impulse.z = 0.0f;
			}
			// handle the Lower Linear Limit case.
			else
			{
				m_Impulse += correctiveImpulse;
			}
		}
		else if ( m_LimitState == ECLS_UPPER )
		{
			dfloat newImpulseZ = m_Impulse.z + correctiveImpulse.z;
			// we need to clamp the impulse to 0.0f as this impulse should not effect the Prismatic Constraint. we should solve for 
			// the new corrective impulse again as because of clamping the old solution has become invalid.
			if( newImpulseZ > 0.0f )
			{
				Vector2f rhs = (-Vector2f(Cdot1, dw) + Vector2f(m_EffectiveMassMatrix.col3.x, m_EffectiveMassMatrix.col3.y) * m_Impulse.z);
				Vector2f newCorrectiveImpulse = m_EffectiveMassMatrix.solve22(rhs);
				correctiveImpulse.x = newCorrectiveImpulse.x;
				correctiveImpulse.y = newCorrectiveImpulse.y;
				correctiveImpulse.z = -m_Impulse.z;
				m_Impulse.x += correctiveImpulse.x;
				m_Impulse.y += correctiveImpulse.y;
				// clamp the impulse
				m_Impulse.z = 0.0f;
			}
			// handle the Lower Linear Limit case.
			else
			{
				m_Impulse += correctiveImpulse;
			}		
		}
	}
	// There is no Limit on the Linear motion.
	else 
	{
		Vector2f rhs(Cdot1, m_PhysicalBody2->m_AngularVelocity - m_PhysicalBody1->m_AngularVelocity + m_AngularError);
		Vector2f impulse = m_EffectiveMassMatrix.solve22(-rhs);
		correctiveImpulse = impulse;
		correctiveImpulse.z = 0.0f;
		m_Impulse += correctiveImpulse;
		m_Impulse.z = 0.0f;
	}
	
	Vector2f impulse =  m_WorldPerpendicularAxis * correctiveImpulse.x + m_WorldAxis * correctiveImpulse.z;
	
	body1->m_Velocity        -= impulse * m1Inv;
	body1->m_AngularVelocity -= i1Inv * ( Vector2f::cross(m_r1+d, impulse) + correctiveImpulse.y  );
	
	body2->m_Velocity        += impulse * m2Inv;
	body2->m_AngularVelocity += i2Inv * ( Vector2f::cross(m_r2, impulse) + correctiveImpulse.y );		
	
}

END_NAMESPACE_DIRAC2D
