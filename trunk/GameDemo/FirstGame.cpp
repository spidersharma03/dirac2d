//
//  FirstGame.cpp
//  Dirac2D
//
//  Created by Prashant Sharma on 27/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "../Dirac2D/Dirac2D.h"

#include "FirstGame.h"
#include "SimpleVehicle.h"
#include "Camera.h"
#include "TerrainGenerator.h"


FirstGame::FirstGame()
{
    m_pWorld = new PhysicalWorld();
	GLRenderer* glRenderer = new GLRenderer(m_pWorld);
	m_pWorld->setRenderer(glRenderer);
    
	m_pVehicle = new SimpleVehicle(this);
	
	m_pCamera = new Camera();
	m_pCamera->setFocusTarget(m_pVehicle);
	
	m_pTerrainGenerator = new TerrainGenerator(this);

	m_StepSize = 1.0f/900.0f;
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
