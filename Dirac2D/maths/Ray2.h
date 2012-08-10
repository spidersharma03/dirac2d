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