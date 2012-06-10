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
class RegularPolygon;
class Circle;
class Edge;

class Capsule : public CollisionShape
{
public:
	Capsule(dfloat radius, dfloat height);
	
	Capsule(const Capsule& other);
	
	void operator=( Capsule& other);
	
	virtual CollisionShape* clone();
	
	virtual Vector2f getSupportPoint(Vector2f& d);

	virtual dbool isPointInside(Vector2f& p);
	
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
	
protected:
	
	virtual void findCentroid();
	
	virtual void findMomentOfInertia();

	
	friend dbool intersectCapsules( Capsule* capsule1, Matrix3f& xform1, Capsule* capsule2, Matrix3f& xform2);
	friend dbool intersectCapsules( Capsule* capsule1, Matrix3f& xform1, Capsule* capsule2, Matrix3f& xform2, ContactManifold* contactManifold);
	friend dbool intersectCapsuleCircle( Circle* circle, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2);
	friend dbool intersectCapsuleCircle( Circle* circle, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2, ContactManifold* contactManifold);
	friend dbool intersectCapsulePolygon( RegularPolygon* poly, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2);
	friend dbool intersectCapsulePolygon( RegularPolygon* poly, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2, ContactManifold* contactManifold);
	friend dbool intersectEdgeCapsule( Edge* edge, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2);
	friend dbool intersectEdgeCapsule( Edge* edge, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2, ContactManifold* contactManifold);
	
protected:
	
	dfloat m_Radius;
	dfloat m_Height;
};

END_NAMESPACE_DIRAC2D

#endif