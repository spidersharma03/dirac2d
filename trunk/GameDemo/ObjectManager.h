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
#include <set>

using namespace std;
USE_NAMESPACE_DIRAC2D


#define MAX_CRATES_ON_SCREEN 10
#define MAX_COINS_ON_SCREEN 20

#define MAX_GAME_OBJECTS 200
#define NUM_COINS 10

class FirstGame;
class GameObjectList;
class GameObject;

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
    void add(GameObject* pObjectList);
    
	// Mark these Objects for deletion after the WorldStep.
	void markObjectsForCleanup(GameObject* pObject);
	
	// Removes marked GameObjects from the Game.
	void cleanup();
private:
    void generateCoins();   
    
	void generateCrates();
    
    // Removes Game Objects from the Global List    
    void remove(GameObject* pObjectList);
    
    ~ObjectManager();
    
private:
	FirstGame* m_pGame;
	DTimer m_Timer;
    GameObject* m_pObjectList;
    
	// TODO -- use some different structure
	set<GameObject*> m_SetMarkedObjects;
    int m_NumCoins;
};

#endif