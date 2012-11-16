/*
 *  EdgeChain.h
 *  Dirac2D
 *
 *  Created by Prashant on 28/05/12.
 *
 */

#ifndef _EDGE_CHAIN_H_
#define _EDGE_CHAIN_H_

#include "CollisionShape.h"

BEGIN_NAMESPACE_DIRAC2D

class Edge;

class EdgeChain : public CollisionShape
{
public:
	EdgeChain(Vector2f* vertices, dint32 numVertices);
	
	EdgeChain(const EdgeChain& other);
	
	void operator= ( EdgeChain& other);

	virtual Vector2f getSupportPoint(const Vector2f& d);
	
	virtual dbool isPointInside(Vector2f& p);
	
	virtual void updateAABB(Matrix3f& xForm);
	
	Edge* getEdge(dint32 index) const;

	virtual CollisionShape* clone();

	inline dint32 getNumEdges() const
	{
		return m_NumEdges;
	}
	
	inline dbool isClosed() const
	{
		return ( m_EdgeList->m_Vertex1 == (m_EdgeList+m_NumEdges)->m_Vertex1
				&& (m_EdgeList->m_Vertex2 == (m_EdgeList+m_NumEdges)->m_Vertex2) );
	}
	
    virtual ~EdgeChain()
    {
        delete [] m_EdgeList;
    }
protected:
	
	virtual void updateShape(Matrix3f& xForm);
	
	virtual void findCentroid();
	
	virtual void findMomentOfInertia();
	
public:
	Vector2f* m_Normals;
	
protected:
	dint32 m_NumEdges;
	Edge* m_EdgeList;
};

END_NAMESPACE_DIRAC2D

#endif