/*
 *  Circle.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 03/05/12.
 *
 */


#include "Circle.h"

BEGIN_NAMESPACE_DIRAC2D


Circle::Circle(dfloat radius)
{
	m_Radius = radius;
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

Circle::~Circle()
{
}

CollisionShape* Circle::clone(MemoryBlockAllocator* pAllocator)
{
	return new(pAllocator->Allocate(sizeof(Circle))) Circle(*this);
}

Vector2f Circle::getSupportPoint(const Vector2f& d)
{
	Vector2f dir = d;
	dir.normalize();
	return m_Centroid + m_Radius * dir;
}

Vector2f Circle::getSupportPointWithoutRadius(const Vector2f& d)
{
	return m_Centroid;
}

dbool Circle::isPointInside(Vector2f& p)
{
	return p.distanceSquared(m_Centroid) < m_Radius * m_Radius;
}

dbool Circle::intersectRaySegment(const Matrix3f& xForm, const RaySegment2f& raySeg, RayIntersectionInfo& intersectInfo)
{
	// Transform Ray Segment into Circle's space.
	Vector2f rayOrigin = raySeg.m_Start - xForm * m_Centroid;
	
	const Vector2f d = raySeg.m_End - raySeg.m_Start;
	dfloat A = d.lengthSquared();
	dfloat B = rayOrigin.dot(d);
	dfloat C = rayOrigin.lengthSquared() - m_Radius * m_Radius;
	
	//Discriminant
	dfloat D = B * B - A * C;
	
	// No Intersection
	if( D < 0.0f )
		return false;
	
	// Get the lowest root
	dfloat hitT = -(B + sqrt(D))/A;
	intersectInfo.m_HitT = hitT;
	intersectInfo.m_HitPoint = raySeg.m_Start + d * hitT;
	intersectInfo.m_HitNormal = raySeg.m_Start + d * hitT - xForm * m_Centroid;
	intersectInfo.m_HitNormal.normalize();
	
	return true;
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