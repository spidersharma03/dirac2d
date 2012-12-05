//
//  Coin.cpp
//  Dirac2D
//
//  Created by Prashant Sharma on 17/11/12.
//

#include "Coin.h"
#include "FirstGame.h"
#include "../Dirac2D/Dirac2D.h"

USE_NAMESPACE_DIRAC2D

FirstGame* Coin::m_pGame = 0;

Coin::Coin(FirstGame* pGame)
{
	m_ObjectInfo.m_ObjectType = EOT_COIN;
	m_Value = 0;
	m_pBody = 0;	
}

Coin::Coin(CoinInfo cInfo, FirstGame* pGame) : GameObject(cInfo)
{
    m_ObjectInfo.m_ObjectType = EOT_COIN;
	m_Value = cInfo.m_Value;
	m_Radius = cInfo.m_Radius;
	m_pGame = pGame;
	
	PhysicalAppearance pApp;
	m_pBody = m_pGame->getPhysicalWorld()->createPhysicalBody();
	m_pBody->m_BodyType = EBT_KINEMATIC;
	pApp.m_CollisionAttributes.m_Shape = new Circle(m_Radius);
	PhysicalShape* pShape = m_pBody->createPhysicalShape(pApp);
	pShape->setSensor(true);	
}

void Coin::update(float dt)
{
}

Coin::~Coin()
{
}