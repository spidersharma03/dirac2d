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
#include "ObjectGenerator.h"
#include "GameCollisions.h"

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
	
	m_pObjectGenerator = new ObjectGenerator(this);
	
	CoinInfo cInfo;
	m_pObjectFactory->createObject(cInfo);
	
    m_pGameCollisionListener = new GameCollisionListener(this);
    
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
	m_pObjectGenerator->manageObjects();
	
    m_pWorld->Step(m_StepSize);
	
    m_pObjectFactory->update(m_StepSize);
    
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
