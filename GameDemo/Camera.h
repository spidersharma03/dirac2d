//
//  Camera.h
//  Dirac2D
//
//  Created by Prashant Sharma on 27/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include "IUpdatable.h"
#include "../Dirac2D/maths/MathUtil.h"


USE_NAMESPACE_DIRAC2D

#ifndef _CAMERA_H_
#define _CAMERA_H_

class GameObject;

class Camera : public IUpdatable
{
public:
    Camera();
    
    virtual void update(float dt);

	void setEyePosition( const Vector3f& eyePosition )
	{
		m_EyePosition = eyePosition;
	}
	
	void setEyePosition( const Vector2f& eyePosition )
	{
		m_EyePosition = eyePosition;
	}
	
    void setAspectRatio(float aspectRatio)
    {
        m_AspectRatio = aspectRatio;
    }
    
    void setZoomValue( const float zValue )
    {
        m_EyePosition.z = zValue;
    }
	
	void setFocusTarget(GameObject* pTarget)
	{
		m_pFocusTarget = pTarget;
	}
    
    void lookAt( const Vector3f& eyePos , const Vector3f& targetPos, const Vector3f& upVec)
    {
        m_ViewMatrix.lookAt( eyePos , targetPos , upVec );
    }
    
    void perspective( float fov , float aspectRatio, float zNear , float zFar )
    {
        m_Fov = fov;
        m_AspectRatio = aspectRatio;
        m_zNear = zNear;
        m_zFar  = zFar;
        m_ProjectionMatrix.perspective(fov , aspectRatio, zNear , zFar);
    }
    
    void frustrum( float left, float right, float bottom, float top, float znear, float zfar )
    {
        m_ProjectionMatrix.frustum( left, right, bottom, top, znear, zfar );
    }
    
    void ortho( float left, float right, float bottom, float top, float znear, float zfar )
    {
        m_ProjectionMatrix.ortho( left, right, bottom, top, znear, zfar );
    }
	
    Matrix4f getViewMatrix() const
    {
        return m_ViewMatrix;
    }
    
    Matrix4f getProjectionMatrix() const
    {
        return m_ProjectionMatrix; 
    }
    
    float getAspectRatio() const
    {
        return m_AspectRatio;
    }
    
    float getFov() const
    {
        return m_Fov;
    }
    
    inline bool isPointInsideView( const Vector2f& point )
    {
        return false;
    }
    
    inline float getScreenWidth()
    {
        return m_ScreenWidth;
    }
    
    inline float getScreenHeight()
    {
        return m_ScreenHeight;
    }
    
    inline Vector3f getPosition()
    {
        return m_EyePosition;
    }
protected:
	
	void followTarget(float dt);
	
protected:
    GameObject* m_pFocusTarget;
    Vector3f m_EyePosition;
    Vector3f m_Target;
    
    Vector2f m_Velocity;
    float m_Elasticity;
    
    Matrix4f m_ViewMatrix;
    Matrix4f m_ProjectionMatrix;
    
    float m_AspectRatio;
    float m_Fov;
    float m_zNear, m_zFar;
    
    float m_ScreenWidth, m_ScreenHeight;
    
    AABB2f m_BoundingRect;
};

#endif
