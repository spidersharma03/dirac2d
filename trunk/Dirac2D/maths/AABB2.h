/*
 *  AABB2.h
 *  Dirac2D
 *
 *  Created by Prashant on 14/05/12.
 *
 */

#include "../definitions.h"
#include "Vector2.h"

#ifndef _AABB2_H_
#define _AABB2_H_

BEGIN_NAMESPACE_DIRAC2D

template< class T >
class AABB2
{
public:
	AABB2()
	{
	}
	
	AABB2(const AABB2<T>& other)
	{
	}
	
	AABB2(Vector2<T>& lower, Vector2<T>& upper)
	{
	}
	
	AABB2(T lowerX, T lowerY, T upperX, T upperY)
	{
	}
	
	inline dbool intersectAABB( AABB2<T>& other )
	{
		dfloat overLapX = m_LowerBounds.x < other.m_LowerBounds.x ? other.m_LowerBounds.x - m_UpperBounds.x : m_LowerBounds.x - other.m_UpperBounds.x;
		dfloat overLapY = m_LowerBounds.y < other.m_LowerBounds.y ? other.m_LowerBounds.y - m_UpperBounds.y : m_LowerBounds.y - other.m_UpperBounds.y;	
		return ( (overLapX < 0.0f) && (overLapY < 0.0f) );
	}
	
public:
	Vector2<T> m_LowerBounds, m_UpperBounds;
};

END_NAMESPACE_DIRAC2D

#endif