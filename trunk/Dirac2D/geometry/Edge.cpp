/*
 *  Edge.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 28/05/12.
 *
 */

#include "Edge.h"
#include "MathUtil.h"

BEGIN_NAMESPACE_DIRAC2D

Edge::Edge(Vector2f& vertex1, Vector2f& vertex2) : m_Vertex1(vertex1), m_Vertex2(vertex2)
{
	m_ShapeType = EST_EDGE;
}

Edge::Edge(const Edge& other): m_Vertex1(other.m_Vertex1), m_Vertex2(other.m_Vertex2)
{
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
	
	m_AABB.m_LowerBounds = v1;
	m_AABB.m_UpperBounds = v2;
}

void Edge::updateShape(Matrix3f& xForm)
{
}

void Edge::findCentroid()
{
}

void Edge::findMomentOfInertia()
{
}

END_NAMESPACE_DIRAC2D
