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

CollisionShape* Edge::clone(MemoryBlockAllocator* pAllocator)
{
	return new(pAllocator->Allocate(sizeof(Edge))) Edge(*this);
}

Vector2f Edge::getSupportPoint(const Vector2f& d) 
{
	return m_Vertex1.dot(d) >= m_Vertex2.dot(d) ? m_Vertex1 : m_Vertex2;
}

dbool Edge::isPointInside(Vector2f& p)
{
	return false;
}

dbool Edge::intersectRaySegment(const Matrix3f& xForm, const RaySegment2f& raySeg, RayIntersectionInfo& intersectInfo)
{
	// Transform the Ray into Edge's space. this can be done by taking inverse of xForm and then transforming the ray. below implementation is more efficient.
	Matrix3f M = xForm;
	M.col3.x = M.col3.y = 0.0f;
	Vector2f T(xForm.col3.x, xForm.col3.y);
	Vector2f rayStart = ( raySeg.m_Start - T ) * M;
	Vector2f rayEnd   = ( raySeg.m_End - T ) * M;
	
	// Solve for the Segments( RaySegment and the Edge )
	dfloat dx1 = rayEnd.x - rayStart.x;
	dfloat dy1 = rayEnd.y - rayStart.y;
	dfloat dx2 = m_Vertex2.x - m_Vertex1.x;
	dfloat dy2 = m_Vertex2.y - m_Vertex1.y;
	
	dfloat Denom = dx1 * dy2 - dx2 * dy1;
	
	// Parallel Segments
	if ( Denom * Denom < EPSILON * EPSILON )
		return false;
	dfloat t1 = -( ( rayStart.x * dy2 - rayStart.y * dx2 ) + ( m_Vertex1.y * dx2 - m_Vertex1.x * dy2 ) )/Denom;
	dfloat t2 = -( ( rayStart.x * dy1 - rayStart.y * dx1 ) + ( m_Vertex1.y * dx1 - m_Vertex1.x * dy1 ) )/Denom;

	// Intersection point lies outside the segments.
	if( t1 < 0.0f || t1 > 1.0f || t2 < 0.0f || t2 > 1.0f)
		return false;
	
	intersectInfo.m_HitT = t1;
	intersectInfo.m_HitNormal = Vector2f( dy2, -dx2 );
	intersectInfo.m_HitNormal.normalize();
	intersectInfo.m_HitPoint = raySeg.m_Start + (raySeg.m_End - raySeg.m_Start) * intersectInfo.m_HitT;

	return true;
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
