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
#include "../Dirac2D/Dirac2D.h"

FirstGame* Tumbler::m_pGame = 0;
int Tumbler::m_TumblerCount = 0;


Tumbler::Tumbler(TumblerInfo tInfo, FirstGame* pGame)
{
	m_ObjectInfo.m_ObjectType = EOT_TUMBLER;
	m_pGame = pGame;
	
	PhysicalAppearance pApp;
	m_pTumblerBody = m_pGame->getPhysicalWorld()->createPhysicalBody();
	m_pTumblerBody->m_BodyType = EBT_STATIC;
	
	Vector2f c = tInfo.m_Position;
	float w = tInfo.m_Width * 0.5f;
	float h = tInfo.m_Height * 0.5f;
	
	float wallWidth = 0.02f;
	
	Vector2f vertices0[4] = { Vector2f(w,wallWidth), Vector2f(-w,wallWidth), Vector2f(-w,-wallWidth), Vector2f(w,-wallWidth)};
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.0f,h);
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices0, 4);
	m_pTumblerBody->createPhysicalShape(pApp);
	
	Vector2f vertices1[4] = { Vector2f(h,wallWidth), Vector2f(-h,wallWidth), Vector2f(-h,-wallWidth), Vector2f(h,-wallWidth)};
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices1, 4);
	pApp.m_PhysicalAttributes.m_Position = Vector2f(w,0.0f);
	pApp.m_PhysicalAttributes.m_Angle = PI_2;
	m_pTumblerBody->createPhysicalShape(pApp);

	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices1, 4);
	pApp.m_PhysicalAttributes.m_Position = Vector2f(-w,0.0f);
	pApp.m_PhysicalAttributes.m_Angle = PI_2;
	m_pTumblerBody->createPhysicalShape(pApp);
	
	m_pTumblerBody->setPosition(c);
	
	m_pTumblerLidBody = m_pGame->getPhysicalWorld()->createPhysicalBody();
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices0, 4);
	pApp.m_PhysicalAttributes.m_Angle = 0.0f;
	m_pTumblerLidBody->createPhysicalShape(pApp);
	
	m_pTumblerLidBody->setPosition(Vector2f(c.x, c.y-2*h));

	HingeConstraint* hc = (HingeConstraint*)m_pGame->getPhysicalWorld()->createConstraint(ECT_HINGE);
	//hc->m_Erp = 0.0f;
	hc->m_PhysicalBody1 = m_pTumblerBody;
	hc->m_PhysicalBody2 = m_pTumblerLidBody;
	hc->m_Anchor = Vector2f(0.0f,0.0f);
	hc->initialize();
	
	m_TumblerCount++;
}


void Tumbler::update(float dt)
{
	m_Position = m_pTumblerBody->m_Position;
}

Tumbler::~Tumbler()
{
	m_pGame->getPhysicalWorld()->deletePhysicalBody(m_pTumblerBody);
	m_pGame->getPhysicalWorld()->deletePhysicalBody(m_pTumblerLidBody);
	m_pTumblerBody = 0;
	//m_pTumblerLidBody = 0;
	m_TumblerCount--;
}