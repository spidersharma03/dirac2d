/*
 *  Shape.h
 *  Dirac2D
 *
 *  Created by Prashant on 24/04/12.
 *
 */

#include "../definitions.h"
#include "../Settings.h"
#include "../maths/MathUtil.h"

#ifndef _COLLISION_SHAPE_H_
#define _COLLISION_SHAPE_H_

BEGIN_NAMESPACE_DIRAC2D

class CollisionShape
{
	enum SHAPE_TYPE { EST_CIRCLE = 0, EST_BOX, EST_REGULARPOLY, EST_CAPSULE };
public:
	CollisionShape();
	
	CollisionShape(const CollisionShape& other);
	
	virtual dfloat getArea();
	
	// For GJK/EPA 
	virtual Vector2f getSupportPoint(Vector2f& d);
	
	virtual dbool isPointInside(Point2f& p);
	
	virtual void updateAABB(Matrix3f& xForm);
	
	Vector2f m_Centroid;
	
protected:
	
	virtual void findCentroid();
	
protected:
	
	dfloat m_Area;
	SHAPE_TYPE m_ShapeType;
	AABB2f m_AABB;
};

END_NAMESPACE_DIRAC2D

#endif