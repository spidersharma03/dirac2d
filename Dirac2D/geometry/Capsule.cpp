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

Capsule::Capsule(const Capsule& other) : CollisionShape(other)
{
	m_Radius = other.m_Radius;
	m_Height = other.m_Height;
}

CollisionShape* Capsule::clone()
{
	return new Capsule(*this);
}

Vector2f Capsule::getSupportPoint(const Vector2f& d)
{
	Vector2f dir = d;
	dir.normalize();
    dfloat signx = dir.x > 0.0f ? 1.0f : -1.0f;
    
	return Vector2f(signx * m_Height * 0.5f + dir.x * m_Radius, m_Radius * dir.y);
}

dbool Capsule::isPointInside(Vector2f& p)
{
	Vector2f p0(-m_Height*0.5f + m_Centroid.x, 0.0f), p1(m_Height*0.5f + m_Centroid.x, 0.0f); 
	Vector2f outPoint;
	findClosestPoint(p0, p1, p, outPoint);
	return p.distanceSquared(outPoint) < m_Radius * m_Radius;
}

dbool Capsule::intersectRaySegment(const Matrix3f& xForm, const RaySegment2f& raySeg, RayIntersectionInfo& intersectInfo)
{
	return false;
}

void Capsule::updateAABB(Matrix3f& xForm)
{
	Vector2f p0(-m_Height*0.5f + m_Centroid.x - m_Radius, m_Radius + m_Centroid.y), p1(m_Height*0.5f + m_Radius + m_Centroid.x, m_Radius+ m_Centroid.y); 
	Vector2f p2(-m_Height*0.5f + m_Centroid.x - m_Radius, -m_Radius+ m_Centroid.y), p3(m_Height*0.5f + m_Radius + m_Centroid.x, -m_Radius+ m_Centroid.y); 
	
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

void Capsule::updateShape(Matrix3f& xForm)
{
	xForm.transformAsPoint(m_Centroid);
}

void Capsule::findCentroid()
{
}

void Capsule::findMomentOfInertia()
{
	m_I = 0.0f;
	dfloat rectCapsulemassRatio = 2.0f * m_Height * m_Radius/m_Area;
	dfloat circleCapsulemassRatio = PI * m_Radius * m_Radius/m_Area;
	
	m_I += ( ( m_Height * m_Height + 4.0f * m_Radius * m_Radius ) * rectCapsulemassRatio/12.0f ); // Moment of the rect box about the centre
	m_I += ( ( ( m_Radius * m_Radius/18.0f + (m_Height*0.5f + 2.0f*m_Radius/3.0f)*(m_Height*0.5f + 2.0f*m_Radius/3.0f) ) ) * circleCapsulemassRatio ); // Moment of the half circles about the centre.
}

END_NAMESPACE_DIRAC2D
