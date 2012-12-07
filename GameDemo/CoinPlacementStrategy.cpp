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


void CoinPlacementStraregy::placeObjects(GameObject* pList, int numObjects)
{
    dAssert( (pList->getGameObjectInfo().m_ObjectType == EOT_COIN ));
    
	TerrainGenerator* pTerrainGenerator = m_pGame->getTerrainGenerator();
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