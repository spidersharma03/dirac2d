/*
 *  WheelConstraint.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 08/07/11.
 *
 */

#include "WheelConstraint.h"
#include "../PhysicalBody.h"

BEGIN_NAMESPACE_DIRAC2D


WheelConstraint::WheelConstraint( const WheelConstraintInfo& cInfo): Constraint(cInfo)
{
    m_Type = ECT_WHEEL;
    m_Anchor = cInfo.m_Anchor;
    m_LocalAxis = cInfo.m_LocalAxis;
	m_Erp = 100.0f;
	m_Cfm = 10.0f;
}

void WheelConstraint::initialize()
{
	if( m_PhysicalBody1 )
		m_Anchor1 = m_PhysicalBody1->getLocalPoint(m_Anchor);
	if( m_PhysicalBody2 )
		m_Anchor2 = m_PhysicalBody2->getLocalPoint(m_Anchor);
	
	if( m_LocalAxis.lengthSquared() == 0.0f )
		m_LocalAxis.set(0.0f, 1.0f);
	m_LocalAxis.normalize();
	m_LocalPerpendicularAxis.set(-m_LocalAxis.y, m_LocalAxis.x);
}

void WheelConstraint::buildJacobian()
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
	dfloat aperp_cross_s  = m_WorldPerpendicularAxis.cross(s);
	dfloat aperp_cross_r2 = m_WorldPerpendicularAxis.cross(m_r2);
	
	m_WorldAxis = m_LocalAxis;
	body1->m_Transform.transformAsVector(m_WorldAxis);
	dfloat a_cross_s  = m_WorldAxis.cross(s);
	dfloat a_cross_r2 = m_WorldAxis.cross(m_r2);
	
	// Positional Error for Position Stabilization( Baumgarte )
	m_PositionError			= d.dot(m_WorldPerpendicularAxis) * m_Erp;
	m_PositionErrorParallel = d.dot(m_WorldAxis) * m_Erp;
	
	m_EffectiveMassMatrix.a11 = m1Inv + i1Inv * aperp_cross_s * aperp_cross_s + m2Inv + i2Inv * aperp_cross_r2 * aperp_cross_r2 + m_Cfm;
	m_EffectiveMassMatrix.a21 = i1Inv * aperp_cross_s * a_cross_s + i2Inv * aperp_cross_r2 * a_cross_r2;
	m_EffectiveMassMatrix.a12 = m_EffectiveMassMatrix.a21;
	m_EffectiveMassMatrix.a22 = m1Inv + i1Inv * a_cross_s * a_cross_s + m2Inv + i2Inv * a_cross_r2 * a_cross_r2 + m_Cfm;
	
	m_EffectiveMassMatrix.invert();
	
	// Apply Corrective impulse on the bodies
	if( 1 )
	{
		Vector2f impulse = m_WorldPerpendicularAxis * m_Impulse.x + m_WorldAxis * m_Impulse.y;
		
		body1->m_Velocity        -= impulse * m1Inv;
		body1->m_AngularVelocity -= i1Inv * ( Vector2f::cross( m_r1, impulse) );
		
		body2->m_Velocity        += impulse * m2Inv;
		body2->m_AngularVelocity += i2Inv * ( Vector2f::cross( m_r2, impulse) );
	}
	else 
	{
		m_Impulse.setZero();
	}
	
}

void WheelConstraint::correctVelocities()
{
	PhysicalBody* body1 = m_PhysicalBody1;
	PhysicalBody* body2 = m_PhysicalBody2;
	
	dfloat m1Inv = 0.0f;
	dfloat m2Inv = 0.0f;
	dfloat i1Inv = 0.0f;
	dfloat i2Inv = 0.0f;
	
	Vector2f v1,v2;
	dfloat w1 = 0.0f, w2 = 0.0f;
	{
		m1Inv = body1->m_InvMass;
		i1Inv = body1->m_InvI;
		v1 = body1->m_Velocity;
		w1 = body1->m_AngularVelocity;
	}
	{
		m2Inv = body2->m_InvMass;
		i2Inv = body2->m_InvI;
		v2 = body2->m_Velocity;
		w2 = body2->m_AngularVelocity;
	}
	
	// Relative Velocity = V2 - V1 = v2 + w2xr2 - v1 - w1xr1			
	Vector2f relVel = v2 + Vector2f::cross(w2, m_r2) - v1 - Vector2f::cross(w1, m_r1);
	dfloat Cdot1 = d.dot(Vector2f::cross(body1->m_AngularVelocity, m_WorldPerpendicularAxis))
	+ relVel.dot(m_WorldPerpendicularAxis);
	
	Cdot1 += m_PositionError;
	
	// Relative Velocity = V2 - V1 = v2 + w2xr2 - v1 - w1xr1			
	dfloat Cdot2 = d.dot(Vector2f::cross(body1->m_AngularVelocity, m_WorldAxis))
	+ relVel.dot(m_WorldAxis);
	
	Cdot2 += m_PositionErrorParallel + m_Cfm * m_Impulse.y;
	
	Vector2f correctiveImpulse;
	
	Vector2f Cdot( Cdot1, Cdot2);
	correctiveImpulse = -(m_EffectiveMassMatrix * Cdot);
	m_Impulse += correctiveImpulse;
	Vector2f impulse = m_WorldPerpendicularAxis * correctiveImpulse.x + m_WorldAxis * correctiveImpulse.y;
	
	body1->m_Velocity        -= impulse * m1Inv;
	body1->m_AngularVelocity -= i1Inv * Vector2f::cross( m_r1 + d, impulse);
	
	body2->m_Velocity        += impulse * m2Inv;
	body2->m_AngularVelocity += i2Inv * Vector2f::cross( m_r2, impulse);		
	
}


END_NAMESPACE_DIRAC2D
