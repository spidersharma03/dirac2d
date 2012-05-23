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
