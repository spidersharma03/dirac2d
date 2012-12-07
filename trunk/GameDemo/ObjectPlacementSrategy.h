/*
 *  ObjectPlacementSrategy.h
 *  Dirac2D
 *
 *  Created by Prashant on 04/12/12.
 *
 */

#ifndef _OBJECT_PLACEMENT_STRATEGY_H_
#define _OBJECT_PLACEMENT_STRATEGY_H_

class FirstGame;
class GameObject;


void placeCoins(FirstGame* pGame ,GameObject* pList, int numObjects);

void placeCrates(FirstGame* pGame ,GameObject* pList, int numObjects, float w, float h);

class ObjectPlacementStraregy 
{
public:
	ObjectPlacementStraregy( FirstGame* pGame )
	{
		m_pGame = pGame;
	}
	
	virtual void placeObjects(GameObject* pList, int numObjects) = 0;
	
protected:
	FirstGame* m_pGame;
};

#endif