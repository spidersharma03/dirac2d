//
//  FirstGame.cpp
//  Dirac2D
//
//  Created by Prashant Sharma on 27/10/12.
//

#include "Coin.h"
#include "FirstGame.h"
#include "SimpleVehicle.h"
#include "Camera.h"
#include "TerrainGenerator.h"
#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "GameCollisions.h"
#include "CoinPlacementStrategy.h"
#include "Crate.h"

FirstGame::FirstGame()
{
    m_pWorld = new PhysicalWorld();
	GLRenderer* glRenderer = new GLRenderer(m_pWorld);
	m_pWorld->setRenderer(glRenderer);
    
	m_pRenderer = new GLRenderer(m_pWorld);
	
	m_pVehicle = new SimpleVehicle(this);
	
	m_pCamera = new Camera();
	m_pCamera->setFocusTarget(m_pVehicle);
	
	m_pTerrainGenerator = new TerrainGenerator(this);
	
	m_pObjectFactory = new ObjectFactory(this);
	
	m_pObjectManager = new ObjectManager(this);
	
    m_pGameCollisionListener = new GameCollisionListener(this);
	    	
    m_pObjectPlacementStrategy[0] = new CoinPlacementStraregy(this);
    
	m_StepSize = 1.0f/1600.0f;
}

void FirstGame::initGame()
{
}

void FirstGame::initGameFromFile(const char* fileName)
{
    
}

void FirstGame::gameLoop()
{	
    m_pWorld->Step(m_StepSize);
	
    m_pObjectManager->update(m_StepSize);
    
    m_pTerrainGenerator->update(m_StepSize);
    
    m_pVehicle->update(m_StepSize);
    
	m_pCamera->update(m_StepSize);
	
	cleanUp();
}

void FirstGame::render()
{
    m_pTerrainGenerator->render();
    
    m_pWorld->draw();
	
	m_pVehicle->render();
}

void FirstGame::keyProcessor(unsigned char key, int x, int y)
{
    
}

void FirstGame::placeObjects(GameObject* pList, int numObjects)
{
    m_pObjectPlacementStrategy[pList->getGameObjectInfo().m_ObjectType]->placeObjects(pList, numObjects);
}

void FirstGame::cleanUp()
{
	m_vecOverlapQueryGameObjects.clear();
}

// CALL BACKS

void FirstGame::overlapCallBack(dint32 overlapNodeID, void* userData)
{
	if( !userData )
		return;
	BroadPhaseNode* pNode = (BroadPhaseNode*)userData;
	PhysicalShape* pShape = pNode->m_PhysicalShape;
	GameObject* pObject = (GameObject*)pShape->getUserData();
	if( pObject )
	{
		if( pObject->getGameObjectInfo().m_ObjectType != EOT_CRATE )
			return;
		m_vecOverlapQueryGameObjects.push_back(pObject);
	}
}

void FirstGame::applyImpulse(const Vector2f& impulseCentre, const float impulseMagnitude)
{
	for( int i=0; i<m_vecOverlapQueryGameObjects.size(); i++ )
	{
		Crate* pCrate = (Crate*)m_vecOverlapQueryGameObjects[i];
		PhysicalBody* pBody = pCrate->getPhysicalBody();
		float d = impulseCentre.distanceSquared(pBody->m_Position);
		Vector2f impulse = pBody->m_Position - impulseCentre;
		impulse.normalize();
		if( d*d > EPSILON * EPSILON )
			impulse *=  ( 1.0f/d * impulseMagnitude );
		pCrate->getPhysicalBody()->applyImpulse(impulse);
	}
}
