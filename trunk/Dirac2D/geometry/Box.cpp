/*
 *  Box.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 03/05/12.
 *
 */
#include "Box.h"

BEGIN_NAMESPACE_DIRAC2D

		
Box::Box(dfloat width, dfloat height):m_Width(width), m_Height(height)
{
	m_Area = m_Width * m_Height;
}

Box::Box(const Box& other)
{
	m_Width  = other.m_Width;
	m_Height = other.m_Height;
}

CollisionShape* Box::clone()
{
	return 0;
}

Vector2f Box::getSupportPoint(Vector2f& d)
{
	return Vector2f();
}

dbool Box::isPointInside(Point2f& p)
{
	return 0;
}

void Box::updateAABB(Matrix3f& xForm)
{
}

void Box::findCentroid()
{
}

void Box::findMomentOfInertia()
{
}

END_NAMESPACE_DIRAC2D
