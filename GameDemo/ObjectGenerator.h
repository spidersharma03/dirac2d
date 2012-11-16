/*
 *  ObjectGenerator.h
 *  Dirac2D
 *
 *  Created by Prashant on 16/11/12.
 *
 */

#include "IUpdatable.h"

#ifndef _OBJECT_GENERATOR_H_
#define _OBJECT_GENERATOR_H_

#include "../Dirac2D/Dirac2D.h"

class FirstGame;

USE_NAMESPACE_DIRAC2D

#define MAX_COINS_ON_SCREEN 30

class ObjectGenerator : public IUpdatable
{
public:
	
	ObjectGenerator(FirstGame* pGame);
	
	void generateCoins( Vector2f* positions, int numCoins );
	
	virtual void update(float dt);
	
private:
	
	FirstGame* m_pGame;
	
	vector<PhysicalBody*> m_vecPhysicalBodies;
};

#endif