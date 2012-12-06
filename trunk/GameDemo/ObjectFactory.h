/*
 *  ObjectGenerator.h
 *  Dirac2D
 *
 *  Created by Prashant on 16/11/12.
 *
 */

#include "IUpdatable.h"

#ifndef _OBJECT_FACTORY_H_
#define _OBJECT_FACTORY_H_

#include "../Dirac2D/Dirac2D.h"
#include "GameObject.h"

class FirstGame;
class Coin;

USE_NAMESPACE_DIRAC2D

#define MAX_COINS_ON_SCREEN 30

class ObjectFactory : public IUpdatable
{
public:
	
	ObjectFactory(FirstGame* pGame);
		
	GameObjectList* createObjects(GameObjectInfo gInfo ,int numObjects);

	GameObject* createObject(GameObjectInfo& gInfo);

	void destroyObjects( GameObjectList* pObjectList );

	void destroyObject( GameObject* pObject );
	
    void createObjects(GAME_OBJECT_TYPE objType, int numObjects);
    
    //void recycleObjects(GAME_OBJECT_TYPE objType, int numObjects);
    
	void generateCoins( Vector2f* positions, int numCoins );
	
	virtual void update(float dt);
	
private:
	
	FirstGame* m_pGame;
	
	vector<PhysicalBody*> m_vecPhysicalBodies;
    
    MemoryAllocator<Coin>* m_pCoinPool;
	MemoryBlockAllocator *m_pBlockAllocator;
};

#endif