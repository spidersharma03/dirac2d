/*
 *  GameCollisions.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 16/11/12.
 *
 */
#include "GameCollisions.h"
#include "FirstGame.h"

GameCollisionListener::GameCollisionListener(FirstGame* pGame) : m_pGame( pGame )
{
}

// Called when two Physical Shapes begin to touch
void GameCollisionListener::onCollisionEnter(PhysicalShape* pShape1, PhysicalShape* pShape2, ContactManifold& manifold)
{
}

// Called when two Physical Shapes are in contact
void GameCollisionListener::onCollision(PhysicalShape* pShape1, PhysicalShape* pShape2, ContactManifold& manifold)   
{
}

// Called when two Physical Shapes begin to seperate
void GameCollisionListener::onCollisionExit(PhysicalShape* pShape1, PhysicalShape* pShape2, ContactManifold& manifold)
{
}