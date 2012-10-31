//
//  Camera.cpp
//  Dirac2D
//
//  Created by Prashant Sharma on 28/10/12.
//

#ifndef WIN32 
#include <sys/time.h>
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif

#include "Camera.h"

Camera::Camera()
{
	m_pFocusTarget = 0;
}

void Camera::update(float dt)
{
	if( m_pFocusTarget )
		followTarget();
	
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	gluLookAt(m_EyePosition.x, m_EyePosition.y, m_EyePosition.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

void Camera::followTarget()
{
}