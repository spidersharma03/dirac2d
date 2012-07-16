/*
 *  HingeConstraint.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 12/06/12.
 *
 */

#include "HingeConstraint.h"
#include "../PhysicalBody.h"
#include "../PhysicalShape.h"
#include <stdio.h>

BEGIN_NAMESPACE_DIRAC2D

HingeConstraint::HingeConstraint()
{
	m_Type = ECT_HINGE;
	m_Cfm = 0.0f;
	m_Erp = 0.0f;
	m_LowerAngle = PI/10.0f;
	m_UpperAngle = PI_2;
}

void HingeConstraint::initialize()
{
	if( m_PhysicalBody1 )
		m_Anchor1 = m_PhysicalBody1->getLocalPoint(m_Anchor);
	if( m_PhysicalBody2 )
		m_Anchor2 = m_PhysicalBody2->getLocalPoint(m_Anchor);
	m_ReferenceAngle = m_PhysicalBody2->m_Angle - m_PhysicalBody1->m_Angle;
}

void HingeConstraint::buildJacobian()
{
	PhysicalBody* body1 = m_PhysicalBody1;
	PhysicalBody* body2 = m_PhysicalBody2;		
		
	Vector2f c1, c2;
	
	if( body1 )
	    c1 +=  body1->m_Centre; 
	if( body2 )
		c2 +=  body2->m_Centre; 
	
	dfloat m1Inv = 0.0f;
	dfloat m2Inv = 0.0f;
	dfloat i1Inv = 0.0f;
	dfloat i2Inv = 0.0f;
	
	if( body1 )
	{
		m_r1 =  m_Anchor1 - c1;
		body1->m_Transform.transformAsVector(m_r1);
		body1->m_Transform.transformAsPoint(c1);
		m1Inv = body1->m_InvMass;
		i1Inv = body1->m_InvI;
	}
	if( body2 )
	{
		m_r2 = m_Anchor2 - c2;
		body2->m_Transform.transformAsVector(m_r2);
		body2->m_Transform.transformAsPoint(c2);
		m2Inv = body2->m_InvMass;
		i2Inv = body2->m_InvI;
	}
	
	m_EffectiveMass.col1.x = m1Inv + m2Inv + i1Inv * m_r1.y * m_r1.y + i2Inv * m_r2.y * m_r2.y;
	m_EffectiveMass.col1.y = - i1Inv * m_r1.y * m_r1.x - i2Inv * m_r2.y * m_r2.x;
	m_EffectiveMass.col1.z = - i1Inv * m_r1.y - i2Inv * m_r2.y;
	
	m_EffectiveMass.col2.x = m_EffectiveMass.col1.y;
	m_EffectiveMass.col2.y = m1Inv + m2Inv + i1Inv * m_r1.x * m_r1.x + i2Inv * m_r2.x * m_r2.x;
	m_EffectiveMass.col2.z = i1Inv * m_r1.x + i2Inv * m_r2.x;
	
	m_EffectiveMass.col3.x = m_EffectiveMass.col1.z;
	m_EffectiveMass.col3.y = m_EffectiveMass.col2.z;
	m_EffectiveMass.col3.z = i1Inv + i2Inv + m_Cfm;
	
	//m_EffectiveMass.invert();
	
	// Positional Error for Position Stabilization( Baumgarte )
	m_PositionError = ( c2 + m_r2 - c1 - m_r1 ) * m_Erp;

	dfloat angle = (body2->m_Angle - body1->m_Angle) - m_ReferenceAngle;

	//printf("Angle = %f\n", angle*180.0f/PI);
	if( fabs(m_UpperAngle - m_LowerAngle) < ANGULAR_ERROR )
	{
		m_AngularError = ( angle - m_UpperAngle ) * m_Erp;
		m_AngleLimitState = ECLS_LOWER_UPPER;
	}
	else if( angle >= m_UpperAngle )
	{
		m_AngularError = ( angle - m_UpperAngle ) * m_Erp;
		m_AngleLimitState = ECLS_UPPER;
	}
	else if( angle <= m_LowerAngle )
	{
		m_AngularError = ( angle - m_LowerAngle ) * m_Erp;
		m_AngleLimitState = ECLS_LOWER;
	}
	else {
		m_Impulse.z = 0.0f;
		m_AngleLimitState = ECLS_NONE;
	}

	//printf(" Error = %f  %f\n", m_PositionError.x, m_PositionError.y);
	// Apply Corrective impulse on the bodies
	if( 1 )//body1->m_PhysicalWorld->m_bWarmStart )
	{
		Vector2f impulse( m_Impulse.x, m_Impulse.y);
		if( body1 )
		{
			body1->m_Velocity        -= impulse * m1Inv;
			body1->m_AngularVelocity -= i1Inv * ( Vector2f::cross( m_r1, impulse) + m_Impulse.z );
		}
		if( body2 )
		{
			body2->m_Velocity        += impulse * m2Inv;
			body2->m_AngularVelocity += i2Inv * ( Vector2f::cross( m_r2, impulse) + m_Impulse.z );
		}
	}
	else 
	{
		m_Impulse.setZero();
	}
	
}

