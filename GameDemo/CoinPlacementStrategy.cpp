/*
 *  CoinPlacementStrategy.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 04/12/12.
 *
 */

#include "../Dirac2D/Dirac2D.h"
#include "FirstGame.h"
#include "CoinPlacementStrategy.h"
#include "TerrainGenerator.h"
#include "Coin.h"

CoinPlacementStraregy::CoinPlacementStraregy( FirstGame* pGame ) : ObjectPlacementStraregy( pGame )
{
}


void CoinPlacementStraregy::placeObjects(GameObjectList* pList, int numObjects)
{
	TerrainGenerator* pTerrainGenerator = m_pGame->getTerrainGenerator();
	float* pCurvePoints;
	int nCurvePoints = pTerrainGenerator->getCurvePointsArray(pCurvePoints);
	Vector2f coinPos[20];
	int n = 2*nCurvePoints-40;
	for (int i=0; i<2*numObjects; i+=2) {
		coinPos[i/2] = Vector2f(pCurvePoints[n], pCurvePoints[n+1] - 0.4);
		n += 2;
	}
	int i = 0;
	while (pList->m_pNext) 
	{
		Coin* pCoin = (Coin*)pList->m_pObject;
		pCoin->getPhysicalBody()->setPosition(coinPos[i]);
		i++;
	}
}