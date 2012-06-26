/*
 *  Matrix3.h
 *  Dirac2D
 *
 *  Created by Prashant on 04/05/12.
 *
 */

#include "Vector2.h"
#include "Vector3.h"
#include <math.h>

#ifndef _MATRIX3_H_
#define _MATRIX3_H_

BEGIN_NAMESPACE_DIRAC2D

template< class T >
class Matrix3
{
public:
	Matrix3()
	{
		setIdentity();
	}
	
	Matrix3(const Matrix3<T>& other)
	{
		col1 = other.col1;
		col2 = other.col2;
		col3 = other.col3;
	}
	
	inline void rotate(T angle)
	{
		T cs = cos(angle); T sn = sin(angle);
		col1.x = cs; col2.x = -sn;
		col1.y = sn; col2.y = cs;
	}
	
	inline void translate( Vector2<T>& translation )
	{
		col3.x = translation.x;
		col3.y = translation.y;
	}
	
	inline void translate( T tx, T ty )
	{
		col3.x = tx;
		col3.y = ty;
	}
	
	inline Vector2<T> operator*( const Vector2<T>& v )
	{
		return Vector2<T>( col1.x*v.x + col2.x*v.y + col3.x,  col1.y*v.x + col2.y*v.y + col3.y);
	}
	
	inline void transformAsPoint(Vector2<T>& v)
	{
		dfloat x = col1.x*v.x + col2.x*v.y + col3.x;
		dfloat y = col1.y*v.x + col2.y*v.y + col3.y;
		v.x = x;
		v.y = y;
	}
	
	inline void transformAsVector(Vector2<T>& v)
	{
		dfloat x = col1.x*v.x + col2.x*v.y;
		dfloat y = col1.y*v.x + col2.y*v.y;
		v.x = x;
		v.y = y;
	}
	
	inline Matrix2<T> getRotationMatrix()
	{
		return Matrix2<T>(col1.x, col1.y, col2.x, col2.y);
	}
	
	inline Matrix2<T> getRotationMatrixTransposed()
	{
		return Matrix2<T>(col1.x, col2.x, col1.y, col2.y);
	}
	
	inline void setIdentity()
	{
		col1.x = 1.0f; col2.x = 0.0f; col3.x = 0.0f;
		col1.y = 0.0f; col2.y = 1.0f; col3.y = 0.0f;
		col1.z = 0.0f; col2.z = 0.0f; col3.z = 1.0f;
	}
	
	void getPointer( T* data )
	{
		data[0] = col1.x; data[1] = col1.y; data[2] = 0.0;    data[3] = 0.0;
		data[4] = col2.x; data[5] = col2.y; data[6] = 0.0;    data[7] = 0.0;
		data[8] = 0.0;    data[9] = 0.0;    data[10] = 1.0;    data[11] = 0.0;
		data[12] = col3.x; data[13] = col3.y; data[14] = 0.0;    data[15] = 1.0;
	}
	
	Matrix3<T> operator*( Matrix3<T>& other )
	{
		Matrix3<T> outMatrix;
		outMatrix.col1.x = col1.x * other.col1.x + col2.x * other.col1.y + col3.x * other.col1.z;
		outMatrix.col2.x = col1.x * other.col2.x + col2.x * other.col2.y + col3.x * other.col2.z;
		outMatrix.col3.x = col1.x * other.col3.x + col2.x * other.col3.y + col3.x * other.col3.z;
		
		outMatrix.col1.y = col1.y * other.col1.x + col2.y * other.col1.y + col3.y * other.col1.z;
		outMatrix.col2.y = col1.y * other.col2.x + col2.y * other.col2.y + col3.y * other.col2.z;
		outMatrix.col3.y = col1.y * other.col3.x + col2.y * other.col3.y + col3.y * other.col3.z;
		
		outMatrix.col1.z = col1.z * other.col1.x + col2.z * other.col1.y + col3.z * other.col1.z;
		outMatrix.col2.z = col1.z * other.col2.x + col2.z * other.col2.y + col3.z * other.col2.z;
		outMatrix.col3.z = col1.z * other.col3.x + col2.z * other.col3.y + col3.z * other.col3.z;
		
		return outMatrix;
	}
	
	void operator*=( Matrix3<T>& other )
	{
		Matrix3<T> outMatrix;
		outMatrix.col1.x = col1.x * other.col1.x + col2.x * other.col1.y + col3.x * other.col1.z;
		outMatrix.col2.x = col1.x * other.col2.x + col2.x * other.col2.y + col3.x * other.col2.z;
		outMatrix.col3.x = col1.x * other.col3.x + col2.x * other.col3.y + col3.x * other.col3.z;
		
		outMatrix.col1.y = col1.y * other.col1.x + col2.y * other.col1.y + col3.y * other.col1.z;
		outMatrix.col2.y = col1.y * other.col2.x + col2.y * other.col2.y + col3.y * other.col2.z;
		outMatrix.col3.y = col1.y * other.col3.x + col2.y * other.col3.y + col3.y * other.col3.z;
		
		outMatrix.col1.z = col1.z * other.col1.x + col2.z * other.col1.y + col3.z * other.col1.z;
		outMatrix.col2.z = col1.z * other.col2.x + col2.z * other.col2.y + col3.z * other.col2.z;
		outMatrix.col3.z = col1.z * other.col3.x + col2.z * other.col3.y + col3.z * other.col3.z;

		*this = outMatrix;
	}
	
	void multiply( Matrix3<T>& m1, Matrix3<T>& m2 )
	{
		col1.x = m1.col1.x * m2.col1.x + m1.col2.x * m2.col1.y + m1.col3.x * m2.col1.z;
		col2.x = m1.col1.x * m2.col2.x + m1.col2.x * m2.col2.y + m1.col3.x * m2.col2.z;
		col3.x = m1.col1.x * m2.col3.x + m1.col2.x * m2.col3.y + m1.col3.x * m2.col3.z;
		
		col1.y = m1.col1.y * m2.col1.x + m1.col2.y * m2.col1.y + m1.col3.y * m2.col1.z;
		col2.y = m1.col1.y * m2.col2.x + m1.col2.y * m2.col2.y + m1.col3.y * m2.col2.z;
		col3.y = m1.col1.y * m2.col3.x + m1.col2.y * m2.col3.y + m1.col3.y * m2.col3.z;
		
		col1.z = m1.col1.z * m2.col1.x + m1.col2.z * m2.col1.y + m1.col3.z * m2.col1.z;
		col2.z = m1.col1.z * m2.col2.x + m1.col2.z * m2.col2.y + m1.col3.z * m2.col2.z;
		col3.z = m1.col1.z * m2.col3.x + m1.col2.z * m2.col3.y + m1.col3.z * m2.col3.z;
	}
	
	inline void invert()
	{
	}
	
	inline void getInverse(Matrix3<T>& other)
	{
	}
		
	inline Matrix3<T> getInverse()
	{
		Matrix3<T> outMatrix;
		return outMatrix;
	}
	
	inline T determinant()
	{
		return 0.0;
	}
	
	// Solve a System like A * x = rhs. returns result in x.
	void solve( Vector3<T>& rhs, Vector3<T>& x )
	{
	}
	
	Vector3<T> solve( Vector3<T>& rhs )
	{
		Vector3<T> solution;
		return solution;
	}
	
	
public:
	Vector3<T> col1, col2, col3;
};

typedef Matrix3<dfloat> Matrix3f;
typedef Matrix3<dfloat> Matrix3d;


END_NAMESPACE_DIRAC2D

#endif