/*
 *  GameCollisions.h
 *  Dirac2D
 *
 *  Created by Prashant on 16/11/12.
 *
 */

#include "../Dirac2D/CallBacks.h"

USE_NAMESPACE_DIRAC2D

#ifndef _GAME_COLLISIONS_H_
#define _GAME_COLLISIONS_H_

class FirstGame;

class GameCollisionListener : public ICollisionLisetner
{
public:
	
	GameCollisionListener(FirstGame* pGame);
	
	// Called when two Physical Shapes begin to touch
    virtual void onCollisionEnter(PhysicalShape*, PhysicalShape*, ContactManifold&);    
	
    // Called when two Physical Shapes are in contact
    virtual void onCollision(PhysicalShape*, PhysicalShape*, ContactManifold&);    
	
    // Called when two Physical Shapes begin to seperate
    virtual void onCollisionExit(PhysicalShape*, PhysicalShape*, ContactManifold&);  
	
private:
	FirstGame* m_pGame;
};

#endif
