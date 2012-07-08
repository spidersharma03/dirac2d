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
	m_Type = ECT_CATENARY_FIXED_ROTATION;
	m_FixedLength = 0.0f;
	m_Cfm = 0.0f;
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
		
	if( len1 > 0.0f )
		l1 /= len1;
	if( len2 > 0.0f )
		l2 /= len2;
	
	m_ImpulseDirection = l1 + l2;
	
	dfloat rcrossX = m_r.cross(m_ImpulseDirection);
	
	// Effective Mass Matrix for the Constraint.	
	m_EffectiveMassMatrix.a11 = pBody->m_InvMass * m_ImpulseDirection.dot(m_ImpulseDirection) + pBody->m_InvI * rcrossX * rcrossX + m_Cfm;
	m_EffectiveMassMatrix.a21 = pBody->m_InvI * rcrossX;
	m_EffectiveMassMatrix.a12 = m_EffectiveMassMatrix.a21;
	m_EffectiveMassMatrix.a22 = pBody->m_InvI + m_Cfm;
			
	// Positional Error for Position Stabilization( Baumgarte )
	m_TotalLength = len1 + len2;
	m_PositionError = m_Erp * ( m_TotalLength - m_FixedLength );
	m_AngularError  = m_Erp * ( pBody->m_Angle - m_ReferenceAngle );
	
	if( m_TotalLength - m_FixedLength <= 0.0f )
	{
		m_LengthLimitState = ELLS_LOWER;
	}
	else
	{
		m_LengthLimitState = ELLS_NONE;
		m_Impulse.x = 0.0f;
	}

	// Apply Corrective impulse on the bodies
	if( 1 )
	{
		Vector2f impulse = m_ImpulseDirection * m_Impulse.x;
		pBody->m_Velocity        += impulse * pBody->m_InvMass;
		pBody->m_AngularVelocity += pBody->m_InvI * ( Vector2f::cross( m_r, impulse) + m_Impulse.y );
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
	Vector2f Cdot1( Cdot + m_PositionError, pBody->m_AngularVelocity + m_AngularError);
	Cdot1 += m_Impulse * m_Cfm;
	
	Vector2f correctiveImpulse = -m_EffectiveMassMatrix.solve(Cdot1);
	if( 1 )//m_LengthLimitState == ELLS_LOWER )
	{
		dfloat newImpulse = m_Impulse.x + correctiveImpulse.x;
		// we need to clamp the impulse to 0.0f as this impulse should not effect the Catenary Constraint. we should solve for 
		// the new corrective impulse again, as because of clamping the old solution has become invalid.
		if( newImpulse > 0.0f )
		{
			dfloat rhs = -Cdot1.y + m_EffectiveMassMatrix.a21 * m_Impulse.x;
			dfloat newCorrectiveImpulse = rhs/m_EffectiveMassMatrix.a22;
			correctiveImpulse.x = -m_Impulse.x;
			correctiveImpulse.y = newCorrectiveImpulse;
			m_Impulse.y += correctiveImpulse.y;
			m_Impulse.x = 0.0f;
		}
		else 
		{
			m_Impulse += correctiveImpulse;
		}
	}
	else 
	{
		m_Impulse += correctiveImpulse;
		m_Impulse.x = 0.0f;
	}
	
	Vector2f impulse = m_ImpulseDirection * correctiveImpulse.x;
	
	// Apply Corrective impulse on the bodies due to Normal Impulse
	pBody->m_Velocity        += ( impulse * pBody->m_InvMass );
	pBody->m_AngularVelocity += ( pBody->m_InvI * ( Vector2f::cross( m_r, impulse) + correctiveImpulse.y ) );
}

END_NAMESPACE_DIRAC2D
