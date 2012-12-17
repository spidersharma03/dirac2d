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
	pApp.m_MassAttributes.m_Density = 50*AVG_OBJECT_DENSITY;
	m_pBody = m_pGame->getPhysicalWorld()->createPhysicalBody();

	if( cInfo.m_ShapeType == ECS_CIRCLE )
		pApp.m_CollisionAttributes.m_Shape = new Circle(cInfo.m_Radius);
	else if( cInfo.m_ShapeType == ECS_POLY )
	{
		pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(cInfo.m_Vertices, cInfo.m_NumVertices);
	}
	else {
		float w = 0.5*cInfo.m_Width; float h = 0.5*cInfo.m_Height;
		Vector2f vertices[4] = { Vector2f(w,h), Vector2f(-w,h), Vector2f(-w,-h), Vector2f(w,-h)};
		pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
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