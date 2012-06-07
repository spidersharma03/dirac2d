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

Circle::Circle(const Circle& other) : CollisionShape(other)
{
	m_Radius = other.m_Radius;
}

void Circle::operator= ( Circle& other)
{
	m_Radius = other.m_Radius;
}

CollisionShape* Circle::clone()
{
	return new Circle(*this);
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
	Vector2f p0(m_Centroid);
	Vector2f p1;
	
	xForm.transformAsPoint(p0);
	p1 = p0;
	
	m_AABB.m_LowerBounds.set(p0.x-m_Radius, p0.y-m_Radius);
	m_AABB.m_UpperBounds.set(p1.x+m_Radius, p1.y+m_Radius);
}

void Circle::updateShape(Matrix3f& xForm)
{
	xForm.transformAsPoint(m_Centroid);
}

void Circle::findCentroid()
{
}

void Circle::findMomentOfInertia()
{
	m_I = m_Radius * m_Radius * 0.5f;
}

END_NAMESPACE_DIRAC2D