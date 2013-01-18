/*
 *  Capsule.h
 *  Dirac2D
 *
 *  Created by Prashant on 24/04/12.
 *
 */

#ifndef _CAPSULE_H_
#define _CAPSULE_H_

#include "CollisionShape.h"

BEGIN_NAMESPACE_DIRAC2D

struct ContactManifold;
class ConvexPolygon;
class Circle;
class Edge;

class Capsule : public CollisionShape
{
public:

	virtual Vector2f getSupportPoint(const Vector2f& d);

	virtual Vector2f getSupportPointWithoutRadius(const Vector2f& d);

	virtual dbool isPointInside(Vector2f& p);
	
	virtual dbool intersectRaySegment(const Matrix3f& xForm, const RaySegment2f& raySeg, RayIntersectionInfo& intersectInfo);

	virtual void updateAABB(Matrix3f& xForm);
	
	virtual void updateShape(Matrix3f& xForm);

	inline dfloat getRadius() const
	{
		return m_Radius;
	}
	
	inline dfloat getHeight() const
	{
		return m_Height;
	}
	
    friend class PhysicalBody;

protected:
	
    Capsule(dfloat radius, dfloat height);
	
	Capsule(const Capsule& other);
	
	void operator=( Capsule& other);
	
	virtual CollisionShape* clone(MemoryBlockAllocator* pAllocator);
	
	virtual void findCentroid();
	
	virtual void findMomentOfInertia();

	
	friend dbool intersectCapsules( Capsule* capsule1, Matrix3f& xform1, Capsule* capsule2, Matrix3f& xform2);
	friend dbool intersectCapsules( Capsule* capsule1, Matrix3f& xform1, Capsule* capsule2, Matrix3f& xform2, ContactManifold* contactManifold);
	friend dbool intersectCapsuleCircle( Circle* circle, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2);
	friend dbool intersectCapsuleCircle( Circle* circle, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2, ContactManifold* contactManifold);
	friend dbool intersectCapsulePolygon( ConvexPolygon* poly, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2);
	friend dbool intersectCapsulePolygon( ConvexPolygon* poly, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2, ContactManifold* contactManifold);
	friend dbool intersectEdgeCapsule( Edge* edge, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2);
	friend dbool intersectEdgeCapsule( Edge* edge, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2, ContactManifold* contactManifold);
	
protected:
	
	dfloat m_Height;
};

END_NAMESPACE_DIRAC2D

#endif