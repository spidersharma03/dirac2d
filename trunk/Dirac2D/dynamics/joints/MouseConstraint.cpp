/*
 *  MouseConstraint.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 10/01/13.
 *
 */

#include "dynamics/joints/MouseConstraint.h"
#include "dynamics/joints/DistantConstraint.h"
#include "dynamics/PhysicalBody.h"
#include <stdio.h>

BEGIN_NAMESPACE_DIRAC2D

#define DAMPING 0.99f

void MouseConstraint::buildJacobian(dfloat dt)
{
	PhysicalBody* body1 = m_PhysicalBody1;
	
	if( !body1 )
		return;
	
	dfloat mInv = 0.0f;
	dfloat iInv = 0.0f;
	
	
	m_r = m_BodyLocalPickPosition;// - body1->m_Centre;
	body1->m_Transform.transformAsVector(m_r);

	mInv = body1->m_InvMass;
	iInv = body1->m_InvI;
				
	dfloat angularFrequency = 2.0f * PI * m_Frequency;
	dfloat k = body1->m_Mass * angularFrequency * angularFrequency;
	dfloat c = 2.0f * m_DampingRatio * body1->m_Mass * angularFrequency;
	
	dfloat cfm = (c + k * dt) * dt;
	cfm = cfm != 0.0f ? 1.0f/cfm : cfm;
	
	// Error reduction parameter
	dfloat erp = dt * k * cfm;
	
	// Effective mass Matrix for the Mouse Constraint.
	m_EffectiveMassMatrix.a11 = mInv + iInv * m_r.y * m_r.y + cfm;
	m_EffectiveMassMatrix.a12 = - m_r.x * m_r.y;
	m_EffectiveMassMatrix.a22 = mInv + iInv * m_r.x * m_r.x + cfm;
	m_EffectiveMassMatrix.a21 = m_EffectiveMassMatrix.a12;
	
	m_EffectiveMassMatrix.invert();
	//printf("D = %f\n", m_EffectiveMassMatrix.a11);	
	
	// Positional Error for Position Stabilization( Baumgarte )
	//body1->m_Transform.transformAsPoint(bodyPickPosition);
	Vector2f bodyPickPosition = body1->getWorldPoint(m_BodyLocalPickPosition);
	m_PositionError = erp * ( bodyPickPosition - m_MousePoint);
	
	// Apply Corrective impulse on the bodies
	if( 1 )
	{
		body1->m_Velocity        += m_Impulse * mInv;
		body1->m_AngularVelocity += iInv * Vector2f::cross( m_r, m_Impulse);
		body1->m_AngularVelocity *= DAMPING;
	}
	else 
	{
		m_Impulse.set(0.0f, 0.0f);
	}
}

void MouseConstraint::correctVelocities()
{
	PhysicalBody* body1 = m_PhysicalBody1;
	
	if( !body1 )
		return;
		
	Vector2f Cdot = body1->m_Velocity + Vector2f::cross(body1->m_AngularVelocity, m_r);
	Cdot += m_PositionError;
	
	Vector2f oldImpulse = m_Impulse;
	Vector2f correctiveImpulse = -(m_EffectiveMassMatrix * Cdot);
	m_Impulse += correctiveImpulse;
	
	// Clamp the Impulse 
	if( m_Impulse.lengthSquared() > m_MaxImpulse * m_MaxImpulse )
	{
		m_Impulse *= m_MaxImpulse / m_Impulse.length();
	}
	
	// Find the new corrective impulse
	correctiveImpulse = m_Impulse - oldImpulse;

	// Apply Corrective impulse on the bodies
	body1->m_Velocity        += correctiveImpulse * body1->m_InvMass;
	body1->m_AngularVelocity += body1->m_InvI * ( Vector2f::cross( m_r, correctiveImpulse) );
}

void MouseConstraint::initialize()
{
}

void MouseConstraint::setMousePosition(const Vector2f& position)
{
	m_MousePoint = position;
}

void MouseConstraint::setTargetBody( PhysicalBody* pBody, const Vector2f position  )
{
	m_PhysicalBody1 = pBody;
	if( m_PhysicalBody1 )
		m_BodyLocalPickPosition =  m_PhysicalBody1->getLocalPoint(position);;
}

Vector2f MouseConstraint::getTargetPickPosition()
{
	return m_PhysicalBody1->getWorldPoint(m_BodyLocalPickPosition);
}

MouseConstraint::MouseConstraint( const MouseConstraintInfo& constraintInfo ) : Constraint( constraintInfo )
{
	m_Frequency = 5.0f;
	m_DampingRatio = 1.0f;
	m_Type = ECT_MOUSE;
	m_MaxImpulse = 2.0f;
}

END_NAMESPACE_DIRAC2D