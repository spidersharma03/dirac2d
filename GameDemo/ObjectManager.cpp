/*
 *  ObjectGenerator.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 04/12/12.
 *
 */

#include "Dirac2D.h"
#include "ObjectManager.h"
#include "FirstGame.h"
#include "Camera.h"
#include "TerrainGenerator.h"
#include "ObjectFactory.h"
#include "Coin.h"
#include "Crate.h"
#include "Tumbler.h"
#include "PathHurdle.h"
#include "CableCar.h"
#include "ObjectPlacementSrategy.h"

ObjectManager::ObjectManager(FirstGame* pGame)
{
	m_pGame = pGame;
    m_pObjectList = 0;
	m_bCableCarExisting = false;
    //m_SetMarkedObjects.reserve(100);
}

void ObjectManager::manageObjects()
{    
	static double initTime = m_Timer.getCurrentTime();
    double time = m_Timer.getCurrentTime();
    if( time - initTime > 2000 )
    {
        initTime = time;
        generateCoins();
		generatePathHurdle();
	} 
	
	if( !m_bCableCarExisting )
	{
		static double initTime = m_Timer.getCurrentTime();
		double time = m_Timer.getCurrentTime();
		if( time - initTime > 2000 )
		{
			m_bCableCarExisting = true;
			initTime = time;
			spawnCableCar();
		} 
	}
	// Place Falling Crates only for Non Linear regions
	if( m_pGame->getTerrainGenerator()->getSampleFunctionType() != ESFT_LINEAR )
	{
		static double initTime = m_Timer.getCurrentTime();
		double time = m_Timer.getCurrentTime();
		if( time - initTime > 3000 )
		{
			initTime = time;
			generateFallingCrates();
		}
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
			generateTumblers();
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
    
    while (pList) 
    {
        bool bRes =  (pCamera->getPosition().x - pList->getPosition().x) > R;
        if( bRes && pList->getGameObjectInfo().m_bAutoCull )
        {
            remove(pList);
            pObjFactory->destroyObject(pList);
        }
        pList = pList->m_pNext;
    }  
}

void ObjectManager::generatePathHurdle()
{
	if( PathHurdle::m_PathHurdleCount > 1 )
		return;
	
	ObjectFactory* pObjFactory = m_pGame->getObjectFactory();
    
    // PathHurdle info. 
    PathHurdleInfo pInfo;
    pInfo.m_Radius = RANDOM_NUMBER( 0.25f, 0.4f);
	pInfo.m_NumVertices = RANDOM_NUMBER(3.0f, 7.0f);
	
	TerrainGenerator* pTerrainGenerator = m_pGame->getTerrainGenerator();
	float* pCurvePoints;
    int nCurvePoints = pTerrainGenerator->getNumTerrainPoints();
	pCurvePoints = pTerrainGenerator->getCurvePointsArray();
	int n = 2*nCurvePoints-40;
	
	//Vector3f cameraPos = m_pGame->getCamera()->getPosition();
	//float sw = m_pGame->getCamera()->getScreenWidth();	
	//Vector2f(pCurvePoints[n], pCurvePoints[n+1] + 0.4)
	
	pInfo.m_Position = Vector2f(pCurvePoints[n], pCurvePoints[n+1] + 0.2);
	
	GameObject* pObject = pObjFactory->createObject(pInfo);
	
	add(pObject);
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
	cInfo.m_Width  = 0.6f;
	cInfo.m_Height = 0.4f;
	cInfo.m_Density = AVG_OBJECT_DENSITY * 10.0f;
	
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

void ObjectManager::generateFallingCrates()
{
    ObjectFactory* pObjFactory = m_pGame->getObjectFactory();
    
    // Create a list of Crates
	int nObjects = 10;
    GameObject *pList[nObjects];
    GameObject* pCurrent = 0;
	
    // Request for Crates from the Object Factory
    for( int i=0; i<nObjects; i++ )
    {
		// Crate info. 
		CrateInfo cInfo;
		cInfo.m_Density = AVG_OBJECT_DENSITY * 10.0f;
		cInfo.m_ShapeType = ECS_REGULAR_POLY;
		cInfo.m_NumVertices = RANDOM_NUMBER(3.0f, 6.0f);
		cInfo.m_Radius  = RANDOM_NUMBER(0.4f, 1.0f);
		cInfo.m_AngularVelocity = RANDOM_NUMBER(-12.0f, 12.0f);
		
		GameObject* pObject = pObjFactory->createObject(cInfo);
		pList[i] = pObject;
		pList[i]->m_pPrev = pCurrent;
		if( pCurrent )
            pCurrent->m_pNext = pList[i];
		pList[i]->m_pNext = 0;
		pCurrent = pList[i];
    }
    
    // Place Crates in the Game
    placeFallingCrates(m_pGame,pList[0], nObjects, 0.6f, 0.6f);
	
	for( int i=0; i<nObjects; i++ )
	{
		GameObject* pObject = pList[i];
		add(pObject);
	}
}

void ObjectManager::generateTumblers()
{
	if( Tumbler::m_TumblerCount > 1 )
		return;
	
	ObjectFactory* pObjFactory = m_pGame->getObjectFactory();
	
	TumblerInfo tInfo;
	tInfo.m_Width = 2.5f;
	tInfo.m_Height = 2.0f;
    tInfo.m_NumObjects = 15;
	
	Vector3f cameraPos = m_pGame->getCamera()->getPosition();
	float sw = m_pGame->getCamera()->getScreenWidth();
	float sh = m_pGame->getCamera()->getScreenHeight() * 0.5f;
	
	tInfo.m_Position = Vector2f( cameraPos.x + sw, cameraPos.y + sh);
		
	GameObject* pObject = pObjFactory->createObject(tInfo);
	
	add(pObject);
}

void ObjectManager::spawnCableCar()
{
	ObjectFactory* pObjFactory = m_pGame->getObjectFactory();
	
	Vector3f cameraPos = m_pGame->getCamera()->getPosition();
	float sw = m_pGame->getCamera()->getScreenWidth();
	float sh = m_pGame->getCamera()->getScreenHeight() * 0.5f;
	
	CableCarInfo cInfo;
	cInfo.m_FixedPoint1 = Vector2f( cameraPos.x - 1.5*sw, cameraPos.y + sh);
	cInfo.m_FixedPoint2 = Vector2f( cameraPos.x + 4*sw, cameraPos.y - 0.5*sh);
	cInfo.m_FixedLength = cInfo.m_FixedPoint1.distance(cInfo.m_FixedPoint2) + 0.1f;
	
	cInfo.m_Position = Vector2f( cameraPos.x - sw , cameraPos.y + sh);
	
	GameObject* pObject = pObjFactory->createObject(cInfo);
	
	add(pObject);
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