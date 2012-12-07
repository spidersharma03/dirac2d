/*
 *  CoinPlacementStrategy.h
 *  Dirac2D
 *
 *  Created by Prashant on 04/12/12.
 *
 */

#ifndef _COIN_PLACEMENT_STRATEGY_H_
#define _COIN_PLACEMENT_STRATEGY_H_

#include "ObjectPlacementSrategy.h"

class CoinPlacementStraregy : public ObjectPlacementStraregy 
{
public:
	CoinPlacementStraregy( FirstGame* pGame );
	
	virtual void placeObjects(GameObject* pList, int numObjects);
	
};

#endif