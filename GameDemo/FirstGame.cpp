//
//  FirstGame.cpp
//  Dirac2D
//
//  Created by Prashant Sharma on 27/10/12.
//

#include "../Dirac2D/Dirac2D.h"

#include "Coin.h"
#include "FirstGame.h"
#include "SimpleVehicle.h"
#include "Camera.h"
#include "TerrainGenerator.h"
#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "GameCollisions.h"
#include "CoinPlacementStrategy.h"

FirstGame::FirstGame()
{
    m_pWorld = new PhysicalWorld();
	GLRenderer* glRenderer = new GLRenderer(m_pWorld);
	m_pWorld->setRenderer(glRenderer);
    
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
}

void FirstGame::render()
{
    m_pTerrainGenerator->render();
    
    m_pWorld->draw();
}

void FirstGame::keyProcessor(unsigned char key, int x, int y)
{
    
}

void FirstGame::placeObjects(GameObjectList* pList, int numObjects)
{
    m_pObjectPlacementStrategy[pList->m_pObject->getGameObjectInfo().m_ObjectType]->placeObjects(pList, numObjects);
}
