/*
 *  GameCollisions.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 16/11/12.
 *
 */
#include "GameCollisions.h"
#include "FirstGame.h"
#include "../Dirac2D/Dirac2D.h"

GameCollisionListener::GameCollisionListener(FirstGame* pGame) : m_pGame( pGame )
{
    m_pGame->getPhysicalWorld()->setCollisionListener(this);
}

// Called when two Physical Shapes begin to touch
void GameCollisionListener::onCollisionEnter(PhysicalShape* pShape1, PhysicalShape* pShape2, ContactManifold& manifold)
{
    void* userData1 = pShape1->getUserData();
    void* userData2 = pShape2->getUserData();
    
    if( userData1 )
    {
        
    }
    if( userData2 )
    {
        
    }
    if( ( !pShape1->isSensor() && pShape2->isSensor() ) ||  ( pShape1->isSensor() && !pShape2->isSensor() ) )
    {
    }
}

// Called when two Physical Shapes are in contact
void GameCollisionListener::onCollision(PhysicalShape* pShape1, PhysicalShape* pShape2, ContactManifold& manifold)   
{
}

// Called when two Physical Shapes begin to seperate
void GameCollisionListener::onCollisionExit(PhysicalShape* pShape1, PhysicalShape* pShape2, ContactManifold& manifold)
{
    if( ( !pShape1->isSensor() && pShape2->isSensor() ) ||  ( pShape1->isSensor() && !pShape2->isSensor() ) )
    {
        
    }
}