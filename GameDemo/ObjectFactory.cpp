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
#include "DebrisBody.h"
#include "Tumbler.h"
#include "PathHurdle.h"
#include "CableCar.h"

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
        case EOT_DEBRIS:
			pObject = new(m_pBlockAllocator->Allocate(sizeof(DebrisBody))) DebrisBody((DebrisInfo&)gInfo, m_pGame);
			break;
		case EOT_TUMBLER:
			pObject = new(m_pBlockAllocator->Allocate(sizeof(Tumbler))) Tumbler((TumblerInfo&)gInfo, m_pGame);
			break;
		case EOT_PATH_HURDLE:
			pObject = new(m_pBlockAllocator->Allocate(sizeof(PathHurdle))) PathHurdle((PathHurdleInfo&)gInfo, m_pGame);
			break;
		case EOT_CABLE_CAR:
			pObject = new(m_pBlockAllocator->Allocate(sizeof(CableCar))) CableCar((CableCarInfo&)gInfo, m_pGame);
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
        case EOT_DEBRIS:
		{
            ((DebrisBody*)pObject)->~DebrisBody();
			m_pBlockAllocator->Free(pObject, sizeof(DebrisBody));
			break;
		}
		case EOT_TUMBLER:
		{
            ((Tumbler*)pObject)->~Tumbler();
			m_pBlockAllocator->Free(pObject, sizeof(Tumbler));
			break;
		}
		case EOT_PATH_HURDLE:
		{
            ((PathHurdle*)pObject)->~PathHurdle();
			m_pBlockAllocator->Free(pObject, sizeof(PathHurdle));
			break;
		}
		case EOT_CABLE_CAR:
		{
            ((CableCar*)pObject)->~CableCar();
			m_pBlockAllocator->Free(pObject, sizeof(CableCar));
			break;
		}
		default:
			break;
	}
}

