/*
 *  CatenaryConstraintFixedRotation.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 06/07/12.
 *
 */

#include "CatenaryConstraintFixedRotation.h"
#include "../PhysicalBody.h"
#include "../PhysicalShape.h"
#include <stdio.h>

BEGIN_NAMESPACE_DIRAC2D

CatenaryConstraintFixedRotation::CatenaryConstraintFixedRotation():Constraint()
{
	m_Type = ECT_CATENARY;
	m_FixedLength = 0.0f;
	m_Cfm = 100.0f;
	m_Erp = 100.0f;
	m_ReferenceAngle = 0.0f;
}

void CatenaryConstraintFixedRotation::initialize()
{
	m_Distance = m_FixedPoint1.distance(m_FixedPoint2);
	dAssert( m_PhysicalBody1 );
	dAssert( m_Distance > 0.0f );
	dAssert( m_FixedLength > m_Distance );
	m_ReferenceAngle = m_PhysicalBody1->m_Angle;
}

void CatenaryConstraintFixedRotation::buildJacobian()
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
	
	//if( m_ImpulseDirection.lengthSquared() < EPSILON )
	//		dAssert(0);
	//	else
	//m_ImpulseDirection.normalize();
	
	dfloat rcrossX = m_r.cross(m_ImpulseDirection);
	
	// Effective Mass Matrix for the Constraint.	
	m_EffectiveMassMatrix.a11 = pBody->m_InvMass * m_ImpulseDirection.dot(m_ImpulseDirection) + pBody->m_InvI * rcrossX * rcrossX + m_Cfm;
	m_EffectiveMassMatrix.a21 = pBody->m_InvI * rcrossX;
	m_EffectiveMassMatrix.a12 = m_EffectiveMassMatrix.a21;
	m_EffectiveMassMatrix.a22 = pBody->m_InvI + m_Cfm;
	
	//printf("m_EffectiveMass = %f\n", rcrossX );

	m_EffectiveMassMatrix.invert();
		
	// Positional Error for Position Stabilization( Baumgarte )
	m_TotalLength = len1 + len2;
	m_PositionError = m_Erp * ( m_TotalLength - m_FixedLength );
	m_AngularError  = m_Erp * ( pBody->m_Angle - m_ReferenceAngle );
	
	// Apply Corrective impulse on the bodies
	if( 0 )
	{
		Vector2f totalImpulse = m_Impulse;
		pBody->m_Velocity        += totalImpulse * pBody->m_InvMass;
		pBody->m_AngularVelocity += pBody->m_InvI * Vector2f::cross( m_r, totalImpulse);
	}
	else 
	{
		m_Impulse.x = m_Impulse.y = 0.0f;
	}
	
}

void CatenaryConstraintFixedRotation::correctVelocities()
{	
	PhysicalBody* pBody = m_PhysicalBody1;
	
	Vector2f vel = ( pBody->m_Velocity + Vector2f::cross(pBody->m_AngularVelocity, m_r) );
	dfloat Cdot = vel.dot(m_ImpulseDirection);	// Cdot = J * v		
	Vector2f Cdot1( Cdot, pBody->m_AngularVelocity);
	//dfloat correctiveImpulseMag = -m_EffectiveMass * ( m_PositionError + Cdot + m_Cfm * m_ImpulseMagnitude );
	//m_ImpulseMagnitude += correctiveImpulseMag;
	Vector2f cerror( m_PositionError, m_AngularError);
	Cdot1 += cerror;
	Cdot1 += m_Impulse * m_Cfm;
	
	Vector2f correctiveImpulse = -( m_EffectiveMassMatrix * Cdot1 );
	Vector2f oldImpulse = m_Impulse;
	m_Impulse.x = MIN(correctiveImpulse.x + m_Impulse.x, 0.0f);
	m_Impulse.y = correctiveImpulse.y + m_Impulse.y;	

	correctiveImpulse = m_Impulse - oldImpulse;

	Vector2f impulse = m_ImpulseDirection * correctiveImpulse.x;
	
	//printf("Impulse = %f   %f\n", m_Impulse.x, m_Impulse.y);
	// Apply Corrective impulse on the bodies due to Normal Impulse
	//if( ( m_TotalLength - m_Length ) > 1e-02f )
	pBody->m_Velocity        += ( impulse * pBody->m_InvMass );
	pBody->m_AngularVelocity += ( pBody->m_InvI * ( Vector2f::cross( m_r, impulse) + correctiveImpulse.y ) );
}

END_NAMESPACE_DIRAC2D
