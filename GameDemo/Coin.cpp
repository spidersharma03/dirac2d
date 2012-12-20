//
//  Coin.cpp
//  Dirac2D
//
//  Created by Prashant Sharma on 17/11/12.
//

#include "Coin.h"
#include "FirstGame.h"
#include "ObjectManager.h"
#include "../Dirac2D/Dirac2D.h"

USE_NAMESPACE_DIRAC2D

FirstGame* Coin::m_pGame = 0;
int Coin::m_CoinCount = 0;

Coin::Coin(FirstGame* pGame)
{
	m_ObjectInfo.m_ObjectType = EOT_COIN;
	m_Value = 0;
	m_pBody = 0;	
	m_CoinCount++;
}

Coin::Coin(CoinInfo cInfo, FirstGame* pGame) : GameObject(cInfo)
{
    m_ObjectInfo.m_ObjectType = EOT_COIN;
	m_Value = cInfo.m_Value;
	m_Radius = cInfo.m_Radius;
	m_pGame = pGame;
	
	PhysicalAppearance pApp;
	pApp.m_MassAttributes.m_Density = 1;
	m_pBody = m_pGame->getPhysicalWorld()->createPhysicalBody();
	m_pBody->m_BodyType = EBT_KINEMATIC;
	pApp.m_CollisionAttributes.m_Shape = new Circle(m_Radius);
	PhysicalShape* pShape = m_pBody->createPhysicalShape(pApp);
    pShape->setUserData(this);
	pShape->setSensor(true);	
	m_CoinCount++;
}

void Coin::update(float dt)
{
    m_Position = m_pBody->m_Position;
}

Coin::~Coin()
{
    m_pGame->getPhysicalWorld()->deletePhysicalBody(m_pBody);
	m_pBody = 0;
	m_Value = -1;
	m_CoinCount--;
}

void Coin::onCollisionEnter( GameObject* pObject, ContactManifold& manifold )
{
    if( pObject && pObject->getGameObjectInfo().m_ObjectType == EOT_VEHICLE ) 
    {
		m_pGame->getObjectManager()->markObjectsForCleanup(this);
    }
	
}

void Coin::onCollisionExit( GameObject* pObject, ContactManifold& manifold )
{
    
}