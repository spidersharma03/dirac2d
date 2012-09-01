/*
 *  Ray2.h
 *  Dirac2D
 *
 *  Created by Prashant on 18/06/12.
 *
 */

#include "../definitions.h"
#include "Vector2.h"
#include "Matrix2.h"
#include "Matrix3.h"
#include "AABB2.h" 

#ifndef _RAY2_H_
#define _RAY2_H_

BEGIN_NAMESPACE_DIRAC2D

template< class T >
class Ray2
{
public:
	Ray2()
	{
	}
	
	Ray2(const Ray2<T>& other)
	{
		m_Origin = other.m_Origin;
		m_Direction = other.m_Direction;
	}
	
	Ray2(Vector2<T>& origin, Vector2<T>& direction)
	{
		m_Origin = origin;
		m_Direction = direction;
	}
	
	inline void set(Ray2<T>& other)
	{
		m_Origin = other.m_Origin;
		m_Direction = other.m_Direction;
	}

	inline void set(Vector2<T>& origin, Vector2<T>& direction)
	{
		m_Origin = origin;
		m_Direction = direction;
	}
	
	dbool intersectAABB(AABB2<T>& aabb)
	{
		dbool bInside = true;
		Vector2<T> MinB = aabb.m_LowerBounds;
		Vector2<T> MaxB = aabb.m_UpperBounds;
		MaxT.x = MaxT.y = -1.0f;
			
		// X DIRECTION
		// Find candidate planes.
		if(m_Origin.x < MinB.x)
		{
			bInside		= false;
			// Calculate T distances to candidate planes
			if(m_Direction.x)	MaxT.x = (MinB.x - m_Origin.x) / m_Direction.x;
		}
		else if(m_Origin.x > MaxB.x)
		{
			bInside		= false;
			// Calculate T distances to candidate planes
			if(m_Direction.x)	MaxT.x = (MaxB.x - m_Origin.x) / m_Direction.x;
		}
			
		// Y DIRECTION
		if(m_Origin.y < MinB.y)
		{
			bInside		= false;
			// Calculate T distances to candidate planes
			if(m_Direction.y)	MaxT.y = (MinB.y - m_Origin.y) / m_Direction.y;
		}
		else if(m_Origin.y > MaxB.y)
		{
			bInside		= false;
			// Calculate T distances to candidate planes
			if(m_Direction.y)	MaxT.y = (MaxB.y - m_Origin.y) / m_Direction.y;
		}	
		// Ray origin inside bounding box
		if(bInside)
		{
			return true;
		}	
		// Get largest of the maxT's for final choice of intersection
		dfloat Largest = MaxT.x;
		if(MaxT.y > Largest)	Largest = MaxT.y;

		dfloat outx = m_Origin.x + Largest * m_Direction.x;
		if( outx < MinB.x - RAYAABB_EPSILON || outx > MaxB.x + RAYAABB_EPSILON )return false;
			
		dfloat outy = m_Origin.y + Largest * m_Direction.y;
		if( outy < MinB.y - RAYAABB_EPSILON || outy > MaxB.y + RAYAABB_EPSILON )return false;
			
		return true;	// ray hits box
	}
	
public:
	Vector2<T> m_Origin;
	Vector2<T> m_Direction;
};


template< class T >
class RaySegment
{
public:
	RaySegment()
	{
	}
	
	RaySegment(const RaySegment<T>& other)
	{
		m_Start = other.m_Start;
		m_End = other.m_End;
	}
	
	RaySegment(Vector2<T>& start, Vector2<T>& end)
	{
		m_Start = start;
		m_End = end;
	}
	
	inline void set(Ray2<T>& other)
	{
		m_Start = other.m_Start;
		m_End = other.m_End;
	}
	
	inline void set(Vector2<T>& start, Vector2<T>& end)
	{
		m_Start = start;
		m_End = end;
	}
	
	dbool intersectAABB(AABB2<T>& aabb)
	{
		return 0;
	}

public:
	Vector2<T> m_Start;
	Vector2<T> m_End;
};

typedef Ray2<dfloat> Ray2f;
typedef Ray2<ddouble> Ray2d;

typedef RaySegment<dfloat> RaySegment2f;
typedef RaySegment<ddouble> RaySegment2d;

END_NAMESPACE_DIRAC2D

#endif