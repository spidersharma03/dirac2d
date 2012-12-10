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

Vector2f Box::getSupportPoint(const Vector2f& d)
{
    dfloat signx = d.x > 0.0f ? 1.0f : -1.0f;
    dfloat signy = d.y > 0.0f ? 1.0f : -1.0f;

	return Vector2f(signx * m_Width * 0.5f, signy * m_Height * 0.5f);
}

dbool Box::isPointInside(Vector2f& p)
{
	return 0;
}

dbool Box::intersectRaySegment(const Matrix3f& xForm, const RaySegment2f& raySeg, RayIntersectionInfo& intersectInfo)
{
	return false;
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
