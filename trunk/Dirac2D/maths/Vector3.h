/*
 *  Vector3.h
 *  Dirac2D
 *
 *  Created by Prashant on 04/05/12.
 *
 */
#include "../definitions.h"

#ifndef _VECTOR3_H_
#define _VECTOR3_H_

BEGIN_NAMESPACE_DIRAC2D

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

typedef Vector3<dfloat> Vector3f;
typedef Vector3<ddouble> Vector3d;

END_NAMESPACE_DIRAC2D

#endif
