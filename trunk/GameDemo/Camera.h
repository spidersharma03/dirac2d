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
	
    void setZoomValue( const float zValue )
    {
        m_EyePosition.z = zValue;
    }
	
	void setFocusTarget(GameObject* pTarget)
	{
		m_pFocusTarget = pTarget;
	}
	
protected:
	
	void followTarget();
	
protected:
    GameObject* m_pFocusTarget;
    Vector3f m_EyePosition;
};

#endif
