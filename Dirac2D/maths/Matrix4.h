/*
 *  Matrix4.h
 *  Dirac2D
 *
 *  Created by Prashant on 04/05/12.
 *
 */

#include "Vector2.h"
#include "Vector3.h"
#include <math.h>

#ifndef _MATRIX4_H_
#define _MATRIX4_H_

BEGIN_NAMESPACE_DIRAC2D

template< class T >
class Matrix4
{
public:
	Matrix4()
	{
		setIdentity();
	}
	
	Matrix4(const Matrix4<T>& other)
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
	
	inline Vector2<T> operator*( const Vector2<T>& v ) const
	{
		return Vector2<T>( col1.x*v.x + col2.x*v.y + col3.x,  col1.y*v.x + col2.y*v.y + col3.y);
	}
	
	inline Vector3<T> operator*( const Vector3<T>& v ) const
	{
		return Vector3<T>( col1.x*v.x + col2.x*v.y + col3.x*v.z,
                          col1.y*v.x + col2.y*v.y + col3.y*v.z, 
                          col1.z*v.x + col2.z*v.y + col3.z*v.z);
	}
	
	inline void transformAsPoint(Vector2<T>& v) const
	{
		dfloat x = col1.x*v.x + col2.x*v.y + col3.x;
		dfloat y = col1.y*v.x + col2.y*v.y + col3.y;
		v.x = x;
		v.y = y;
	}
	
	inline void transformAsVector(Vector2<T>& v) const
	{
		dfloat x = col1.x*v.x + col2.x*v.y;
		dfloat y = col1.y*v.x + col2.y*v.y;
		v.x = x;
		v.y = y;
	}
	
	inline Matrix2<T> getRotationMatrix() const
	{
		return Matrix2<T>(col1.x, col1.y, col2.x, col2.y);
	}
	
	inline Matrix2<T> getRotationMatrixTransposed()
	{
		return Matrix2<T>(col1.x, col2.x, col1.y, col2.y);
	}
	
	inline void setIdentity()
	{
		m00 = 1.0; m01 = 0.0; m02 = 0.0; m03 = 0.0;
        m10 = 0.0; m11 = 1.0; m12 = 0.0; m13 = 0.0;
        m20 = 0.0; m21 = 0.0; m22 = 1.0; m23 = 0.0;
        m30 = 0.0; m31 = 0.0; m32 = 0.0; m33 = 1.0;;
	}
    
    void getPointer( T* data )
    {        
        data[0]  = m00 ; data[1]  = m10; data[2]  = m20; data[3]  = m30;
        data[4]  = m01 ; data[5]  = m11; data[6]  = m21; data[7]  = m31;
        data[8]  = m02 ; data[9]  = m12; data[10] = m22; data[11] = m32;
        data[12] = m03 ; data[13] = m13; data[14] = m23; data[15] = m33;        
    }
	
		
	inline void getInverse(Matrix4<T>& other)
	{
		
	}
    
	inline Matrix4<T> getInverse()
	{
		Matrix4<T> outMatrix;
		
		return outMatrix;
	}
	
	inline T determinant()
	{
		// Volume of a Parallelepid with col1, col2 and col3 as axis.
		return col1.dot( col2.cross(col3) );
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
    
    void lookAt(const Vector3<T>& eye , const Vector3<T>& target , const Vector3<T>& up )
    {
        setIdentity();
        Vector3<T> dir , newUp;
        
        dir = target - eye;
        dir.normalize();
        
        Vector3<T> n;
        n = dir.cross(up);
        n.normalize();
        
        newUp = n.cross(dir);
        
        T eyeX = n.dot(eye);
        T eyeY = newUp.dot(eye);
        T eyeZ = dir.dot(eye);
        
        m00 = n.x  ;  m01 = n.y  ;  m02 = n.z  ;  m03 = -eyeX;
        m10 = newUp.x ;  m11 = newUp.y ;  m12 = newUp.z ;  m13 = -eyeY;
        m20 = -dir.x;  m21 = -dir.y;  m22 = -dir.z;  m23 = eyeZ;
    }
    
    
    void lookAt(T eyeX , T eyeY , T eyeZ
                          , T targetX , T targetY , T targetZ , T upX , T upY , T upZ )
    {
        setIdentity();
        Vector3<T> dir , newUp , eye(eyeX , eyeY, eyeZ) , target(targetX , targetY , targetZ);
        Vector3<T> up(upX, upY, upZ);
        dir = target - eye;
        dir.normalize();
        
        Vector3<T> n;
        n  = dir.cross(up);
        n.normalize();
        
        newUp = n.cross(dir);
        
        m00 = n.x  ;  m01 = n.y  ;  m02 = n.z  ;  m03 = n.dot(eye);
        m10 = newUp.x ;  m11 = newUp.y ;  m12 = newUp.z ;  m13 = newUp.dot(eye);
        m20 = -dir.x;  m21 = -dir.y;  m22 = -dir.z;  m23 = dir.dot(eye);
    }
    
    void frustum(T left , T right , T bottom , T top , T zNear , T zFar )
    {
        setIdentity();
        
        m00 = (2*zNear/(right-left));
        m01 = 0;
        m02 = (right + left )/(right - left );
        m03 = 0;
        
        m10 = 0;
        m11 = (2*zNear/(top-bottom));
        m12 = (top + bottom)/(top - bottom);
        m13 = 0;
        
        m20 = 0;
        m21 = 0;
        m22 = -(zFar + zNear)/(zFar-zNear);
        m23 = -2*zFar*zNear/(zFar-zNear);
        
        m30 = 0;
        m31 = 0;
        m32 = -1;
        m33 = 0;
    }
    
    void ortho(T left , T right , T bottom , T top , T zNear , T zFar)
    {
        setIdentity();
        
        m00 = (2/(right-left));
        m01 = 0;
        m02 = -(right + left )/(right - left );
        m03 = 0;
        
        m10 = 0;
        m11 = (2/(top-bottom));
        m12 = -(top + bottom)/(top - bottom);
        m13 = 0;
        
        m20 = 0;
        m21 = 0;
        m22 = -2/(zFar-zNear);
        m23 = -(zFar + zNear)/(zFar-zNear);
        
        m30 = 0;
        m31 = 0;
        m32 = 0;
        m33 = 1.0f;
    }
    
    void perspective(T fov , T aspectRatio , T zNear , T zFar)
    {
        setIdentity();
        
        T volumeHeight = 2 * zNear * tan(fov/2);
        T volumeWidth  = volumeHeight * aspectRatio;
        
        m00 = 2*zNear/volumeWidth;
        m01 = 0;
        m02 = 0;
        m03 = 0;
        
        m10 = 0;
        m11 = 2*zNear/volumeHeight;
        m12 = 0;
        m13 = 0;
        
        m20 = 0;
        m21 = 0;
        m22 = -(zFar + zNear)/(zFar-zNear);
        m23 = -2*zFar*zNear/(zFar-zNear);
        
        m30 = 0;
        m31 = 0;
        m32 = -1;
        m33 = 0;
    }
	
public:
	Vector3<T> col1, col2, col3;
    T m00, m01, m02, m03;
    T m10, m11, m12, m13;
    T m20, m21, m22, m23;
    T m30, m31, m32, m33;    
};

typedef Matrix4<dfloat> Matrix4f;
typedef Matrix4<dfloat> Matrix4d;


END_NAMESPACE_DIRAC2D

#endif