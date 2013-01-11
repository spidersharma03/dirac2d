/*
 *  CubicSplinePathConstraint.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 14/07/11.
 *
 */

#include "CubicSplinePathConstraint.h"
#include "../PhysicalBody.h"
#include <stdio.h>

BEGIN_NAMESPACE_DIRAC2D

//CubicSplinePathConstraint::CubicSplinePathConstraint(Vector2f* splineVertices, int numVertices)
//{
//	m_NumSplineVertices = numVertices;
//	dAssert(m_NumSplineVertices>3);
//	m_SplineVertices = new Vector2f[numVertices];
//	for( dint32 v=0; v<numVertices; v++ )
//	{
//		m_SplineVertices[v] = splineVertices[v];
//	}
//	
//	
//	m_Type = ECT_CUBIC_SPLINE;
//	m_Erp = 0.0f;
//	m_Cfm = 0.0f;
//	m_LowerLimit = -0.4f;
//	m_UpperLimit = 0.4f;
//}

void CubicSplinePathConstraint::initialize()
{
	if( m_PhysicalBody1 )
		m_Anchor1 = m_PhysicalBody1->getLocalPoint(m_Anchor);
	if( m_PhysicalBody2 )
		m_Anchor2 = m_PhysicalBody2->getLocalPoint(m_Anchor);
	
	if( m_LocalAxis.lengthSquared() == 0.0f )
		m_LocalAxis.set(0.0f, 1.0f);
	m_LocalAxis.normalize();
	m_LocalPerpendicularAxis.set(-m_LocalAxis.y, m_LocalAxis.x);
	
	dfloat D[10];
	dfloat y[10];
	dfloat x[10];
	dfloat rhs[10];
	
	for ( dint32 i = 0 ; i < m_NumSplineVertices ; i ++)
	{
        x[i] = m_SplineVertices[i].x;
		y[i] = m_SplineVertices[i].y;
	}
	
	for ( dint32 i = 1 ; i < m_NumSplineVertices ; i ++)
	{
        rhs[i] = 3 * ( y[i+1] - y[i-1] );
	}
	
	rhs[0] = 3 * ( y[1] - y[0]);
	rhs[m_NumSplineVertices-1] = 3 * ( y[m_NumSplineVertices-1] - y[m_NumSplineVertices-2]);
	
	tridia_sl(rhs, D, m_NumSplineVertices);
	
	//findSplineCoeff(spline, D, y);
	
	
	//cubicSplinesy = new CubicSpline[count-1];
	
	//for ( int i = 0 ; i < count - 1 ; i ++){
//        //cubicSplinesy[i] = new CubicSpline();
//        cubicSplinesy[i].a = spline[i].a;
//        cubicSplinesy[i].b = spline[i].b;
//        cubicSplinesy[i].c = spline[i].c;
//        cubicSplinesy[i].d = spline[i].d;
//	}
	
}

void CubicSplinePathConstraint::buildJacobian(dfloat dt)
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
	m_PositionError			= d.dot(m_WorldPerpendicularAxis) * 1.0f;
	
	dfloat constraintDisplacement = d.dot(m_WorldAxis);
	
	if( fabs(m_UpperLimit - m_LowerLimit) < LINEAR_ERROR )
	{
		m_PositionErrorParallel = ( constraintDisplacement - m_LowerLimit ) * 1.0f;
		m_LimitState = ECLS_LOWER_UPPER;
	}
	else if( constraintDisplacement >= m_UpperLimit )
	{
		m_PositionErrorParallel = ( constraintDisplacement - m_UpperLimit ) * 1.0f;
		m_LimitState = ECLS_UPPER;
	}
	else if( constraintDisplacement <= m_LowerLimit )
	{
		m_PositionErrorParallel = ( constraintDisplacement - m_LowerLimit ) * 1.0f;
		m_LimitState = ECLS_LOWER;
	}
	else
	{
		m_Impulse.y = 0.0f;
		m_LimitState = ECLS_NONE;
	}
	
	m_EffectiveMassMatrix.a11 = m1Inv + i1Inv * aperp_cross_s * aperp_cross_s + m2Inv + i2Inv * aperp_cross_r2 * aperp_cross_r2;
	m_EffectiveMassMatrix.a21 = i1Inv * aperp_cross_s * a_cross_s + i2Inv * aperp_cross_r2 * a_cross_r2;
	m_EffectiveMassMatrix.a12 = m_EffectiveMassMatrix.a21;
	m_EffectiveMassMatrix.a22 = m1Inv + i1Inv * a_cross_s * a_cross_s + m2Inv + i2Inv * a_cross_r2 * a_cross_r2 + m_Cfm;
	
	//m_EffectiveMassMatrix.invert();
	
	// Apply Corrective impulse on the bodies
	if( 1 )
	{
		Vector2f impulse = m_WorldPerpendicularAxis * m_Impulse.x + m_WorldAxis * m_Impulse.y;
		
		body1->m_Velocity        -= impulse * m1Inv;
		body1->m_AngularVelocity -= i1Inv * ( Vector2f::cross( m_r1 + d, impulse) );
		
		body2->m_Velocity        += impulse * m2Inv;
		body2->m_AngularVelocity += i2Inv * ( Vector2f::cross( m_r2, impulse) );
	}
	else 
	{
		m_Impulse.setZero();
	}
	
}

