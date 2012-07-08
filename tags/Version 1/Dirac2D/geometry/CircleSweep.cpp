/*
 *  CircleSweep.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 04/07/12.
 *
 */

#include "CircleSweep.h"
#include "Edge.h"
#include "../common.h"

BEGIN_NAMESPACE_DIRAC2D

CircleSweep::CircleSweep(dfloat radius, Vector2f* vertices, dint32 numVertices):m_Radius(radius)
{
	dAssert(numVertices > 1);
	m_NumEdges = numVertices - 1;
	m_SweepList = new Edge[m_NumEdges];
	
	dint32 e = 0;
	Edge* sweep = m_SweepList + e;
	sweep->m_Vertex1 = vertices[e];
	sweep->m_Vertex2 = vertices[e+1];
	
	if( m_NumEdges > 1 )
	{
		sweep->m_bHasNext = true;
		sweep->m_NextVertex = vertices[e+2];
	}
	
	sweep->m_bHasNext = true;
	
	for( e=1; e<m_NumEdges-1; e++ )
	{
		sweep = m_SweepList + e;
		sweep->m_Vertex1 = vertices[e];
		sweep->m_Vertex2 = vertices[e+1];
		sweep->m_PrevVertex = vertices[e-1];
		sweep->m_NextVertex = vertices[e+2];
		sweep->m_bHasPrev = true;
		sweep->m_bHasNext = true;
	}
	
	sweep = m_SweepList + e;
	
	if( m_NumEdges > 1 )
	{
		sweep->m_bHasPrev = true;
		sweep->m_PrevVertex = vertices[e-1];
	}
	sweep->m_Vertex1 = vertices[e];
	sweep->m_Vertex2 = vertices[e+1];
	m_ShapeType = EST_CIRCLE_SWEEP;
	
	findCentroid();
	
	findMomentOfInertia();	
}

CircleSweep::CircleSweep(const CircleSweep& other)
{
}

void CircleSweep::operator= ( CircleSweep& other)
{
}

Vector2f CircleSweep::getSupportPoint(Vector2f& d)
{
	return Vector2f();
}

dbool CircleSweep::isPointInside(Vector2f& p)
{
	return 0;
}

void CircleSweep::updateAABB(Matrix3f& xForm)
{
}

CollisionShape* CircleSweep::clone()
{
	return new CircleSweep(*this);
}

void CircleSweep::updateShape(Matrix3f& xForm)
{
}

void CircleSweep::findCentroid()
{
}

void CircleSweep::findMomentOfInertia()
{
}

END_NAMESPACE_DIRAC2D