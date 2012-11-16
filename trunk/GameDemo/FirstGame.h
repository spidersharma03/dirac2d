//
//  FirstGame.h
//  Dirac2D
//
//  Created by Prashant Sharma on 27/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//


#include <vector>
#include "Game.h"

using namespace std;

#ifndef _FIRST_GAME_H_
#define _FIRST_GAME_H_

class SimpleVehicle;
class Camera;

BEGIN_NAMESPACE_DIRAC2D
class PhysicalWorld;
END_NAMESPACE_DIRAC2D

USE_NAMESPACE_DIRAC2D

class GameObject;
class TerrainGenerator;
class ObjectGenerator;

class FirstGame : public Game
{
	
public:
    FirstGame();
    
    virtual void initGame();
    
    virtual void initGameFromFile(const char* fileName);

    virtual void gameLoop();
    
    virtual void render();
    
public:
    virtual void keyProcessor(unsigned char key, int x, int y);
    
public:
	PhysicalWorld* getPhysicalWorld()
	{
		return m_pWorld;
	}
    
    Camera* getCamera()
    {
        return m_pCamera;
    }
    
    TerrainGenerator* getTerrainGenerator()
    {
        return m_pTerrainGenerator;
    }
	
	SimpleVehicle* getVehicle()
	{
		return m_pVehicle;
	}
	
	ObjectGenerator* getObjectGenerator()
	{
		return m_pObjectGenerator;
	}
private:
    TerrainGenerator* m_pTerrainGenerator;
    SimpleVehicle* m_pVehicle;
	PhysicalWorld* m_pWorld;
	Camera* m_pCamera;
	ObjectGenerator* m_pObjectGenerator;
	
	float m_StepSize;
	
	vector<GameObject*> m_vecGameObjects;
};

#endif
