/*
 *  WeldConstraint.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 26/06/12.
 *
 */

#include "dynamics/joints/WeldConstraint.h"
#include "dynamics/PhysicalBody.h"
#include "dynamics/PhysicalShape.h"

BEGIN_NAMESPACE_DIRAC2D


WeldConstraint::WeldConstraint(const WeldConstraintInfo& cInfo):Constraint(cInfo)
{
    m_Type = ECT_WELD;
    m_Anchor = cInfo.m_Anchor;
    m_Cfm = 0.4f;
	m_Frequency = 5.0f;
	m_DampingRatio = 0.2f;
}

void WeldConstraint::initialize()
{
	if( m_PhysicalBody1 )
		m_Anchor1 = m_PhysicalBody1->getLocalPoint(m_Anchor);
	if( m_PhysicalBody2 )
		m_Anchor2 = m_PhysicalBody2->getLocalPoint(m_Anchor);
	m_InitialAngle = m_PhysicalBody2->m_Angle - m_PhysicalBody1->m_Angle;
}

void WeldConstraint::buildJacobian(dfloat dt)
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
	m_EffectiveMass.col3.z = i1Inv + i2Inv;
	
	
	dfloat effectiveMass = i1Inv + i2Inv;
	effectiveMass = effectiveMass != 0.0f ? 1.0f/effectiveMass : effectiveMass;
	
	dfloat angularFrequency = 2.0f * PI * m_Frequency;
	dfloat k = effectiveMass * angularFrequency * angularFrequency;
	dfloat c = 2.0f * m_DampingRatio * effectiveMass * angularFrequency;
	
	dfloat cfm = (c + k * dt) * dt;
	cfm = cfm != 0.0f ? 1.0f/cfm : cfm;
	m_Cfm = cfm;
	
	m_EffectiveMass.col3.z = i1Inv + i2Inv + m_Cfm;

	m_EffectiveMass.invert();

	// Error reduction parameter
	dfloat erp = dt * k * m_Cfm;
	
	// Positional and Angular Error for Position Stabilization( Baumgarte )
	m_PositionError = ( c2 + m_r2 - c1 - m_r1 ) * erp;
	m_AngleError    = (  ( m_PhysicalBody2->m_Angle - m_PhysicalBody1->m_Angle ) - m_InitialAngle ) * erp;

	// Apply Corrective impulse on the bodies
	if( 1 )//body1->m_PhysicalWorld->m_bWarmStart )
	{
		Vector2f impulse = Vector2f( m_Impulse.x, m_Impulse.y);
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
		m_Impulse.x = m_Impulse.y = 0.0f;
	}
	
}

void WeldConstraint::correctVelocities()
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
	
	// Relative Velocity = V2 - V1 = v2 + w2xr2 - v1 - w1xr1			
	Vector2f Cdot = v2 + Vector2f::cross(w2, m_r2) - v1 - Vector2f::cross(w1, m_r1);
	Cdot += m_PositionError;

	Vector3f Rhs( Cdot, w2-w1 + m_AngleError + m_Cfm * m_Impulse.z);
	
	Vector3f correctiveImpulse = -( m_EffectiveMass * Rhs );
	m_Impulse += correctiveImpulse;
	
	Vector2f impulse( correctiveImpulse.x, correctiveImpulse.y); 
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