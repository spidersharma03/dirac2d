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
}

void HingeConstraint::initialize()
{
	if( m_PhysicalBody1 )
		m_Anchor1 = m_PhysicalBody1->getLocalPoint(m_Anchor);
	if( m_PhysicalBody2 )
		m_Anchor2 = m_PhysicalBody2->getLocalPoint(m_Anchor);
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
	
	m_EffectiveMass.a11 = m1Inv + m2Inv + i1Inv * m_r1.y * m_r1.y + i2Inv * m_r2.y * m_r2.y;
	m_EffectiveMass.a21 = - i1Inv * m_r1.y * m_r1.x - i2Inv * m_r2.y * m_r2.x;
	m_EffectiveMass.a12 =	m_EffectiveMass.a21;
	m_EffectiveMass.a22 = m1Inv + m2Inv + i1Inv * m_r1.x * m_r1.x + i2Inv * m_r2.x * m_r2.x;
	
	m_EffectiveMass.invert();
	
	// Positional Error for Position Stabilization( Baumgarte )
	m_PositionError = ( c2 + m_r2 - c1 - m_r1 ) * m_Erp;
	//printf(" Error = %f  %f\n", m_PositionError.x, m_PositionError.y);
	// Apply Corrective impulse on the bodies
	if( 1 )//body1->m_PhysicalWorld->m_bWarmStart )
	{
		Vector2f totalImpulse = m_Impulse;
		if( body1 )
		{
			body1->m_Velocity        -= totalImpulse * m1Inv;
			body1->m_AngularVelocity -= i1Inv * Vector2f::cross( m_r1, totalImpulse);
		}
		if( body2 )
		{
			body2->m_Velocity        += totalImpulse * m2Inv;
			body2->m_AngularVelocity += i2Inv * Vector2f::cross( m_r2, totalImpulse);
		}
	}
	else 
	{
		m_Impulse.x = m_Impulse.y = 0.0f;
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
	Vector2f correctiveImpulse = -( m_EffectiveMass * Cdot );
	m_Impulse += correctiveImpulse;
		
	// Apply Corrective impulse on the bodies due to Normal Impulse
	if( body1 )
	{
		body1->m_Velocity        -= correctiveImpulse * m1Inv;
		body1->m_AngularVelocity -= i1Inv * Vector2f::cross( m_r1, correctiveImpulse);
	}
	if( body2 )
	{
		body2->m_Velocity        += correctiveImpulse * m2Inv;
		body2->m_AngularVelocity += i2Inv * Vector2f::cross( m_r2, correctiveImpulse);		
	}
}

END_NAMESPACE_DIRAC2D