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
#include "../memory/memoryBlockAllocator.h"
#include <new>

#ifndef _COLLISION_SHAPE_H_
#define _COLLISION_SHAPE_H_

BEGIN_NAMESPACE_DIRAC2D

enum SHAPE_TYPE { EST_CIRCLE = 0, EST_BOX, EST_REGULARPOLY, EST_CAPSULE, EST_EDGE, EST_EDGE_CHAIN, EST_CIRCLE_SWEEP };

class BroadPhaseNode;

class CollisionShape
{
public:
	CollisionShape()
	{
		m_Area = 0.0f;
		m_I = 0.0f;
		m_pBroadPhaseNode = 0;
		m_Radius = 0.0f;
	}

	CollisionShape( const CollisionShape& other )
	{
		m_Centroid  = other.m_Centroid;
		m_Area	    = other.m_Area;
		m_ShapeType = other.m_ShapeType;
		m_AABB      = other.m_AABB;	
		m_Radius    = other.m_Radius;
	}
	
	void operator= ( CollisionShape& other)
	{
		m_Centroid  = other.m_Centroid;
		m_Area	    = other.m_Area;
		m_ShapeType = other.m_ShapeType;
		m_AABB      = other.m_AABB;
		m_Radius	= other.m_Radius;
	}
	
	inline dfloat getArea() const
	{
		return m_Area;
	}
	
	inline dfloat getMomentOfInertia() const
	{
		return m_I;
	}
	
	inline SHAPE_TYPE getShapeType() const
	{
		return m_ShapeType;
	}
	
	inline AABB2f& getAABB()
	{
		return m_AABB;
	}
    
	inline dfloat getRadius()
	{
		return m_Radius;
	}
    
	// For GJK/EPA 
	// returns Local Support Point on the Shape.
	virtual Vector2f getSupportPoint(const Vector2f& d) = 0;

	// Returns local support Point on the Shape, without considering the radius.
	virtual Vector2f getSupportPointWithoutRadius(const Vector2f& d) = 0;

	virtual dbool isPointInside(Vector2f& p) = 0;
	
	virtual void updateAABB(Matrix3f& xForm) = 0;
	
	virtual dbool intersectRaySegment(const Matrix3f& xForm, const RaySegment2f& raySeg, RayIntersectionInfo& intersectInfo) = 0;
	
	virtual dbool intersectRay(const Ray2f& ray)
	{
		return false;
	}
		
	virtual ~CollisionShape()
	{
		m_pBroadPhaseNode = 0;
	};
	
	
	Vector2f m_Centroid;

	// pointer to the BroadPhase Node
	BroadPhaseNode* m_pBroadPhaseNode;

	// friend classes
	friend class PhysicalShape;
	friend class PhysicalBody;
	
protected:
	
    virtual CollisionShape* clone(MemoryBlockAllocator* pAllocator) = 0;

	virtual void updateShape(Matrix3f& xForm){};
	
	virtual void findCentroid() = 0;
	
	virtual void findMomentOfInertia() = 0;
	
	protected:
	
	dfloat m_Area;
	dfloat m_I;
	SHAPE_TYPE m_ShapeType;
	AABB2f m_AABB;
	dfloat m_Radius;
};

END_NAMESPACE_DIRAC2D

#endif