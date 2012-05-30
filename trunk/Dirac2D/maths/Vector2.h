/*
 *  Vector2.h
 *  Dirac2D
 *
 *  Created by Prashant on 04/05/12.
 *
 */
#include "../definitions.h"

#ifndef _VECTOR2_H_
#define _VECTOR2_H_

BEGIN_NAMESPACE_DIRAC2D


template< class T > class Matrix2;
template< class T > class Matrix3;

template< class T >
class Vector2
{
public:
	Vector2():x(0),y(0)
	{
	}
	
	Vector2( T x, T y):x(x),y(y)
	{
	}
	
	Vector2( const Vector2<T>& other):x(other.x),y(other.y)
	{
	}
	
	inline void operator=(Vector2<T> other)
	{
		x = other.x;
		y = other.y;
	}

	inline T dot( Vector2<T>& other)
	{
		return x*other.x + y*other.y;
	}

	inline T cross( Vector2<T>& other )
	{
		return x*other.y - y*other.x;
	}
	
	inline Vector2<T> cross( T d )
	{
		return Vector2<T>(d*y, -d*x);
	}
	
	static inline Vector2<T> cross( Vector2<T>& v, T d )
	{
		return Vector2<T>(d * v.y, -d * v.x);
	}
	
	static inline Vector2<T> cross( T d, Vector2<T>& v )
	{
		return Vector2<T>(-d * v.y, d * v.x);
	}
	
	static inline T cross( Vector2<T>& v1, Vector2<T>& v2 )
	{
		return  v1.x * v2.y - v1.y * v2.x;
	}
	
	inline T distanceSquared( Vector2<T>& other)
	{
		T dx = x-other.x;
		T dy = y-other.y;
		return  dx*dx + dy*dy;
	}
	
	inline T distance( Vector2<T>& other)
	{
		T dx = x-other.x;
		T dy = y-other.y;
		return  sqrt(dx*dx + dy*dy);
	}
	
	static inline T distanceSquared( Vector2<T>& v1, Vector2<T>& v2)
	{
		T dx = v1.x-v2.x;
		T dy = v1.y-v2.y;
		return  dx*dx + dy*dy;
	}
	
	static inline T distance( Vector2<T>& v1, Vector2<T>& v2)
	{
		T dx = v1.x-v2.x;
		T dy = v1.y-v2.y;
		return  sqrt(dx*dx + dy*dy);
	}
	
	inline T length()
	{
		return sqrt(x*x + y*y);
	}
	
	inline T lengthSquared()
	{
		return (x*x + y*y);
	}
	
	inline void normalize()
	{
		T len = length();
		if( len > EPSILON )
		{
			len = 1.0f/len;
			x *= len;
			y *= len;
		}
	}
	
	inline void set( T inx, T iny)
	{
		x = inx; y = iny;
	}
	
	inline void set( Vector2<T>& other )
	{
		x = other.x; y = other.y;
	}
	
	inline void add( Vector2<T>& other )
	{
		x += other.x;
		y += other.y;
	}
	
	inline void sub( Vector2<T>& other )
	{
		x -= other.x;
		y -= other.y;
	}
	
	inline Vector2<T> operator-()
	{
		return Vector2<T>(-x, -y);
	}
	
	inline Vector2<T> operator+( const Vector2<T>& other )
	{
		return Vector2<T>(x+other.x, y+other.y);
	}
	
	inline Vector2<T> operator-( const Vector2<T>& other )
	{
		return Vector2<T>(x-other.x, y-other.y);
	}
	
	inline Vector2<T> operator*( const Vector2<T>& other )
	{
		return Vector2<T>(x*other.x, y*other.y);
	}
	
	// Right Multiply By a 2x2 Matrix( this is same as out = AT * v )
	inline Vector2<T> operator*( const Matrix2<T>& matrix )
	{
		return Vector2<T>(matrix.a11*x + matrix.a21*y, matrix.a12*x + matrix.a22*y);
	}
	
	// Right Multiply By a 3x3 Matrix( this is same as out = AInv * v )
	inline Vector2<T> operator*( const Matrix3<T>& matrix )
	{
		dfloat tx  = -matrix.col1.x * matrix.col3.x - matrix.col1.y * matrix.col3.y;
		dfloat ty  = -matrix.col2.x * matrix.col3.x - matrix.col2.y * matrix.col3.y;
		
		return Vector2<T>(matrix.col1.x * x + matrix.col1.y * y + tx , matrix.col2.x * x + matrix.col2.y * y + ty );
	}
	
	// Right Multiply By a 2x2 Matrix( this is same as this = AT * v )
	inline void operator*=( const Matrix2<T>& matrix )
	{
		dfloat tx = matrix.a11 * x + matrix.a21 * y;
		y = matrix.a12 * x + matrix.a22 * y;
		x = tx;
	}
	
	// Right Multiply By a 3x3 Matrix( this is same as this = AInv * v )
	inline void operator*=( const Matrix3<T>& matrix )
	{
		dfloat tx  = -matrix.col1.x * matrix.col3.x - matrix.col1.y * matrix.col3.y;
		dfloat ty  = -matrix.col2.x * matrix.col3.x - matrix.col2.y * matrix.col3.y;
		dfloat temp = matrix.col1.x * x + matrix.col1.y * y + tx;
		y = matrix.col2.x * x + matrix.col2.y * y + ty;
		x = temp;
	}
	
	inline Vector2<T> operator*( const T d )
	{
		return Vector2<T>(x*d, y*d);
	}
	
	inline Vector2<T> operator/( const T d )
	{
		return Vector2<T>(x/d, y/d);
	}
	
	inline void operator/=( const T d )
	{
		x /= d;
		y /= d;
	}
	
	inline Vector2<T> operator/( const Vector2<T>& other )
	{
		return Vector2<T>(x/other.x, y/other.y);
	}
	
	inline void operator/=( const Vector2<T>& other )
	{
		x /= other.x;
		y /= other.y;
	}
	
	inline void operator*=( const Vector2<T>& other )
	{
		x *= other.x;
		y *= other.y;
	}
	
	inline void operator*=( const T d )
	{
		x *= d;
		y *= d;
	}
	
	inline void operator+=( const Vector2<T>& other )
	{
		x += other.x;
		y += other.y;
	}
	
	inline void operator-=( const Vector2<T>& other )
	{
		x -= other.x;
		y -= other.y;
	}
	
	inline bool operator==( const Vector2<T>& other )
	{
		return (x==other.x) && (y==other.y);
	}
public:
	T x, y;
};

typedef Vector2<dfloat> Vector2f;
typedef Vector2<ddouble> Vector2d;

END_NAMESPACE_DIRAC2D

#endif