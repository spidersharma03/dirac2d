/*
 *  EdgeChain.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 28/05/12.
 *
 */

#include "../common.h"
#include "Edge.h"
#include "EdgeChain.h"

BEGIN_NAMESPACE_DIRAC2D

EdgeChain::EdgeChain(Vector2f* vertices, dint32 numVertices)
{
	dAssert(numVertices > 1);
	m_NumEdges = numVertices - 1;
	m_EdgeList = new Edge[m_NumEdges];
	
	dint32 e = 0;
	Edge* edge = m_EdgeList + e;
	edge->m_Vertex1 = vertices[e];
	edge->m_Vertex2 = vertices[e+1];
	
	if( m_NumEdges > 1 )
	{
		edge->m_bHasNext = true;
		edge->m_NextVertex = vertices[e+2];
	}
	
	edge->m_bHasNext = true;

	for( e=1; e<m_NumEdges-1; e++ )
	{
		edge = m_EdgeList + e;
		edge->m_Vertex1 = vertices[e];
		edge->m_Vertex2 = vertices[e+1];
		edge->m_PrevVertex = vertices[e-1];
		edge->m_NextVertex = vertices[e+2];
		edge->m_bHasPrev = true;
		edge->m_bHasNext = true;
	}
	
	edge = m_EdgeList + e;
	
	if( m_NumEdges > 1 )
	{
		edge->m_bHasPrev = true;
		edge->m_PrevVertex = vertices[e-1];
	}
	edge->m_Vertex1 = vertices[e];
	edge->m_Vertex2 = vertices[e+1];
	m_ShapeType = EST_EDGE_CHAIN;
	
	findCentroid();
	
	findMomentOfInertia();
}

EdgeChain::EdgeChain(const EdgeChain& other) : CollisionShape(other)
{
	m_NumEdges = other.m_NumEdges;
	m_EdgeList = new Edge[m_NumEdges];
	for( dint32 e=0; e<m_NumEdges; e++ )
		m_EdgeList[e] = other.m_EdgeList[e];
}

void EdgeChain::operator= ( EdgeChain& other)
{
	m_NumEdges = other.m_NumEdges;
	for( dint32 e=0; e<m_NumEdges; e++ )
		m_EdgeList[e] = other.m_EdgeList[e];
}

CollisionShape* EdgeChain::clone()
{
	return new EdgeChain(*this);
}

Vector2f EdgeChain::getSupportPoint(Vector2f& d)
{
	return Vector2f();
}

dbool EdgeChain::isPointInside(Vector2f& p)
{
	if( !isClosed() )
		return false;
	
	for( dint32 e=0; e<m_NumEdges; e++ )
	{
		Edge* edge = m_EdgeList + e;
		edge->m_Vertex1;
	}
	return false;
}

void EdgeChain::updateAABB(Matrix3f& xForm)
{
	for( dint32 e=0; e<m_NumEdges; e++ )
		(m_EdgeList+e)->updateAABB(xForm);
}

Edge* EdgeChain::getEdge(dint32 index) const
{
	return m_EdgeList + index;
}

void EdgeChain::updateShape(Matrix3f& xForm)
{
	for( dint32 e=0; e<m_NumEdges; e++ )
		(m_EdgeList+e)->updateShape(xForm);
}

void EdgeChain::findCentroid()
{
	m_Centroid.set(0.0f, 0.0f);
	m_Area = 0.0f;
	for( dint32 e=0; e<m_NumEdges; e++ )
	{
		Edge* edge = m_EdgeList + e;
		Vector2f& v1 = edge->m_Vertex1;
		Vector2f& v2 = edge->m_Vertex2;
		Vector2f c = ( v1 + v2 ) * 0.5f;
		dfloat edgeLength = v1.distance(v2);
		m_Area += edgeLength;
		m_Centroid += c;
	}
	dAssert(m_Area > 0.0f );
	m_Centroid /= (dfloat)m_NumEdges;
}

void EdgeChain::findMomentOfInertia()
{
	m_I = 0.0f;
	for( dint32 e=0; e<m_NumEdges; e++ )
	{
		Edge* edge = m_EdgeList + e;
		Vector2f& v1 = edge->m_Vertex1;
		Vector2f& v2 = edge->m_Vertex2;
		Vector2f c = ( v1 + v2 ) * 0.5f;
		dfloat edgeLength = v1.distance(v2);
		dfloat I = edgeLength/m_Area * edgeLength*edgeLength/12.0f;
		Vector2f d = c - m_Centroid;
		m_I += ( I + edgeLength/m_Area * d.lengthSquared() ); 
	}
	dAssert( m_I > 0.0f );
}

END_NAMESPACE_DIRAC2D