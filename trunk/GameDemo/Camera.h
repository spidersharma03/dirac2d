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

class Camera : public IUpdatable
{
public:
    Camera();
    
    virtual void update(float dt);

    void setZoomValue( float zValue )
    {
        m_EyePosition.z = zValue;
    }
protected:
    
    Vector3f m_EyePosition;
};

#endif
