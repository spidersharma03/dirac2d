/*
 *  Matrix2.h
 *  Dirac2D
 *
 *  Created by Prashant on 04/05/12.
 *
 */

#include "Vector2.h"

#ifndef _MATRIX2_H_
#define _MATRIX2_H_

BEGIN_NAMESPACE_DIRAC2D


template< class T >
class Matrix2
{
public:
	Matrix2()
	{
		a11 = 1; a12 = 0;
		a21 = 0; a22 = 1;
	}
	
	Matrix2(T a, T b, T c, T d)
	{
		a11 = a; a12 = c;
		a21 = b; a22 = d;
	}
	
	Matrix2(Vector2<T> c1, Vector2<T> c2 )
	{
		a11 = c1.x; a12 = c2.x;
		a21 = c1.y; a22 = c2.y;
	}
	
	inline Vector2<T> operator* ( Vector2<T>& v )
	{
		return Vector2<T>(a11*v.x + a12*v.y, a21*v.x + a22*v.y);
	}
	
	inline Matrix2<T> operator* ( Matrix2<T>& other )
	{
		return Matrix2<T>( a11 * other*a11 + a12 * other.a21,
						   a21 * other.a11 + a22 * other.a21,
						   a11 * other.a12 + a12 * other.a22,
						   a21 * other.a12 + a22 * other.a22 );
	}
	
	inline void operator*= ( Matrix2<T>& other )
	{
		dfloat a = a11 * other.a11 + a12 * other.a21;
		dfloat b = a21 * other.a11 + a22 * other.a21;
		dfloat c = a11 * other.a12 + a12 * other.a22;
		dfloat d = a21 * other.a12 + a22 * other.a22;
		
		a11 = a;
		a21 = b;
		a12 = c;
		a22 = d;
	}

	inline T determinant()
	{
		return a11 * a22 - a21 * a12;
	}
	
	inline T& operator[] ( dint32 index )
	{
		return (&a11)[index];
	}
	
public:	
	
	T a11, a21, a12, a22;
};

typedef Matrix2<dfloat> Matrix2f;
typedef Matrix2<dfloat> Matrix2d;

END_NAMESPACE_DIRAC2D

#endif