/*
 *  ObjectGenerator.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 04/12/12.
 *
 */

#include "../Dirac2D/Dirac2D.h"
#include "ObjectManager.h"
#include "FirstGame.h"
#include "Camera.h"
#include "ObjectFactory.h"
#include "Coin.h"
#include "ObjectPlacementSrategy.h"

ObjectManager::ObjectManager(FirstGame* pGame)
{
	m_pGame = pGame;
    m_pObjects = 0;
    
    m_pObjectPool = new MemoryAllocator<GameObjectList>(MAX_GAME_OBJECTS);
}

void ObjectManager::manageObjects()
{    
	static double initTime = m_Timer.getCurrentTime();
    double time = m_Timer.getCurrentTime();
    if( time - initTime > 2000 )
    {
        initTime = time;
        //
        generateCoins();
	}    
}

void ObjectManager::cullObjects()
{
    GameObjectList* pList = m_pObjects;
    
    ObjectFactory* pObjFactory = m_pGame->getObjectFactory();
    Camera* pCamera = m_pGame->getCamera();
    
    float swh = pCamera->getScreenWidth() * 0.5f;
	
    float R = ( swh + swh * 2.0f );
    
    int n = 0;
    while (pList) 
    {
        n++;
        GameObject* pObject = pList->m_pObject;
        bool bRes =  (pCamera->getPosition().x - pObject->getPosition().x) > R;
        if( bRes )
        {
            removeFromPool(pList);
            pObjFactory->destroyObject(pObject);
        }
        pList = pList->m_pNext;
    }  
}

void ObjectManager::generateCoins() 
{
    ObjectFactory* pObjFactory = m_pGame->getObjectFactory();
    
    // Coin info. 
    CoinInfo cInfo;
    cInfo.m_Radius = 0.05f;

    // Create a list of Coins
    GameObjectList pList[NUM_COINS];
    GameObjectList* pCurrent = 0;

    // Request for Coins from the Object Factory
    for( int i=0; i<NUM_COINS; i++ )
    {
       GameObject* pObject = pObjFactory->createObject(cInfo);
       pList[i].m_pObject = pObject;
       pList[i].m_pPrev = pCurrent;
       if( pCurrent )
            pCurrent->m_pNext = &pList[i];
       pList[i].m_pNext = 0;
       pCurrent = &pList[i];
    }
    
    // Place Coins in the Game
    m_pGame->placeObjects(pList, NUM_COINS);
    
    // Add to the Object Pool List
    for( int i=0; i<NUM_COINS; i++ )
    {
        GameObjectList *pObjectList = new(m_pObjectPool->Allocate()) GameObjectList();
        pObjectList->m_pObject = pList[i].m_pObject;
        addToPool(pObjectList);
    }
}

void ObjectManager::update(float dt)
{
    manageObjects();
    GameObjectList* pList = m_pObjects;
    while (pList) {
        pList->m_pObject->update(dt);
        pList = pList->m_pNext;
    }
    cullObjects();
}

ObjectManager::~ObjectManager()
{
    GameObjectList* pList = m_pObjects;
    while (pList) {
        m_pGame->getObjectFactory()->destroyObject(m_pObjects->m_pObject);
        pList = pList->m_pNext;
    }
    delete m_pObjectPool;
    m_pObjects = 0;
}

void ObjectManager::addToPool(GameObjectList* pObjectList)
{
    pObjectList->m_pPrev = 0;
    pObjectList->m_pNext = m_pObjects;
    
    if( m_pObjects )
    {
        m_pObjects->m_pPrev = pObjectList;
    }
    m_pObjects = pObjectList;
}

void ObjectManager::removeFromPool(GameObjectList* pObjectList)
{
    if( pObjectList->m_pPrev )
	{
		pObjectList->m_pPrev->m_pNext = pObjectList->m_pNext;
	}
	else 
	{
		m_pObjects = pObjectList->m_pNext;
	}
	if( pObjectList->m_pNext )
	{
		pObjectList->m_pNext->m_pPrev = pObjectList->m_pPrev;
	}
    m_pObjectPool->Free(pObjectList);
}