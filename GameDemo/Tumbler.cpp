/*
 *  Tumbler.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 20/12/12.
 *
 */

#include "Tumbler.h"
#include "Crate.h"
#include "FirstGame.h"
#include "Camera.h"
#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "../Dirac2D/Dirac2D.h"

FirstGame* Tumbler::m_pGame = 0;
int Tumbler::m_TumblerCount = 0;


Tumbler::Tumbler(TumblerInfo tInfo, FirstGame* pGame)
{
	m_ObjectInfo.m_ObjectType = EOT_TUMBLER;
	m_pGame = pGame;
	
    m_NumObjects = tInfo.m_NumObjects;
    m_bLidOpened = false;
	m_LidOpenDistance = 6.0f;
	
	PhysicalAppearance pApp;
	m_pTumblerBody = m_pGame->getPhysicalWorld()->createPhysicalBody();
	m_pTumblerBody->m_BodyType = EBT_STATIC;
	
	Vector2f c = tInfo.m_Position;
	float w = tInfo.m_Width * 0.5f;
	float h = tInfo.m_Height * 0.5f;
	
	float wallWidth = 0.02f;
	
	Vector2f vertices0[4] = { Vector2f(w,wallWidth), Vector2f(-w,wallWidth), Vector2f(-w,-wallWidth), Vector2f(w,-wallWidth)};
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.0f,h);
    PolygonInfo pInfo0(vertices0, 4);
    pApp.m_CollisionAttributes.m_CollisionShapeInfo = &pInfo0;
	m_pTumblerBody->createPhysicalShape(pApp);
	
	Vector2f vertices1[4] = { Vector2f(h,wallWidth), Vector2f(-h,wallWidth), Vector2f(-h,-wallWidth), Vector2f(h,-wallWidth)};
    PolygonInfo pInfo1(vertices1, 4);
    pApp.m_CollisionAttributes.m_CollisionShapeInfo = &pInfo1;
	pApp.m_PhysicalAttributes.m_Position = Vector2f(w,0.0f);
	pApp.m_PhysicalAttributes.m_Angle = PI_2;
	m_pTumblerBody->createPhysicalShape(pApp);

	pApp.m_PhysicalAttributes.m_Position = Vector2f(-w,0.0f);
	pApp.m_PhysicalAttributes.m_Angle = PI_2;
	m_pTumblerBody->createPhysicalShape(pApp);
	
	m_pTumblerBody->setPosition(c);
	
	m_pTumblerLidBody = m_pGame->getPhysicalWorld()->createPhysicalBody();
    PolygonInfo pInfo(vertices0, 4);
    pApp.m_CollisionAttributes.m_CollisionShapeInfo = &pInfo;
	pApp.m_PhysicalAttributes.m_Angle = 0.0f;
	m_pTumblerLidBody->createPhysicalShape(pApp);
	
	m_pTumblerLidBody->setPosition(Vector2f(c.x+w, c.y-1.05*h));
    
    HingeConstraintInfo hInfo;
    hInfo.m_PhysicalBody1 = m_pTumblerBody;
	hInfo.m_PhysicalBody2 = m_pTumblerLidBody;
	hInfo.m_Anchor = c - Vector2f(0.0f,h);
    hInfo.m_UpperAngle = PI_4/1115;
    
	m_pLidConstraint = (HingeConstraint*)m_pGame->getPhysicalWorld()->createConstraint(hInfo);
	
	m_pLidConstraint->initialize();
	
    for( int i = 0; i< m_NumObjects; i++ )
    {
        CrateInfo cInfo;
		cInfo.m_Density = 150 * AVG_OBJECT_DENSITY;
        cInfo.m_ShapeType = ECS_CIRCLE;
        cInfo.m_Radius = w/8;
        float x = RANDOM_NUMBER(-w, w);
        float y = RANDOM_NUMBER(-h, h);
        cInfo.m_Position = c + Vector2f(x,y);
        GameObject* pObject = m_pGame->getObjectFactory()->createObject(cInfo);
        m_pGame->getObjectManager()->add(pObject);
    }
    
	m_TumblerCount++;
}

void Tumbler::openLid()
{
    m_pLidConstraint->m_UpperAngle = PI_4;
	m_pLidConstraint->initialize();
}

void Tumbler::update(float dt)
{
	m_Position = m_pTumblerBody->m_Position;
    if( !m_bLidOpened && fabs(m_pGame->getCamera()->getPosition().x - m_Position.x) < m_LidOpenDistance )
    {
		m_bLidOpened = true;
        openLid();
    }
}

Tumbler::~Tumbler()
{
	m_pGame->getPhysicalWorld()->deletePhysicalBody(m_pTumblerBody);
	m_pGame->getPhysicalWorld()->deletePhysicalBody(m_pTumblerLidBody);
	m_pTumblerBody = 0;
	m_TumblerCount--;
}