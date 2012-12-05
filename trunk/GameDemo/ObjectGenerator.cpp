/*
 *  ObjectGenerator.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 04/12/12.
 *
 */

#include "../Dirac2D/Dirac2D.h"
#include "ObjectGenerator.h"
#include "FirstGame.h"

ObjectGenerator::ObjectGenerator(FirstGame* pGame)
{
	m_pGame = pGame;
}

void ObjectGenerator::manageObjects()
{
	static double initTime = m_Timer.getCurrentTime();
    double time = m_Timer.getCurrentTime();
    if( time - initTime > 2000 )
    {
        initTime = time;
	}
}

