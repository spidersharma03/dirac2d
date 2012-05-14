/*
 *  MathUtil.h
 *  Dirac2D
 *
 *  Created by Prashant on 23/04/12.
 *
 */

#include "../definitions.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Matrix2.h"
#include "Matrix3.h"
#include "AABB2.h"

#ifndef _MATH_UTIL_H_
#define _MATH_UTIL_H_

BEGIN_NAMESPACE_DIRAC2D

template< typename T >
inline T MIN( T a, T b )
{
	T min = a < b ? a : b;
	return min;
}

template< typename T >
inline T MAX( T a, T b )
{
	T max = a > b ? a : b;
	return max;
}

template< typename T >
inline void SWAP( T a, T b )
{
	T temp = a;
	a = b;
	b = temp;
}

template< typename T >
inline T CLAMP( T a, T lo, T hi )
{
	if( a < lo )
	{
		a = lo;
		return a;
	}
	if( a > hi )
		a = hi;
	return a;
}

template< class T >
class Point2
{
public:
	Point2():x(0),y(0)
	{
	}
	
	Point2( T x, T y):x(x),y(y)
	{
	}
	
	Point2( const Point2<T>& other):x(other.x),y(other.y)
	{
	}
	
	Point2<T> operator-()
	{
		return Point2<T>(-x, -y);
	}
	
	Point2<T> operator+( const Point2<T>& other )
	{
		return Point2<T>(x+other.x, y+other.y);
	}
	
	Point2<T> operator-( const Point2<T>& other )
	{
		return Point2<T>(x-other.x, y-other.y);
	}
	
	inline void set( T inx, T iny)
	{
		x = inx; y = iny;
	}
	
	void operator/=( const T d )
	{
		x /= d;
		y /= d;
	}
	
	void operator/=( const Point2<T>& other )
	{
		x /= other.x;
		y /= other.y;
	}
	
	void operator*=( const Point2<T>& other )
	{
		x *= other.x;
		y *= other.y;
	}
	
	void operator*=( const T d )
	{
		x *= d;
		y *= d;
	}
	
	void operator+=( const Point2<T>& other )
	{
		x += other.x;
		y += other.y;
	}
	
	void operator-=( const Point2<T>& other )
	{
		x -= other.x;
		y -= other.y;
	}
	
	bool operator==( const Point2<T>& other )
	{
		return (x==other.x) && (y==other.y);
	}
	
	inline T EuclidiandDistance( Point2<T>& other )
	{
		return sqrt( (x-other.x)*(x-other.x) + (y-other.y)*(y-other.y) );
	}
	
	inline T EuclidiandDistanceSquared( Point2<T>& other )
	{
		return (x-other.x)*(x-other.x) + (y-other.y)*(y-other.y);
	}
	
	inline T ManhattendDistance( Point2<T>& other )
	{
		return fabs(x-other.x) + fabs(y-other.y);
	}
	
public:
	T x,y;
};



typedef AABB2<dfloat> AABB2f;
typedef AABB2<ddouble> AABB2d;

typedef Point2<dfloat> Point2f;
typedef Point2<ddouble> Point2d;

END_NAMESPACE_DIRAC2D

#endif