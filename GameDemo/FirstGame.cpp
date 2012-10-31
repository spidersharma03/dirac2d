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


FirstGame::FirstGame()
{
    m_pWorld = new PhysicalWorld();
	
	m_pVehicle = new SimpleVehicle();
	
	m_pCamera = new Camera();
	m_pCamera->setFocusTarget(m_pVehicle);
	
	m_StepSize = 1.0f/600.0f;
}

void FirstGame::initGame()
{
    m_pVehicle = new SimpleVehicle();
}

void FirstGame::initGameFromFile(const char* fileName)
{
    
}

void FirstGame::gameLoop()
{
    m_pWorld->Step(m_StepSize);
	
	m_pCamera->update(m_StepSize);
}

void FirstGame::render()
{
    
}

void FirstGame::keyProcessor(unsigned char key, int x, int y)
{
    
}
