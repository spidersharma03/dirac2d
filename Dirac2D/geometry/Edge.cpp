/*
 *  Edge.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 28/05/12.
 *
 */

#include "Edge.h"
#include "../maths/MathUtil.h"

BEGIN_NAMESPACE_DIRAC2D

Edge::Edge()
{
	m_ShapeType = EST_EDGE;
	m_bHasPrev = m_bHasNext = false;
}

Edge::Edge(Vector2f& vertex1, Vector2f& vertex2) : m_Vertex1(vertex1), m_Vertex2(vertex2)
{
	m_ShapeType = EST_EDGE;
	m_bHasPrev = m_bHasNext = false;
}

Edge::Edge(const Edge& other):CollisionShape(other)
{
	m_Vertex1 = other.m_Vertex1;
	m_Vertex2 = other.m_Vertex2;
	m_PrevVertex = other.m_PrevVertex;
	m_NextVertex = other.m_NextVertex;
	m_bHasPrev = other.m_bHasPrev;
	m_bHasNext = other.m_bHasNext;
}

void Edge::operator= ( Edge& other)
{
	m_Vertex1 = other.m_Vertex1;
	m_Vertex2 = other.m_Vertex2;
	m_PrevVertex = other.m_PrevVertex;
	m_NextVertex = other.m_NextVertex;
	m_bHasPrev = other.m_bHasPrev;
	m_bHasNext = other.m_bHasNext;
}

CollisionShape* Edge::clone()
{
	return new Edge(*this);
}

Vector2f Edge::getSupportPoint(Vector2f& d) 
{
	return Vector2f();
}

dbool Edge::isPointInside(Point2f& p)
{
	return false;
}

void Edge::updateAABB(Matrix3f& xForm)
{
	Vector2f v1 = xForm * m_Vertex1;
	Vector2f v2 = xForm * m_Vertex2;
	
	dfloat min_x = 100000.0f;
	dfloat min_y = 100000.0f;
	
	dfloat max_x = -100000.0f;
	dfloat max_y = -100000.0f;
	
	if( min_x > v1.x )
		min_x = v1.x;
	if( min_y > v1.y )
		min_y = v1.y;
	
	if( max_x < v1.x )
		max_x = v1.x;
	if( max_y < v1.y )
		max_y = v1.y;
	
	if( min_x > v2.x )
		min_x = v2.x;
	if( min_y > v2.y )
		min_y = v2.y;
	
	if( max_x < v2.x )
		max_x = v2.x;
	if( max_y < v2.y )
		max_y = v2.y;
	
	m_AABB.m_LowerBounds.set(min_x - EDGE_THICKNESS, min_y - EDGE_THICKNESS);
	m_AABB.m_UpperBounds.set(max_x + EDGE_THICKNESS, max_y + EDGE_THICKNESS);
}

void Edge::updateShape(Matrix3f& xForm)
{
	xForm.transformAsPoint(m_Vertex1);
	xForm.transformAsPoint(m_Vertex2);
}

void Edge::findCentroid()
{
}

void Edge::findMomentOfInertia()
{
	m_I = m_Vertex1.distanceSquared(m_Vertex2)/12.0f;
}

END_NAMESPACE_DIRAC2D
