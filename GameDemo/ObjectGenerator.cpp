/*
 *  ObjectGenerator.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 16/11/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "ObjectGenerator.h"
#include "../Dirac2D/Dirac2D.h"
#include "FirstGame.h"
#include "Camera.h"

ObjectGenerator::ObjectGenerator(FirstGame* pGame) : m_pGame( pGame )
{
	m_vecPhysicalBodies.reserve(MAX_COINS_ON_SCREEN);
}

void ObjectGenerator::update(float dt)
{
	Camera* pCamera = m_pGame->getCamera();
    float swh = pCamera->getScreenWidth() * 0.5f;
	
    float R = ( swh + swh * 2.0f ); 
	
    for( int i=0; i<m_vecPhysicalBodies.size(); i++ )
    {
        PhysicalBody* pBody = m_vecPhysicalBodies[i];
        bool bRes =  (pCamera->getPosition().x - pBody->m_Position.x) > R;
        if( bRes )
        {
            m_vecPhysicalBodies.erase(m_vecPhysicalBodies.begin() + i);
        }
    }
	
}

void ObjectGenerator::generateCoins( Vector2f* positions, int numCoins )
{
	if( m_vecPhysicalBodies.size() > MAX_COINS_ON_SCREEN )
		return;
	
	PhysicalAppearance pApp;
	for( int i=0; i<numCoins; i++ )
	{
		PhysicalBody* pBody = m_pGame->getPhysicalWorld()->createPhysicalBody();
		pBody->setPosition(positions[i]);
		pBody->m_BodyType = EBT_STATIC;
		pApp.m_CollisionAttributes.m_Shape = new Circle(0.02f);
		PhysicalShape* pShape = pBody->createPhysicalShape(pApp);
		pShape->setSensor(true);
		m_vecPhysicalBodies.push_back(pBody);
	}
}