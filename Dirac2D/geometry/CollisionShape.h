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
	CollisionShape(){}

	CollisionShape( const CollisionShape& other )
	{
		m_Centroid  = other.m_Centroid;
		m_Area	    = other.m_Area;
		m_ShapeType = other.m_ShapeType;
		m_AABB      = other.m_AABB;
	}
	
	dfloat getArea()
	{
		return m_Area;
	}
	
	// For GJK/EPA 
	// returns Local Support Point on the Shape.
	virtual Vector2f getSupportPoint(Vector2f& d) = 0;
	
	virtual dbool isPointInside(Point2f& p) = 0;
	
	virtual void updateAABB(Matrix3f& xForm) = 0;
	
	Vector2f m_Centroid;

protected:
	
	virtual void findCentroid() = 0;
	
protected:

	dfloat m_Area;
	SHAPE_TYPE m_ShapeType;
	AABB2f m_AABB;
};

END_NAMESPACE_DIRAC2D

#endif