/*
 *  Box.h
 *  Dirac2D
 *
 *  Created by Prashant on 25/04/12.
 *
 */
#include "CollisionShape.h"


#ifndef _BOX_H_
#define _BOX_H_


BEGIN_NAMESPACE_DIRAC2D

class Box : public CollisionShape
{
public:
	// For GJK/EPA 
	virtual Vector2f getSupportPoint(const Vector2f& d);
	
	virtual dbool isPointInside(Vector2f& p);
	
	virtual dbool intersectRaySegment(const Matrix3f& xForm, const RaySegment2f& raySeg, RayIntersectionInfo& intersectInfo);

	virtual void updateAABB(Matrix3f& xForm);
	
    friend class PhysicalBody;

protected:
	
    Box(dfloat width, dfloat height);
	
	Box(const Box& other);
	
    virtual CollisionShape* clone(MemoryBlockAllocator* pAllocator);

	virtual void findCentroid();
	
	virtual void findMomentOfInertia();
	
protected:
	dfloat m_Width, m_Height;
};

END_NAMESPACE_DIRAC2D

#endif