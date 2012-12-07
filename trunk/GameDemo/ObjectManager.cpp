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
#include "TerrainGenerator.h"
#include "ObjectFactory.h"
#include "Coin.h"
#include "Crate.h"
#include "ObjectPlacementSrategy.h"

ObjectManager::ObjectManager(FirstGame* pGame)
{
	m_pGame = pGame;
    m_pObjectList = 0;
    //m_SetMarkedObjects.reserve(100);
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
	// Place Stack of Crates only for Linear regions
	if( m_pGame->getTerrainGenerator()->getSampleFunctionType() == ESFT_LINEAR )
	{
		static double initTime = m_Timer.getCurrentTime();
		double time = m_Timer.getCurrentTime();
		if( time - initTime > 4000 )
		{
			initTime = time;
			generateCrates();
		}
	}
}

void ObjectManager::cullObjects()
{
    GameObject* pList = m_pObjectList;
    
    ObjectFactory* pObjFactory = m_pGame->getObjectFactory();
    Camera* pCamera = m_pGame->getCamera();
    
    float swh = pCamera->getScreenWidth() * 0.5f;
	
    float R = ( swh + swh * 2.0f );
    
    int n = 0;
    while (pList) 
    {
        n++;
        bool bRes =  (pCamera->getPosition().x - pList->getPosition().x) > R;
        if( bRes )
        {
            remove(pList);
            pObjFactory->destroyObject(pList);
        }
        pList = pList->m_pNext;
    }  
}

void ObjectManager::generateCoins() 
{
	if( Coin::m_CoinCount > MAX_COINS_ON_SCREEN )
		return;

    ObjectFactory* pObjFactory = m_pGame->getObjectFactory();
    
    // Coin info. 
    CoinInfo cInfo;
    cInfo.m_Radius = 0.05f;

    // Create a list of Coins
    GameObject *pList[NUM_COINS];
    GameObject* pCurrent = 0;

    // Request for Coins from the Object Factory
    for( int i=0; i<NUM_COINS; i++ )
    {
       GameObject* pObject = pObjFactory->createObject(cInfo);
       pList[i] = pObject;
       pList[i]->m_pPrev = pCurrent;
       if( pCurrent )
            pCurrent->m_pNext = pList[i];
       pList[i]->m_pNext = 0;
       pCurrent = pList[i];
    }
    
    // Place Coins in the Game
    placeCoins(m_pGame,pList[0], NUM_COINS);
	
	for( int i=0; i<NUM_COINS; i++ )
	{
		GameObject* pObject = pList[i];
		add(pObject);
	}
}

void ObjectManager::generateCrates() 
{
	if( Crate::m_CrateCount > MAX_CRATES_ON_SCREEN )
		return;
	
    ObjectFactory* pObjFactory = m_pGame->getObjectFactory();
    
    // Crate info. 
    CrateInfo cInfo;
	cInfo.m_Width  = 0.4f;
	cInfo.m_Height = 0.3f;
	
    // Create a list of Crates
	int nColumns = 6;
	int nObjects = nColumns*(nColumns+1)/2;
    GameObject *pList[nObjects];
    GameObject* pCurrent = 0;
	
    // Request for Crates from the Object Factory
    for( int i=0; i<nObjects; i++ )
    {
		GameObject* pObject = pObjFactory->createObject(cInfo);
		pList[i] = pObject;
		pList[i]->m_pPrev = pCurrent;
		if( pCurrent )
            pCurrent->m_pNext = pList[i];
		pList[i]->m_pNext = 0;
		pCurrent = pList[i];
    }
    
    // Place Crates in the Game
    placeCrates(m_pGame,pList[0], nColumns, cInfo.m_Width, cInfo.m_Height);
	
	for( int i=0; i<nObjects; i++ )
	{
		GameObject* pObject = pList[i];
		add(pObject);
	}
}

void ObjectManager::update(float dt)
{
	cleanup();

    manageObjects();
    GameObject* pList = m_pObjectList;
    while (pList) {
        pList->update(dt);
        pList = pList->m_pNext;
    }	
	cullObjects();
}

ObjectManager::~ObjectManager()
{
    GameObject* pList = m_pObjectList;
    while (pList) {
        m_pGame->getObjectFactory()->destroyObject(pList);
        pList = pList->m_pNext;
    }
    m_pObjectList = 0;
}

void ObjectManager::markObjectsForCleanup(GameObject* pObject)
{
	m_SetMarkedObjects.insert(pObject);
}

void ObjectManager::cleanup()
{
	while(m_SetMarkedObjects.size() > 0) 
	{
		GameObject* pObject = *(m_SetMarkedObjects.begin());
		m_SetMarkedObjects.erase(m_SetMarkedObjects.begin());
		remove(pObject);
		m_pGame->getObjectFactory()->destroyObject(pObject);
	}
}

void ObjectManager::add(GameObject* pObject)
{
    pObject->m_pPrev = 0;
    pObject->m_pNext = m_pObjectList;
    
    if( m_pObjectList )
    {
        m_pObjectList->m_pPrev = pObject;
    }
    m_pObjectList = pObject;
}

void ObjectManager::remove(GameObject* pObject)
{
    if( pObject->m_pPrev )
	{
		pObject->m_pPrev->m_pNext = pObject->m_pNext;
	}
	else 
	{
		m_pObjectList = pObject->m_pNext;
	}
	if( pObject->m_pNext )
	{
		pObject->m_pNext->m_pPrev = pObject->m_pPrev;
	}
}