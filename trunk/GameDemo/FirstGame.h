//
//  FirstGame.h
//  Dirac2D
//
//  Created by Prashant Sharma on 27/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef _FIRST_GAME_H_
#define _FIRST_GAME_H_

#include <vector>
#include "Game.h"
#include "../Dirac2D/Dirac2D.h"

using namespace std;

class SimpleVehicle;
class Camera;
class DebrisBodyCreator;

USE_NAMESPACE_DIRAC2D

class GameObject;
class GameObjectList;
class TerrainGenerator;
class ObjectFactory;
class ObjectManager;
class GameCollisionListener;
class ObjectPlacementStraregy;
struct GameObjectInfo;

class FirstGame : public Game, public OverlapCallBackClass
{
	
public:
    FirstGame();
    
    virtual void initGame();
    
    virtual void initGameFromFile(const char* fileName);

    virtual void gameLoop();
    
    virtual void render();
    
public:
    virtual void keyProcessor(unsigned char key, int x, int y);
    
	void MouseButton(int button, int state, int x, int y);
	
	void MouseMotion(int x, int y);
	
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
	
	ObjectFactory* getObjectFactory()
	{
		return m_pObjectFactory;
	}
	
    ObjectManager* getObjectManager()
	{
		return m_pObjectManager;
	}
	
	GLRenderer* getRenderer()
	{
		return m_pRenderer;
	}
	
	Vector2f getMousePositionScreenSpace() const
	{
		return m_MousePositionScreenSpace;
	}
	
	Vector2f getMousePositionWorldSpace() const
	{
		return m_MousePositionWorldSpace;
	}
	
    void placeObjects(GameObject* pList, int numObjects);
    	
public:
// CALL BACKS
	// Overlap Call Back
	void overlapCallBack(dint32 overlapNodeID, void* userData);
	
    // Aplies impulses to Objects queried from AABB overlap
	void applyImpulse(const Vector2f& impulseCentre, const float impulseMagnitude);
	
    // Creates Debris Bodies for different explosions
    void explodeDebris( const GameObjectInfo& objectInfo, const Vector2f& position );
    
private:
	
	void cleanUp();
	
private:
    TerrainGenerator* m_pTerrainGenerator;
    SimpleVehicle* m_pVehicle;
	PhysicalWorld* m_pWorld;
	Camera* m_pCamera;
	ObjectFactory* m_pObjectFactory;
	ObjectManager* m_pObjectManager;
	GameCollisionListener* m_pGameCollisionListener;
	
    DebrisBodyCreator* m_pDebrisBodyCreator;
    ObjectPlacementStraregy* m_pObjectPlacementStrategy[10];
    GLRenderer *m_pRenderer;
	
	float m_StepSize;
	
	Vector2f m_MousePositionScreenSpace;
	Vector2f m_MousePositionWorldSpace;
		
	vector<GameObject*> m_vecOverlapQueryGameObjects;
};

#endif
