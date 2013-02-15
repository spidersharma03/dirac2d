/*
 *  CableCar.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 14/02/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "CableCar.h"
#include "FirstGame.h"

CableCar::CableCar( CableCarInfo cInfo, FirstGame* pGame ) : GameObject( cInfo )
{
	m_ObjectInfo.m_bAutoCull = false;
	m_pGame = pGame;
	m_FixedPoint1 = cInfo.m_FixedPoint1;
	m_FixedPoint2 = cInfo.m_FixedPoint2;
	m_FixedLength = cInfo.m_FixedLength;	
	
	m_pCableCarBody = m_pGame->getPhysicalWorld()->createPhysicalBody();
	PhysicalAppearance pApp;
	CapsuleInfo circleInfo(0.2f,0.4f);
	
	pApp.m_CollisionAttributes.m_CollisionShapeInfo = &circleInfo;
	PhysicalShape* pShape = m_pCableCarBody->createPhysicalShape(pApp);
	m_pCableCarBody->setPosition(cInfo.m_Position);
	m_pCableCarBody->setAngle(PI_2);
	m_pCableCarBody->setVelocity(Vector2f(5.0f,0.0f));
	pShape->setSensor(true);
	
	CatenaryConstraintInfo constraintInfo;
	constraintInfo.m_PhysicalBody1 = m_pCableCarBody;
	constraintInfo.m_Anchor = Vector2f(0.4f,0.0f);
	constraintInfo.m_FixedPoint1 = m_FixedPoint1;
	constraintInfo.m_FixedPoint2 = m_FixedPoint2;
	constraintInfo.m_FixedLength = m_FixedLength;
	constraintInfo.m_Frequency = 10.0f;
	constraintInfo.m_DampingRatio = 0.0f;
	m_pCableCarConstraint = (CatenaryConstraint*)m_pGame->getPhysicalWorld()->createConstraint(constraintInfo);
	m_pCableCarConstraint->initialize();
}


void CableCar::init()
{
   
}

void CableCar::update(float dt)
{
   
}

CableCar::~CableCar()
{
	m_pGame->getPhysicalWorld()->deletePhysicalBody(m_pCableCarBody);
	m_pGame->getPhysicalWorld()->deleteConstraint(m_pCableCarConstraint);
	m_pCableCarBody = 0;
	m_pCableCarConstraint = 0;
}
// COLLISION CALL BACKS

void CableCar::onCollisionEnter( GameObject* pObject, ContactManifold& manifold )
{
}

void CableCar::onCollisionExit( GameObject* pObject, ContactManifold& manifold )
{
}
