/*
 *  DistanceConstraint.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 08/06/12.
 *
 */
#include "DistantConstraint.h"
#include "../PhysicalBody.h"
#include "../PhysicalShape.h"

BEGIN_NAMESPACE_DIRAC2D

DistanceConstraint::DistanceConstraint():Constraint()
{
	m_Type = ECT_DISTANCE;
}

void DistanceConstraint::initialize()
{
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
	m_Distance = a1.distance(a2);
}

void DistanceConstraint::buildJacobian()
{
	PhysicalBody* body1 = m_PhysicalBody1;
	PhysicalBody* body2 = m_PhysicalBody2;		
	
	m_r1 = m_Anchor1; // World radius vector on body 1.
	m_r2 = m_Anchor2; // World radius vector on body 2.
	
	Vector2f R1 = m_Anchor1, R2 = m_Anchor2;
	
	if( body1 )
	    R1 +=  body1->m_Centre; // World Anchor Point of Body 1.
	if( body2 )
		R2 +=  body2->m_Centre; // World Anchor Point of Body 2.
	
	dfloat m1Inv = 0.0f;
	dfloat m2Inv = 0.0f;
	dfloat i1Inv = 0.0f;
	dfloat i2Inv = 0.0f;
	
	if( body1 )
	{
		body1->m_Transform.transformAsPoint(R1);
		body1->m_Transform.transformAsVector(m_r1);
		m1Inv = body1->m_InvMass;
		i1Inv = body1->m_InvI;
	}
	if( body2 )
	{
		body2->m_Transform.transformAsPoint(R2);
		body2->m_Transform.transformAsVector(m_r2);
		m2Inv = body2->m_InvMass;
		i2Inv = body2->m_InvI;
	}
	
	m_ImpulseDirection = R2 - R1;
	
	dfloat distance = m_ImpulseDirection.length();
	
	if( distance > 0.0f )
		m_ImpulseDirection /= distance;
	
	dfloat r1cross_d = m_r1.cross(m_ImpulseDirection);
	dfloat r2cross_d = m_r2.cross(m_ImpulseDirection);
	
	dfloat JInvMJT = ( m1Inv + m2Inv ) + r1cross_d * r1cross_d * i1Inv + r2cross_d * r2cross_d * i2Inv; 
	
	dfloat cerror = (distance - m_Distance);
	
	// Effective mass for the Constraint.
	m_EffectiveMass = 1.0f/(JInvMJT + m_Cfm);
	// Positional Error for Position Stabilization( Baumgarte )
	m_PositionError = m_Erp * cerror;
	
	// Apply Corrective impulse on the bodies
	if( 1 )//body1->m_PhysicalWorld->m_bWarmStart )
	{
		Vector2f totalImpulse = m_ImpulseDirection * m_ImpulseMagnitude;
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
		m_ImpulseMagnitude = 0.0f;
	}
	
}
	
void DistanceConstraint::correctVelocities()
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
	
	// Relative Velocity of the Contact point on the Bodies = V2 - V1 = v2 + w2xr2 - v1 - w1xr1			
	Vector2f relvel = ( v2 + Vector2f::cross(w2, m_r2) - v1 - Vector2f::cross(w1, m_r1) );
	dfloat Cdot = relvel.dot(m_ImpulseDirection);	// Cdot = J * v		
	
	dfloat correctiveImpulseMag = -m_EffectiveMass * ( m_PositionError + Cdot + m_Cfm * m_ImpulseMagnitude );
	m_ImpulseMagnitude += correctiveImpulseMag;
	
	Vector2f correctiveImpulse = m_ImpulseDirection * correctiveImpulseMag;
	
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
