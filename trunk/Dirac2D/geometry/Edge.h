/*
 *  Edge.h
 *  Dirac2D
 *
 *  Created by Prashant on 28/05/12.
 *
 */

#ifndef _EDGE_H_
#define _EDGE_H_

#include "CollisionShape.h"

BEGIN_NAMESPACE_DIRAC2D

class Edge : public CollisionShape
{
public:
	virtual Vector2f getSupportPoint(const Vector2f& d);
	
	virtual dbool isPointInside(Vector2f& p);
	
	virtual dbool intersectRaySegment(const Matrix3f& xForm, const RaySegment2f& raySeg, RayIntersectionInfo& intersectInfo);

	virtual void updateAABB(Matrix3f& xForm);
	    
    virtual ~Edge()
    {
    }
    
    friend class PhysicalBody;
    friend class CircleSweep;
    
protected:
	
    Edge();
	
	Edge(Vector2f& vertex1, Vector2f& vertex2);
	
	Edge(const Edge& other);
	
	void operator= ( Edge& other);

    virtual CollisionShape* clone(MemoryBlockAllocator* pAllocator);

	virtual void updateShape(Matrix3f& xForm);
	
	virtual void findCentroid();
	
	virtual void findMomentOfInertia();
	
	friend class EdgeChain;
public:
	Vector2f m_Vertex1, m_Vertex2;
	Vector2f m_PrevVertex, m_NextVertex;
	duchar m_bHasPrev, m_bHasNext;
};

END_NAMESPACE_DIRAC2D

#endif