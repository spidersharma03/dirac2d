/*
 *  Circle.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 03/05/12.
 *
 */


#include "Circle.h"

BEGIN_NAMESPACE_DIRAC2D


Circle::Circle(dfloat radius):m_Radius(radius)
{
	m_Area = PI * m_Radius * m_Radius;
}

Circle::Circle(const Circle& other)
{
}

Vector2f Circle::getSupportPoint(Vector2f& d)
{
	return Vector2f();
}

dbool Circle::isPointInside(Point2f& p)
{
	return 0;
}

void Circle::updateAABB(Matrix3f& xForm)
{
}

void Circle::findCentroid()
{
}

void Circle::findMomentOfInertia()
{
	m_I = m_Radius * m_Radius * 0.5f;
}

END_NAMESPACE_DIRAC2D