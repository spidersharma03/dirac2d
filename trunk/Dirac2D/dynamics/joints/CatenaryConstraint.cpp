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

CatenaryConstraint::CatenaryConstraint():Constraint()
{
	m_Type = ECT_CATENARY;
	m_Length = 0.0f;
	m_Cfm = 50.0f;
	m_Erp = 100.0f;
}

void CatenaryConstraint::initialize()
{
	m_Distance = m_FixedPoint1.distance(m_FixedPoint2);
	dAssert( m_Distance > 0.0f );
	dAssert( m_Length > m_Distance );
}

void CatenaryConstraint::buildJacobian()
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
	
	dfloat cerror =  len1 + len2 - m_Length;

	//printf("Length=%f\n", len1 + len2);

	if( len1 > 0.0f )
		l1 /= len1;
	if( len2 > 0.0f )
		l2 /= len2;
	
	m_ImpulseDirection = l1 + l2;//R * 2.0f - ( m_FixedPoint1 + m_FixedPoint2 );
	
	if( m_ImpulseDirection.lengthSquared() < EPSILON )
		dAssert(0);
	else
		m_ImpulseDirection.normalize();
	
	dfloat rcrossX = m_r.cross(m_ImpulseDirection);
	dfloat JInvMJT = pBody->m_InvMass + pBody->m_InvI * rcrossX * rcrossX; 
		
	
	// Effective mass for the Constraint.
	m_EffectiveMass = 1.0f/(JInvMJT + m_Cfm);
	// Positional Error for Position Stabilization( Baumgarte )
	m_PositionError = m_Erp * cerror;
	
	// Apply Corrective impulse on the bodies
	if( 0 )
	{
		Vector2f totalImpulse = m_ImpulseDirection * m_ImpulseMagnitude;
		pBody->m_Velocity        -= totalImpulse * pBody->m_InvMass;
		pBody->m_AngularVelocity -= pBody->m_InvI * Vector2f::cross( m_r, totalImpulse);
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
	
	dfloat correctiveImpulseMag = -m_EffectiveMass * ( m_PositionError + Cdot + m_Cfm * m_ImpulseMagnitude );
	m_ImpulseMagnitude += correctiveImpulseMag;
	
	Vector2f correctiveImpulse;
	correctiveImpulse = m_ImpulseDirection * correctiveImpulseMag;
	
	// Apply Corrective impulse on the bodies due to Normal Impulse
	if( m_PositionError > 1e-03 )
	{
		pBody->m_Velocity        += correctiveImpulse * pBody->m_InvMass;
		pBody->m_AngularVelocity += pBody->m_InvI * Vector2f::cross( m_r, correctiveImpulse);
	}
}

END_NAMESPACE_DIRAC2D