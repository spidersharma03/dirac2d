/*
 *  ObjectGenerator.h
 *  Dirac2D
 *
 *  Created by Prashant on 04/12/12.
 *
 */

#ifndef _OBJECT_GENERATOR_H_
#define _OBJECT_GENERATOR_H_

#include "../Dirac2D/Dirac2D.h"

USE_NAMESPACE_DIRAC2D

#define MAX_GAME_OBJECTS 200
#define NUM_COINS 10

class FirstGame;
class GameObjectList;

class ObjectGenerator
{
public:
	ObjectGenerator(FirstGame* pGame);
	
	// Generate/Destroy Objects
	void manageObjects();
	
    // Cull/Destroy Objects which are outside the view from the Camera with some tolerance.
    // since the Camera is always moving in front direction, only objects which are left behind are Culled/Destroyed.
    void cullObjects();
    
private:
    void generateCoins();   
    
    // Add Game Objects to the Global List
    void addToPool(GameObjectList* pObjectList);
    
    // Removes Game Objects from the Global List    
    void removeFromPool(GameObjectList* pObjectList);
    
    ~ObjectGenerator();
    
private:
	FirstGame* m_pGame;
	DTimer m_Timer;
    GameObjectList* m_pObjects;
    MemoryAllocator<GameObjectList> *m_pObjectPool;
};

#endif