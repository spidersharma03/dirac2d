/*
 *  ObjectGenerator.h
 *  Dirac2D
 *
 *  Created by Prashant on 04/12/12.
 *
 */

#ifndef _OBJECT_MANAGER_H_
#define _OBJECT_MANAGER_H_

#include "../Dirac2D/Dirac2D.h"
#include "IUpdatable.h"

USE_NAMESPACE_DIRAC2D

#define MAX_GAME_OBJECTS 200
#define NUM_COINS 10

class FirstGame;
class GameObjectList;

class ObjectManager : public IUpdatable
{
public:
	ObjectManager(FirstGame* pGame);
	
	// Generate/Destroy Objects
	void manageObjects();
	
    // Cull/Destroy Objects which are outside the view from the Camera with some tolerance.
    // since the Camera is always moving in front direction, only objects which are left behind are Culled/Destroyed.
    void cullObjects();
    
    virtual void update(float dt);

    // Add Game Objects to the Global List
    void addToPool(GameObjectList* pObjectList);
    
    // Removes Game Objects from the Global List    
    void removeFromPool(GameObjectList* pObjectList);
    
private:
    void generateCoins();   
    
    
    ~ObjectManager();
    
private:
	FirstGame* m_pGame;
	DTimer m_Timer;
    GameObjectList* m_pObjects;
    MemoryAllocator<GameObjectList> *m_pObjectPool;
    
    int m_NumCoins;
};

#endif