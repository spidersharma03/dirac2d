/*
 *  MathUtil.h
 *  Dirac2D
 *
 *  Created by Prashant on 23/04/12.
 *
 */

#include "../definitions.h"

#ifndef _MATH_UTIL_H_
#define _MATH_UTIL_H_

BEGIN_NAMESPACE_DIRAC2D

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
	
	T EuclidiandDistanceSquared( Point2<T>& other )
	{
		return (x-other.x)*(x-other.x) + (y-other.y)*(y-other.y);
	}
	
	T ManhattendDistance( Point2<T>& other )
	{
		return fabs(x-other.x) + fabs(y-other.y);
	}
	
public:
	T x,y;
};


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
		return Vector2<T>(-d*y, d*x);
	}
	
	inline T length()
	{
		return sqrt(x*x + y*y);
	}
	
	inline T lengthSquared()
	{
		return (x*x + y*y);
	}
	
	void normalize()
	{
		T len = length();
		if( len > EPSILON )
		{
			len = 1.0f/len;
			x *= len;
			y *= len;
		}
	}
	
	void add( Vector2<T>& other )
	{
		x += other.x;
		y += other.y;
	}
	
	void sub( Vector2<T>& other )
	{
		x -= other.x;
		y -= other.y;
	}
	
	Vector2<T> operator-()
	{
		return Vector2<T>(-x, -y);
	}

	Vector2<T> operator+( const Vector2<T>& other )
	{
		return Vector2<T>(x+other.x, y+other.y);
	}
	
	Vector2<T> operator-( const Vector2<T>& other )
	{
		return Vector2<T>(x-other.x, y-other.y);
	}
	
	void operator/=( const T d )
	{
		x /= d;
		y /= d;
	}
	
	void operator/=( const Vector2<T>& other )
	{
		x /= other.x;
		y /= other.y;
	}
	
	void operator*=( const Vector2<T>& other )
	{
		x *= other.x;
		y *= other.y;
	}
	
	void operator*=( const T d )
	{
		x *= d;
		y *= d;
	}
	
	void operator+=( const Vector2<T>& other )
	{
		x += other.x;
		y += other.y;
	}
	
	void operator-=( const Vector2<T>& other )
	{
		x -= other.x;
		y -= other.y;
	}
	
	bool operator==( const Vector2<T>& other )
	{
		return (x==other.x) && (y==other.y);
	}
	
public:
	T x, y;
};

template< class T >
class Vector3
{
public:
	Vector3():x(0),y(0),z(0)
	{
	}
	
	Vector3( T x, T y, T z):x(x),y(y),z(z)
	{
	}
	
	Vector3( const Vector3<T>& other):x(other.x),y(other.y),z(other.z)
	{
	}
	
	inline T dot( Vector3<T>& other)
	{
		return x*other.x + y*other.y + z*other.z;
	}
	
	inline T length()
	{
		return sqrt(x*x + y*y + z*z);
	}
	
	inline T lengthSquared()
	{
		return (x*x + y*y + z*z);
	}
	
	void normalize()
	{
		T len = length();
		if( len > EPSILON )
		{
			len = 1.0f/len;
			x *= len;
			y *= len;
			z *= len;
		}
	}
	
	void add( Vector3<T>& other )
	{
		x += other.x;
		y += other.y;
		z += other.z;
	}
	
	void sub( Vector3<T>& other )
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}
	
	Vector3 operator-()
	{
		return Vector3<T>(-x, -y, -z);
	}
	
	Vector3 operator+( const Vector3& other )
	{
		return Vector3<T>(x+other.x, y+other.y, z+other.z);
	}
	
	Vector3 operator-( const Vector3& other )
	{
		return Vector3<T>(x-other.x, y-other.y, z-other.z);
	}
	
	void operator/=( const T d )
	{
		x /= d;
		y /= d;
		z /= d;
	}
	
	void operator/=( const Vector3& other )
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
	}
	
	void operator*=( const Vector3& other )
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
	}
	
	void operator*=( const T d )
	{
		x *= d;
		y *= d;
		z *= d;
	}
	
	void operator+=( const Vector3& other )
	{
		x += other.x;
		y += other.y;
		z += other.z;
	}
	
	void operator-=( const Vector3& other )
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}
	
	bool operator==( const Vector3& other )
	{
		return (x==other.x) && (y==other.y) && (z==other.z);
	}
	
public:
	T x, y, z;
};

template< class T >
class Matrix2
{
public:
	Matrix2()
	{
		col1.x = 1; col1.y = 0;
		col2.x = 0; col2.y = 1;
	}
	
	Matrix2(Vector2<T> c1, Vector2<T> c2 ):col1(c1), col2(c2)
	{
	}
	
	T determinant()
	{
		return col1.x*col2.y - col1.y*col2.x;
	}
	
public:
	Vector2<T> col1, col2;
};


template< class T >
class Matrix3
{
public:
	Matrix3()
	{
	}
	
	Matrix3(const Matrix3<T>& other)
	{
	}
	
	Vector2<T> operator*( const Vector2<T>& v )
	{
		return Vector2<T>( col1.x*v.x + col2.x*v.y,  col1.y*v.x + col2.y*v.y);
	}
	
	Point2<T> operator*( const Point2<T>& p )
	{
		return Vector2<T>( col1.x*p.x + col2.x*p.y + col3.x,  col1.y*p.x + col2.y*p.y + col3.y);
	}
	
	Matrix3<T> getInverse()
	{
	}
	
public:
	Vector3<T> col1, col2, col3;
};

typedef Vector2<dfloat> Vector2f;
typedef Vector2<ddouble> Vector2d;

typedef Point2<dfloat> Point2f;
typedef Point2<ddouble> Point2d;

typedef Vector3<dfloat> Vector3f;
typedef Vector3<ddouble> Vector3d;

typedef Matrix2<dfloat> Matrix2f;
typedef Matrix2<dfloat> Matrix2d;

typedef Matrix3<dfloat> Matrix3f;
typedef Matrix3<dfloat> Matrix3d;

END_NAMESPACE_DIRAC2D

#endif