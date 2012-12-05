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
class GameObjectList;

class ObjectPlacementStraregy 
{
public:
	ObjectPlacementStraregy( FirstGame* pGame )
	{
		m_pGame = pGame;
	}
	
	virtual void placeObjects(GameObjectList* pList, int numObjects) = 0;
	
protected:
	FirstGame* m_pGame;
};

#endif