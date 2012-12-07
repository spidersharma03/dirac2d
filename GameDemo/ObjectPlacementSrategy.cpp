/*
 *  ObjectPlacementSrategy.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 07/12/12.
 *
 */

#include "ObjectPlacementSrategy.h"
#include "CoinPlacementStrategy.h"
#include "TerrainGenerator.h"
#include "Coin.h"
#include "Crate.h"
#include "../Dirac2D/Dirac2D.h"
#include "FirstGame.h"

USE_NAMESPACE_DIRAC2D

void placeCoins(FirstGame* pGame ,GameObject* pList, int numObjects)
{
    dAssert( (pList->getGameObjectInfo().m_ObjectType == EOT_COIN ));
    
	TerrainGenerator* pTerrainGenerator = pGame->getTerrainGenerator();
	float* pCurvePoints;
    int nCurvePoints = pTerrainGenerator->getNumTerrainPoints();
	pCurvePoints = pTerrainGenerator->getCurvePointsArray();
	Vector2f coinPos[20];
	int n = 2*nCurvePoints-40;
	for (int i=0; i<2*numObjects; i+=2) {
		coinPos[i/2] = Vector2f(pCurvePoints[n], pCurvePoints[n+1] - 0.4);
		n += 2;
	}
	int i = 0;
	while (pList) 
	{
		Coin* pCoin = (Coin*)pList;
		pCoin->getPhysicalBody()->setPosition(coinPos[i]);
		i++;
        pList = pList->m_pNext;
    }
}

// Place Crates in a Stack
void placeCrates(FirstGame* pGame ,GameObject* pList, int nColumns, float w, float h)
{
	dAssert( (pList->getGameObjectInfo().m_ObjectType == EOT_CRATE ));
    
	int numObjects = nColumns*(nColumns+1)/2;
	
	dAssert( numObjects < 50 );
	
	TerrainGenerator* pTerrainGenerator = pGame->getTerrainGenerator();
	float* pCurvePoints;
    int nCurvePoints = pTerrainGenerator->getNumTerrainPoints();
	pCurvePoints = pTerrainGenerator->getCurvePointsArray();
	
	int n = 2*nCurvePoints-20;
	Vector2f start = Vector2f( pCurvePoints[n], pCurvePoints[n+1] );
	
	Vector2f cratePos[50];
		
	int l = 0;
	int m = nColumns;
	float x = start.x;
	float y = start.y + 0.1f;
	float eps = w/10;
	
	// Crate Placement
	for( int i=0; i<nColumns; i++ )
	{
		x = start.x + i * w/2;
		
		for( int j=0; j<m; j++ )
		{
			cratePos[l++] = Vector2f(x,y);
			x += w + eps;
		}
		m--;
		y += h + eps;
	}
	
	int i = 0;
		
	while (pList) 
	{
		Crate* pCrate = (Crate*)pList;
		pCrate->getPhysicalBody()->setPosition(cratePos[i]);
		i++;
        pList = pList->m_pNext;
    }
}