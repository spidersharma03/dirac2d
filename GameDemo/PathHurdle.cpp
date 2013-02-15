/*
 *  PathHurdle.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 30/01/13.
 *
 */

#include "PathHurdle.h"
#include "FirstGame.h"
#include "ObjectManager.h"
#include "SimpleVehicle.h"
#include "../Dirac2D/Dirac2D.h"

FirstGame* PathHurdle::m_pGame = 0;
int PathHurdle::m_PathHurdleCount = 0;

USE_NAMESPACE_DIRAC2D

PathHurdle::PathHurdle(PathHurdleInfo pInfo, FirstGame* pGame)
{
	m_ObjectInfo.m_ObjectType = EOT_PATH_HURDLE;
	m_pGame = pGame;
	
	PhysicalAppearance pApp;
	pApp.m_MassAttributes.m_Density = pInfo.m_Density;
	m_pBody = m_pGame->getPhysicalWorld()->createPhysicalBody();
	m_pBody->setPosition(pInfo.m_Position);
    
	m_pBody->m_BodyType = EBT_STATIC;
	
	Vector2f vertices[10];
	dfloat angle = 0.0f;
	dfloat dAngle = 2*PI/(pInfo.m_NumVertices);
	dfloat x, y;
	
	for( dint32 v=0; v<pInfo.m_NumVertices; v++ )
	{
		x = pInfo.m_Radius * cos(angle);
		y = pInfo.m_Radius * sin(angle);
		vertices[v].x = x;
		vertices[v].y = y;
		angle += dAngle;
	}
	PolygonInfo polyInfo(vertices, pInfo.m_NumVertices);
	pApp.m_CollisionAttributes.m_CollisionShapeInfo = &polyInfo;
	
	PhysicalShape* pShape = m_pBody->createPhysicalShape(pApp);
	//pShape->setSensor(true);	
	//pShape->m_CollisionFilter.m_CollisionBit = EOCB_PHYSICAL_BODY;
    //pShape->m_CollisionFilter.m_CollisionMask = EOCB_PARTICLE_DEBRIS;
    pShape->setUserData(this);
	m_PathHurdleCount++;
}

PathHurdle::PathHurdle(FirstGame* pGame)
{
	m_PathHurdleCount++;
}


void PathHurdle::update(float dt)
{
	m_Position = m_pBody->m_Position;
}

PathHurdle::~PathHurdle()
{
	m_pGame->getPhysicalWorld()->deletePhysicalBody(m_pBody);
	m_pBody = 0;
	m_PathHurdleCount--;
}

// COLLISION CALL BACKS

void PathHurdle::onCollisionEnter( GameObject* pObject, ContactManifold& manifold )
{
	if( pObject && pObject->getGameObjectInfo().m_ObjectType == EOT_VEHICLE ) 
    {
		// Destroy the Path Hurdle
		m_pGame->getObjectManager()->markObjectsForCleanup(this);
		m_pGame->explodeDebris(getGameObjectInfo(), getPosition());
		// slow down the vehicle
		m_pGame->getVehicle()->setVelocity(Vector2f());
		m_pGame->getVehicle()->setMotorSpeed(0.0f);
    }
}

void PathHurdle::onCollisionExit( GameObject* pObject, ContactManifold& manifold )
{
}