void HingeConstraint::correctVelocities()
{
	PhysicalBody* body1 = m_PhysicalBody1;
	PhysicalBody* body2 = m_PhysicalBody2;
	
	dfloat m1Inv = 0.0f;
	dfloat m2Inv = 0.0f;
	dfloat i1Inv = 0.0f;
	dfloat i2Inv = 0.0f;
	
	Vector2f v1,v2;
	dfloat w1 = 0.0f, w2 = 0.0f;
	if( body1 )
	{
		m1Inv = body1->m_InvMass;
		i1Inv = body1->m_InvI;
		v1 = body1->m_Velocity;
		w1 = body1->m_AngularVelocity;
	}
	if( body2 )
	{
		m2Inv = body2->m_InvMass;
		i2Inv = body2->m_InvI;
		v2 = body2->m_Velocity;
		w2 = body2->m_AngularVelocity;
	}
	
	// Relative Velocity = V2 -`int V1 = v2 + w2xr2 - v1 - w1xr1			
	Vector2f Cdot = v2 + Vector2f::cross(w2, m_r2) - v1 - Vector2f::cross(w1, m_r1);
	
	Cdot += m_PositionError;
	
	Vector3f correctiveImpulse;
	
	// This is Angular Limit case 
	if( m_AngleLimitState != ECLS_NONE )
	{		
		// First Solve for the Complete 3x3 Jacobian Matrix( ie all constraints simultaneously).		
		Vector3f Cdot1 = Vector3f( Cdot,  body2->m_AngularVelocity - body1->m_AngularVelocity + m_AngularError + m_Cfm * m_Impulse.z );
		correctiveImpulse = -( m_EffectiveMass.solve(Cdot1) );
				
		// Both Lower and Upper Limits are Same. we need to only solve the Complete 3x3 Jacobian Matrix.
		if( m_AngleLimitState == ECLS_LOWER_UPPER )
		{
			m_Impulse += correctiveImpulse;
		}
		// Lower Limit.
		else if ( m_AngleLimitState == ECLS_LOWER )
		{
			dfloat newImpulseZ = m_Impulse.z + correctiveImpulse.z;
			// we need to clamp the impulse to 0.0f as this impulse should not effect the Hinge Constraint. we should solve for 
			// the new corrective impulse again as because of clamping the old solution has become invalid.
			if( newImpulseZ < 0.0f )
			{
				Vector2f rhs = -Cdot + Vector2f(m_EffectiveMass.col3.x, m_EffectiveMass.col3.y) * m_Impulse.z;
				Vector2f newCorrectiveImpulse = m_EffectiveMass.solve22(rhs);
				correctiveImpulse = newCorrectiveImpulse;
				correctiveImpulse.z = -m_Impulse.z;
				m_Impulse.x += correctiveImpulse.y;
				m_Impulse.y += correctiveImpulse.y;
				// clamp the impulse
				m_Impulse.z = 0.0f;
			}
			// handle the Lower Angular Limit case.
			else
			{
				m_Impulse += correctiveImpulse;
			}
		}
		else if ( m_AngleLimitState == ECLS_UPPER )
		{
			dfloat newImpulseZ = m_Impulse.z + correctiveImpulse.z;
			// we need to clamp the impulse to 0.0f as this impulse should not effect the Hinge Constraint. we should solve for 
			// the new corrective impulse again as because of clamping the old solution has become invalid.
			if( newImpulseZ > 0.0f )
			{
				Vector2f rhs = -Cdot + Vector2f(m_EffectiveMass.col3.x, m_EffectiveMass.col3.y) * m_Impulse.z;
				Vector2f newCorrectiveImpulse = m_EffectiveMass.solve22(rhs);
				correctiveImpulse = newCorrectiveImpulse;
				correctiveImpulse.z = -m_Impulse.z;
				m_Impulse.x += correctiveImpulse.y;
				m_Impulse.y += correctiveImpulse.y;
				// clamp the impulse
				m_Impulse.z = 0.0f;
			}
			// handle the Lower Angular Limit case.
			else
			{
				m_Impulse += correctiveImpulse;
			}
		}
	}
	// There is no Limit on the Angle. solve for the normal 2x2 Jacobian Matrix.
	else 
	{
		Vector2f impulse = -m_EffectiveMass.solve22(Cdot);
		correctiveImpulse = impulse;
		correctiveImpulse.z = 0.0f;
		m_Impulse += correctiveImpulse;
	}
	
	Vector2f impulse( correctiveImpulse.x, correctiveImpulse.y );
	// Apply Corrective impulse on the bodies due to Normal Impulse
	if( body1 )
	{
		body1->m_Velocity        -= impulse * m1Inv;
		body1->m_AngularVelocity -= i1Inv * ( Vector2f::cross( m_r1, impulse) + correctiveImpulse.z );
	}
	if( body2 )
	{
		body2->m_Velocity        += impulse * m2Inv;
		body2->m_AngularVelocity += i2Inv * ( Vector2f::cross( m_r2, impulse) + correctiveImpulse.z );		
	}
}

END_NAMESPACE_DIRAC2D