/*
 *  ObjectGenerator.h
 *  Dirac2D
 *
 *  Created by Prashant on 04/12/12.
 *
 */

#ifndef _OBJECT_GENERATOR_H_
#define _OBJECT_GENERATOR_H_

#include "../Dirac2D/Dirac2D.h"

USE_NAMESPACE_DIRAC2D

class FirstGame;

class ObjectGenerator
{
public:
	ObjectGenerator(FirstGame* pGame);
	
	// Generate/Destroy Objects
	void manageObjects();
	
private:
	FirstGame* m_pGame;
	DTimer m_Timer;
};

#endif