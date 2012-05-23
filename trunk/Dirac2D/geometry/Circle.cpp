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
	
	m_ShapeType = EST_CIRCLE;

	findMomentOfInertia();
	
	m_AABB.m_LowerBounds.x = -m_Radius;
	m_AABB.m_LowerBounds.y = -m_Radius;

	m_AABB.m_UpperBounds.x = m_Radius;
	m_AABB.m_UpperBounds.y = m_Radius;
}

Circle::Circle(const Circle& other)
{
	findMomentOfInertia();
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
	m_AABB.m_LowerBounds.set(-m_Radius + xForm.col3.x, -m_Radius + xForm.col3.y);
	m_AABB.m_UpperBounds.set(m_Radius + xForm.col3.x, m_Radius + xForm.col3.y);
}

void Circle::findCentroid()
{
}

void Circle::findMomentOfInertia()
{
	m_I = m_Radius * m_Radius * 0.5f;
}

END_NAMESPACE_DIRAC2D