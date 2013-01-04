//
//  Camera.cpp
//  Dirac2D
//
//  Created by Prashant Sharma on 28/10/12.
//

#include "Camera.h"
#include "GameObject.h"
#include "FirstGame.h"
#include "TerrainGenerator.h"
#include "SimpleVehicle.h"

Camera::Camera(FirstGame* pGame) : m_pGame(pGame)
{
	m_pFocusTarget = 0;
    m_EyePosition = Vector3f(0.0f,0.0f,1.0f);
    m_Target = Vector3f(0.0f,0.0f,-1.0f);
    m_ViewMatrix.lookAt(m_EyePosition, m_Target, Vector3f(0.0f,1.0f,0.0f));

    m_Elasticity = 100.0f;
    m_AspectRatio = 4.0f/3.0f;
	m_InitEyeZ = 5.0f;
	m_EyePosition.z = m_InitEyeZ;
    
    m_ScreenHeight = 2*m_EyePosition.z;
    m_ScreenWidth = m_ScreenHeight * m_AspectRatio;
    
    m_cameraOffset = Vector2f(4.0,1.0f);
	m_WindowWidth = 800;
	m_WindowHeight = 600;
}

void Camera::update(float dt)
{
	if( m_pFocusTarget )
		followTarget(dt);

	autoZoom();
	
    float data[16];

    
	m_ProjectionMatrix.perspective(PI/2, m_AspectRatio, 0.001f, 100.0f);
    //m_ProjectionMatrix.ortho(-m_ScreenWidth/2*m_AspectRatio, m_ScreenWidth/2*m_AspectRatio, -m_ScreenWidth/2, m_ScreenWidth/2, 0.001, 100.0f);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    m_ProjectionMatrix.getPointer(data);
    glLoadMatrixf(data);
    
    m_ViewMatrix.lookAt(m_EyePosition, m_Target, Vector3f(0.0f,1.0f,0.0f));

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    m_ViewMatrix.getPointer(data);
    glLoadMatrixf(data);
}

void Camera::autoZoom()
{
	if( m_pFocusTarget )
	{
		float y = m_pFocusTarget->getPosition().y;
		//float y = m_pGame->getVehicle()->getDistanceFromTerrain();
        float currentYlevel = m_pGame->getTerrainGenerator()->getAvgLevel(m_pFocusTarget->getPosition().x);
        y -= currentYlevel;
        //printf(" y = %f\n", y);
		if( y > 0 )
		{
			m_EyePosition.z = m_InitEyeZ + y;
            m_ScreenHeight = 2*m_EyePosition.z;
			m_ScreenWidth = m_ScreenHeight * m_AspectRatio;
		}
	}
}

void Camera::followTarget(float dt)
{
    Vector2f targetPos = m_pFocusTarget->getPosition();
    Vector2f vel = (targetPos - Vector2f(m_EyePosition.x, m_EyePosition.y) + m_cameraOffset) * m_Elasticity;
    m_Target.x = m_EyePosition.x += vel.x * dt;
    m_Target.y = m_EyePosition.y += vel.y * dt;
}

