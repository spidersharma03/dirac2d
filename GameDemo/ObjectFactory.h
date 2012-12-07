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

#define MAX_COINS_ON_SCREEN1 30

class ObjectFactory
{
public:
	
	ObjectFactory(FirstGame* pGame);
		
	GameObject* createObject(GameObjectInfo& gInfo);

	void destroyObject( GameObject* pObject );
	        		
private:
	
	FirstGame* m_pGame;
	    
    MemoryAllocator<Coin>* m_pCoinPool;
	MemoryBlockAllocator *m_pBlockAllocator;
};

#endif