void CubicSplinePathConstraint::correctVelocities()
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
	
	Cdot2 += m_PositionErrorParallel + m_Impulse.y * m_Cfm;
	
	Vector2f correctiveImpulse;
	
	if( m_LimitState != ECLS_NONE )
	{		
		// First Solve for the Complete 2x2 Jacobian Matrix( ie all constraints simultaneously).		
		Vector2f Cdot = Vector2f( Cdot1,  Cdot2 );
		correctiveImpulse = -( m_EffectiveMassMatrix.solve(Cdot) );
		
		// Both Lower and Upper Limits are Same. we need to only solve the Complete 2x2 Jacobian Matrix.
		if( m_LimitState == ECLS_LOWER_UPPER )
		{
			m_Impulse += correctiveImpulse;
		}
		// Lower Limit.
		else if ( m_LimitState == ECLS_LOWER )
		{
			dfloat newImpulseY = m_Impulse.y + correctiveImpulse.y;
			// we need to clamp the impulse to 0.0f as this impulse should not effect the Line Constraint. we should solve for 
			// the new corrective impulse again as because of clamping the old solution has become invalid.
			if( newImpulseY < 0.0f )
			{
				dfloat rhs = (-Cdot1 + m_EffectiveMassMatrix.a12 * m_Impulse.y);
				dfloat newCorrectiveImpulse = rhs/m_EffectiveMassMatrix.a11;
				correctiveImpulse.x = newCorrectiveImpulse;
				correctiveImpulse.y = -m_Impulse.y;
				m_Impulse.x += correctiveImpulse.x;
				// clamp the impulse
				m_Impulse.y = 0.0f;
			}
			// handle the Lower Linear Limit case.
			else
			{
				m_Impulse += correctiveImpulse;
			}
		}
		else if ( m_LimitState == ECLS_UPPER )
		{
			dfloat newImpulseY = m_Impulse.y + correctiveImpulse.y;
			// we need to clamp the impulse to 0.0f as this impulse should not effect the Hinge Constraint. we should solve for 
			// the new corrective impulse again as because of clamping the old solution has become invalid.
			if( newImpulseY > 0.0f )
			{
				dfloat rhs = (-Cdot1 + m_EffectiveMassMatrix.a12 * m_Impulse.y);
				dfloat newCorrectiveImpulse = rhs/m_EffectiveMassMatrix.a11;
				correctiveImpulse.x = newCorrectiveImpulse;
				correctiveImpulse.y = -m_Impulse.y;
				m_Impulse.x += correctiveImpulse.x;
				// clamp the impulse
				m_Impulse.y = 0.0f;
			}
			// handle the Upper Linear Limit case.
			else
			{
				m_Impulse += correctiveImpulse;
			}
		}
	}
	// There is no Limit on the Linear motion.
	else 
	{
		correctiveImpulse.x = -Cdot1/m_EffectiveMassMatrix.a11;
		correctiveImpulse.y = 0.0f;
		m_Impulse.x += correctiveImpulse.x;
		m_Impulse.y = 0.0f;
	}
	
	
	Vector2f linearImpulse = m_WorldPerpendicularAxis * correctiveImpulse.x;
	Vector2f limitImpulse  = m_WorldAxis * correctiveImpulse.y;
	
	Vector2f impulse = linearImpulse + limitImpulse;
	
	//dfloat l1 = Vector2f::cross( m_r1 + d, linearImpulse) + Vector2f::cross( m_r1 + d, limitImpulse);
	//dfloat l2 = Vector2f::cross( m_r2, linearImpulse) + Vector2f::cross( m_r2, limitImpulse);
	
	body1->m_Velocity        -= impulse * m1Inv;
	body1->m_AngularVelocity -= i1Inv * Vector2f::cross( m_r1 + d, impulse);
	
	body2->m_Velocity        += impulse * m2Inv;
	body2->m_AngularVelocity += i2Inv * Vector2f::cross( m_r2, impulse);		
	
}


END_NAMESPACE_DIRAC2D