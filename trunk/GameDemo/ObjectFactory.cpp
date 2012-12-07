/*
 *  ObjectGenerator.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 16/11/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "ObjectFactory.h"
#include "FirstGame.h"
#include "Camera.h"
#include "Coin.h"
#include "Crate.h"

ObjectFactory::ObjectFactory(FirstGame* pGame) : m_pGame( pGame )
{
	//m_vecPhysicalBodies.reserve(MAX_COINS_ON_SCREEN);
    
    //m_pCoinPool = new MemoryAllocator<Coin>(MAX_COINS_ON_SCREEN);
	m_pBlockAllocator = new MemoryBlockAllocator();
}

GameObject* ObjectFactory::createObject(GameObjectInfo& gInfo)
{
	GameObject* pObject = 0;
	
	switch (gInfo.m_ObjectType) {
		case EOT_COIN:
			pObject = new(m_pBlockAllocator->Allocate(sizeof(Coin))) Coin((CoinInfo&)gInfo, m_pGame);
			break;
		case EOT_CRATE:
			pObject = new(m_pBlockAllocator->Allocate(sizeof(Crate))) Crate((CrateInfo&)gInfo, m_pGame);
			break;
		default:
			break;
	}
	return pObject;
}


void ObjectFactory::destroyObject( GameObject* pObject )
{
	switch (pObject->getGameObjectInfo().m_ObjectType) {
		case EOT_COIN:
		{
            ((Coin*)pObject)->~Coin();
			m_pBlockAllocator->Free(pObject, sizeof(Coin));
			break;
		}
		case EOT_CRATE:
		{
            ((Crate*)pObject)->~Crate();
			m_pBlockAllocator->Free(pObject, sizeof(Crate));
			break;
		}
		default:
			break;
	}
}

