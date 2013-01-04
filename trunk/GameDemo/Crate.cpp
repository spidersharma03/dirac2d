/*
 *  Crate.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 07/12/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "Crate.h"
#include "FirstGame.h"
#include "../Dirac2D/Dirac2D.h"

FirstGame* Crate::m_pGame = 0;
int Crate::m_CrateCount = 0;

USE_NAMESPACE_DIRAC2D

Crate::Crate(CrateInfo cInfo, FirstGame* pGame)
{
	m_ObjectInfo.m_ObjectType = EOT_CRATE;
	m_pGame = pGame;
	
	PhysicalAppearance pApp;
	pApp.m_MassAttributes.m_Density = cInfo.m_Density;
	m_pBody = m_pGame->getPhysicalWorld()->createPhysicalBody();
	m_pBody->setPosition(cInfo.m_Position);
    m_pBody->setVelocity(cInfo.m_Velocity);
    m_pBody->setAngularVelocity(cInfo.m_AngularVelocity);
	
	if( cInfo.m_BodyType == 1 )
	{
		m_pBody->m_BodyType = EBT_STATIC;
	}
	if( cInfo.m_BodyType == 2 )
	{
		m_pBody->m_BodyType = EBT_KINEMATIC;
	}
	
	if( cInfo.m_ShapeType == ECS_CIRCLE )
    {
        CircleInfo circleInfo(cInfo.m_Radius);
		pApp.m_CollisionAttributes.m_CollisionShapeInfo = &circleInfo;
    }
	else if( cInfo.m_ShapeType == ECS_POLY )
	{
        PolygonInfo pInfo(cInfo.m_Vertices, cInfo.m_NumVertices);
		pApp.m_CollisionAttributes.m_CollisionShapeInfo = &pInfo;
	}
	else if( cInfo.m_ShapeType == ECS_REGULAR_POLY )
	{
        Vector2f vertices[10];
        dfloat angle = 0.0f;
        dfloat dAngle = 2*PI/(cInfo.m_NumVertices);
        dfloat x, y;
        
        for( dint32 v=0; v<cInfo.m_NumVertices; v++ )
        {
            x = cInfo.m_Radius * cos(angle);
            y = cInfo.m_Radius * sin(angle);
            vertices[v].x = x;
            vertices[v].y = y;
            angle += dAngle;
        }
        PolygonInfo pInfo(vertices, cInfo.m_NumVertices);
		pApp.m_CollisionAttributes.m_CollisionShapeInfo = &pInfo;
	}
	else {
		float w = 0.5*cInfo.m_Width; float h = 0.5*cInfo.m_Height;
		Vector2f vertices[4] = { Vector2f(w,h), Vector2f(-w,h), Vector2f(-w,-h), Vector2f(w,-h)};
        PolygonInfo pInfo(vertices, 4);
		pApp.m_CollisionAttributes.m_CollisionShapeInfo = &pInfo;
	}

	
	PhysicalShape* pShape = m_pBody->createPhysicalShape(pApp);
	pShape->m_CollisionFilter.m_CollisionBit = EOCB_PHYSICAL_BODY;
    pShape->m_CollisionFilter.m_CollisionMask = EOCB_PARTICLE_DEBRIS;
    pShape->setUserData(this);
	m_CrateCount++;
}

Crate::Crate(FirstGame* pGame)
{
	m_CrateCount++;
}


void Crate::update(float dt)
{
	m_Position = m_pBody->m_Position;
}

Crate::~Crate()
{
	m_pGame->getPhysicalWorld()->deletePhysicalBody(m_pBody);
	m_pBody = 0;
	m_CrateCount--;
}