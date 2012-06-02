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
	
	for( e=1; e<m_NumEdges; e++ )
	{
		edge = m_EdgeList + e;
		edge->m_Vertex1 = vertices[e];
		edge->m_Vertex2 = vertices[e+1];
		edge->m_bHasPrev = true;
		edge->m_bHasNext = true;
	}
	
	edge->m_Vertex1 = vertices[e];
	edge->m_Vertex2 = vertices[e+1];
}

EdgeChain::EdgeChain(const EdgeChain& other)
{
}

Vector2f EdgeChain::getSupportPoint(Vector2f& d)
{
	return Vector2f();
}

dbool EdgeChain::isPointInside(Point2f& p)
{
	return false;
}

void EdgeChain::updateAABB(Matrix3f& xForm)
{
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
}

void EdgeChain::findMomentOfInertia()
{
}

END_NAMESPACE_DIRAC2D