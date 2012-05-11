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

enum SHAPE_TYPE { EST_CIRCLE = 0, EST_BOX, EST_REGULARPOLY, EST_CAPSULE };

class CollisionShape
{
public:
	CollisionShape()
	{
	}

	CollisionShape( const CollisionShape& other )
	{
		m_Centroid  = other.m_Centroid;
		m_Area	    = other.m_Area;
		m_ShapeType = other.m_ShapeType;
		m_AABB      = other.m_AABB;		
	}
	
	dfloat getArea() const
	{
		return m_Area;
	}
	
	dfloat getMomentOfInertia() const
	{
		return m_I;
	}
	
	inline SHAPE_TYPE getShapeType()
	{
		return m_ShapeType;
	}
	
	// For GJK/EPA 
	// returns Local Support Point on the Shape.
	virtual Vector2f getSupportPoint(Vector2f& d) = 0;
	
	virtual dbool isPointInside(Point2f& p) = 0;
	
	virtual void updateAABB(Matrix3f& xForm) = 0;
	
	Vector2f m_Centroid;

	// friend classes
	friend class PhysicalShape;
protected:
	
	virtual void findCentroid() = 0;
	
	virtual void findMomentOfInertia() = 0;
	
	protected:

	dfloat m_Area;
	dfloat m_I;
	SHAPE_TYPE m_ShapeType;
	AABB2f m_AABB;
};

END_NAMESPACE_DIRAC2D

#endif