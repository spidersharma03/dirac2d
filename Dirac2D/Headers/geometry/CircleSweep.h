/*
 *  CircleSweep.h
 *  Dirac2D
 *
 *  Created by Prashant on 04/07/12.
 *
 */

#ifndef _CIRCLE_SWEEP_H_
#define _CIRCLE_SWEEP_H_

#include "CollisionShape.h"

BEGIN_NAMESPACE_DIRAC2D

class Edge;

class CircleSweep : public CollisionShape
{
public:
	
	virtual Vector2f getSupportPoint(const Vector2f& d);
	
	virtual Vector2f getSupportPointWithoutRadius(const Vector2f& d);

	virtual dbool isPointInside(Vector2f& p);
	
	virtual dbool intersectRaySegment(const Matrix3f& xForm, const RaySegment2f& raySeg, RayIntersectionInfo& intersectInfo);

	virtual void updateAABB(Matrix3f& xForm);
	
    friend class PhysicalBody;

protected:
	
    CircleSweep(dfloat radius, Vector2f* vertices, dint32 numVertices);
    
	CircleSweep(const CircleSweep& other);
	
	void operator= ( CircleSweep& other);
    
    virtual CollisionShape* clone(MemoryBlockAllocator* pAllocator);

	virtual void updateShape(Matrix3f& xForm);
	
	virtual void findCentroid();
	
	virtual void findMomentOfInertia();
	
public:
	dint32 m_NumEdges;
	dfloat m_Radius;
	Edge* m_SweepList;
	Vector2f m_PrevVertex, m_NextVertex;
	duchar m_bHasPrev, m_bHasNext;
};

END_NAMESPACE_DIRAC2D

#endif