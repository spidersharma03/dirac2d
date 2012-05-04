/*
 *  Matrix3.h
 *  Dirac2D
 *
 *  Created by Prashant on 04/05/12.
 *
 */

#include "Vector2.h"
#include "Vector3.h"

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
		setIdentity();
	}
	
	void rotate(T angle)
	{
		T cs = cos(angle); T sn = sin(angle);
		col1.x = cs; col2.x = -sn;
		col1.x = sn; col2.x = cs;
	}
	
	void translate( Vector2<T>& translation )
	{
		col3.x = translation.x;
		col3.y = translation.y;
	}
	
	Vector2<T> operator*( const Vector2<T>& v )
	{
		return Vector2<T>( col1.x*v.x + col2.x*v.y,  col1.y*v.x + col2.y*v.y);
	}
	
	//Point2<T> operator*( const Point2<T>& p )
//	{
//		return Vector2<T>( col1.x*p.x + col2.x*p.y + col3.x,  col1.y*p.x + col2.y*p.y + col3.y);
//	}
	
	void transformAsPoint(Vector2<T>& v)
	{
	}
	
	void transformAsVector(Vector2<T>& v)
	{
	}
	
	Matrix3<T> getInverse()
	{
	}
	
	inline Matrix2<T> getRotationMatrix()
	{
		return Matrix2<T>(col1.x, col1.y, col2.x, col2.y);
	}
	
	inline Matrix2<T> getRotationMatrixTransposed()
	{
		return Matrix2<T>(col1.x, col2.x, col1.y, col2.y);
	}
	
	void setIdentity()
	{
		col1.x = 1.0f; col2.x = 0.0f; col3.x = 0.0f;
		col1.y = 0.0f; col2.y = 1.0f; col3.y = 0.0f;
		col1.z = 0.0f; col2.z = 0.0f; col3.z = 1.0f;
	}
	
public:
	Vector3<T> col1, col2, col3;
};

typedef Matrix3<dfloat> Matrix3f;
typedef Matrix3<dfloat> Matrix3d;


END_NAMESPACE_DIRAC2D

#endif