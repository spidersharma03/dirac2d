/*
 *  RegularPolygon.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 03/05/12.
 *
 */


#include "RegularPolygon.h"
#include "../common.h"

BEGIN_NAMESPACE_DIRAC2D


RegularPolygon::RegularPolygon(Vector2f* vertices, dint32 numVertices) : m_NumVertices(numVertices)
{
	dAssert(m_NumVertices > 2);
	for( dint32 v=0; v< m_NumVertices; v++ )
		m_Vertices[v] = vertices[v];

	for( dint32 v=0; v< m_NumVertices-1; v++ )
	{
		Vector2f edge = m_Vertices[v+1] - m_Vertices[v];
		m_Normals[v].set(edge.y, -edge.x);
		m_Normals[v].normalize();
	}
	Vector2f edge = m_Vertices[0] - m_Vertices[m_NumVertices-1];
	m_Normals[m_NumVertices-1].set(edge.y, -edge.x);
	m_Normals[m_NumVertices-1].normalize();
	
	m_ShapeType = EST_REGULARPOLY;
	findCentroid();
	findMomentOfInertia();
}

RegularPolygon::RegularPolygon(const RegularPolygon& other)
{
	m_NumVertices = other.m_NumVertices;
	for( dint32 v=0; v< m_NumVertices; v++ )
		m_Vertices[v] = other.m_Vertices[v];
	
	for( dint32 v=0; v< m_NumVertices; v++ )
		m_Normals[v] = other.m_Normals[v];
	
	m_I = other.m_I;
	m_Centroid = other.m_Centroid;
	m_Radius   = other.m_Radius;
}

Vector2f RegularPolygon::getSupportPoint(Vector2f& d)
{
	return Vector2f();
}

dbool RegularPolygon::isPointInside(Point2f& p)
{
	return 0;
}

void RegularPolygon::updateAABB(Matrix3f& xForm)
{
	dfloat min_x = 100000.0f;
	dfloat min_y = 100000.0f;

	dfloat max_x = -100000.0f;
	dfloat max_y = -100000.0f;

	for( dint32 v=0; v<m_NumVertices; v++ )
	{
		Vector2f p = m_Vertices[v];
		xForm.transformAsPoint(p);
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

void RegularPolygon::findCentroid()
{
	m_Centroid.set(0.0f, 0.0f);
	dint32 i = 0;
	dfloat Area = 0.0f;
	const dfloat ONE_BY_THREE = 1.0f/3.0f;
	
	for( dint32 v=0; v<m_NumVertices; v++ )
	{
		Vector2f v1 = m_Vertices[v];
		i = v+1;
		if( v == m_NumVertices-1 )
			i = 0;
		Vector2f v2 = m_Vertices[i];
		
		// Find Area of the Triangle formed by the above vertices
		Vector2f edge1 = -v1;
		Vector2f edge2 = v2 - v1;
		dfloat A = 0.5f * edge2.cross(edge1);
		// Center of the Triangle
		Vector2f c = ( v1 + v2 ) * ONE_BY_THREE;

		// Wighted Sum ( Center = SUM ( Ai * Ci )/ SUM( Ai )
		Area += A;
		m_Centroid += c * A;
	}
	m_Area = Area;
	dAssert( m_Area > 0.0f );
	m_Centroid /= m_Area;
}

// From wikipedia.
void RegularPolygon::findMomentOfInertia()
{
	dfloat denom     = 0.0f;
	dfloat numerator = 0.0f;
	
	for( dint32 v=0; v<m_NumVertices-1; v++ )
	{
		Vector2f p1 = m_Vertices[v] - m_Centroid;
		Vector2f p2 = m_Vertices[v+1] - m_Centroid;
		dfloat d1  = p1.dot(p1);
		dfloat d2  = p2.dot(p2);
		dfloat d12 = p1.dot(p2);
		dfloat p1_cross_p2 = fabs(p1.cross(p2));
		denom += (d1 + d2 + d12) * p1_cross_p2;
		numerator += p1_cross_p2;
	}

	m_I = denom/numerator/6.0f;	
}

END_NAMESPACE_DIRAC2D