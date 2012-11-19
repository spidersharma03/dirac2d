/*
 *  ObjectGenerator.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 16/11/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "ObjectFactory.h"
#include "../Dirac2D/Dirac2D.h"
#include "FirstGame.h"
#include "Camera.h"
#include "Coin.h"

ObjectFactory::ObjectFactory(FirstGame* pGame) : m_pGame( pGame )
{
	m_vecPhysicalBodies.reserve(MAX_COINS_ON_SCREEN);
    
    m_pCoinPool = new MemoryAllocator<Coin>(MAX_COINS_ON_SCREEN);
}

void ObjectFactory::update(float dt)
{
	Camera* pCamera = m_pGame->getCamera();
    float swh = pCamera->getScreenWidth() * 0.5f;
	
    float R = ( swh + swh * 2.0f ); 
	
    // Remove/Cull Coins
    for( int i=0; i<m_vecPhysicalBodies.size(); i++ )
    {
        PhysicalBody* pBody = m_vecPhysicalBodies[i];
        bool bRes =  (pCamera->getPosition().x - pBody->m_Position.x) > R;
        if( bRes )
        {
            PhysicalBody* pBody = m_vecPhysicalBodies[i];
            m_pGame->getPhysicalWorld()->deletePhysicalBody(pBody);
            m_vecPhysicalBodies.erase(m_vecPhysicalBodies.begin() + i);
        }
    }
	
}

void ObjectFactory::generateCoins( Vector2f* positions, int numCoins )
{
	if( m_vecPhysicalBodies.size() > MAX_COINS_ON_SCREEN )
		return;
	
	PhysicalAppearance pApp;
	for( int i=0; i<numCoins; i++ )
	{
		PhysicalBody* pBody = m_pGame->getPhysicalWorld()->createPhysicalBody();
		pBody->setPosition(positions[i]);
		pBody->m_BodyType = EBT_KINEMATIC;
		pApp.m_CollisionAttributes.m_Shape = new Circle(0.05f);
		PhysicalShape* pShape = pBody->createPhysicalShape(pApp);
		pShape->setSensor(true);
		m_vecPhysicalBodies.push_back(pBody);
	}
}