/*
 *  Capsule.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 03/05/12.
 *
 */

#include "Capsule.h"

BEGIN_NAMESPACE_DIRAC2D


Capsule::Capsule(dfloat radius, dfloat height):m_Radius(radius), m_Height(height)
{
	m_Area = PI * m_Radius * m_Radius + 2.0f * m_Radius * m_Height;
	
	m_ShapeType = EST_CAPSULE;
	
	findMomentOfInertia();
}

Capsule::Capsule(const Capsule& other)
{
}

Vector2f Capsule::getSupportPoint(Vector2f& d)
{
	return Vector2f();
}

dbool Capsule::isPointInside(Point2f& p)
{
	return 0;
}

void Capsule::updateAABB(Matrix3f& xForm)
{
	Vector2f p0(-m_Height*0.5f - m_Radius, m_Radius), p1(m_Height*0.5f + m_Radius, m_Radius); 
	Vector2f p2(-m_Height*0.5f - m_Radius, -m_Radius), p3(m_Height*0.5f + m_Radius, -m_Radius); 
		
	xForm.transformAsPoint(p0);
	xForm.transformAsPoint(p1);
	xForm.transformAsPoint(p2);
	xForm.transformAsPoint(p3);

	Vector2f vertices[4] = {p0, p1, p2, p3};

	dfloat min_x = 100000.0f;
	dfloat min_y = 100000.0f;
	
	dfloat max_x = -100000.0f;
	dfloat max_y = -100000.0f;
	
	for( dint32 v=0; v<4; v++ )
	{
		Vector2f p = vertices[v];
		if( min_x > p.x )
			min_x = p.x;
		if( min_y > p.y )
			min_y = p.y;
		
		if( max_x < p.x )
			max_x = p.x;
		if( max_y < p.y )
			max_y = p.y;
	}
	
	m_AABB.m_LowerBounds.set(min_x, min_y);
	m_AABB.m_UpperBounds.set(max_x, max_y);
}

void Capsule::findCentroid()
{
}

void Capsule::findMomentOfInertia()
{
	m_I = 0.0f;
	m_I += ( m_Height * m_Height + 4.0f * m_Radius * m_Radius ); // Moment of the rect box about the centre
	m_I += ( 2.0f * ( m_Radius * m_Radius/18.0f + (m_Height*0.5f + 2.0f*m_Radius/3.0f)*(m_Height*0.5f + 2.0f*m_Radius/3.0f) ) ); // Moment of the half circles about the centre.
}

END_NAMESPACE_DIRAC2D
