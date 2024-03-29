/*
 *  Circle.h
 *  Dirac2D
 *
 *  Created by Prashant on 24/04/12.
 *
 */

#ifndef _CIRCLE_H_
#define _CIRCLE_H_

#include "CollisionShape.h"

BEGIN_NAMESPACE_DIRAC2D


struct ContactManifold;
class ConvexPolygon;
class Capsule;
class Edge;

class Circle : public CollisionShape
{
public:
	virtual Vector2f getSupportPoint(const Vector2f& d);

	virtual Vector2f getSupportPointWithoutRadius(const Vector2f& d);

	virtual dbool isPointInside(Vector2f& p);
	
	virtual dbool intersectRaySegment(const Matrix3f& xForm, const RaySegment2f& raySeg, RayIntersectionInfo& intersectInfo);

	virtual void updateAABB(Matrix3f& xForm);
	
	inline dfloat getRadius() const
	{
		return m_Radius;
	}
	
	virtual ~Circle();
	
    friend class PhysicalBody;
private:
    
    Circle(dfloat radius);
	
	Circle(const Circle& other);
    
	void operator= ( Circle& other);
    
    virtual CollisionShape* clone(MemoryBlockAllocator* pAllocator);

protected:
	
	virtual void updateShape(Matrix3f& xForm);

	virtual void findCentroid();
	
	virtual void findMomentOfInertia();

protected:
	
	friend dbool intersectCircles( Circle* circle1, Matrix3f& xform1, Circle* circle2, Matrix3f& xform2);
	friend dbool intersectCircles( Circle* circle1, Matrix3f& xform1, Circle* circle2, Matrix3f& xform2, ContactManifold* contactManifold);
	friend dbool intersectCirclePolygon( Circle* circle, Matrix3f& xform1, ConvexPolygon* poly, Matrix3f& xform2);
	friend dbool intersectCirclePolygon( Circle* circle, Matrix3f& xform1, ConvexPolygon* poly, Matrix3f& xform2, ContactManifold* contactManifold);
	
	friend dbool intersectCapsuleCircle( Circle* circle, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2);
	friend dbool intersectCapsuleCircle( Circle* circle, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2, ContactManifold* contactManifold);
	
	friend dbool intersectEdgeCircle( Edge* edge, Matrix3f& xform1, Circle* circle, Matrix3f& xform2);
	friend dbool intersectEdgeCircle( Edge* edge, Matrix3f& xform1, Circle* circle, Matrix3f& xform2, ContactManifold* contactManifold);
	
};

END_NAMESPACE_DIRAC2D

#endif