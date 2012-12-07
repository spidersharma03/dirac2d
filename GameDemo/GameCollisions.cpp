/*
 *  GameCollisions.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 16/11/12.
 *
 */
#include "GameCollisions.h"
#include "FirstGame.h"
#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "../Dirac2D/Dirac2D.h"


GameCollisionListener::GameCollisionListener(FirstGame* pGame) : m_pGame( pGame )
{
    m_pGame->getPhysicalWorld()->setCollisionListener(this);
}

// Called when two Physical Shapes begin to touch
void GameCollisionListener::onCollisionEnter(PhysicalShape* pShape1, PhysicalShape* pShape2, ContactManifold& manifold)
{
    GameObject* pObject1 = (GameObject*)pShape1->getUserData();
    GameObject* pObject2 = (GameObject*)pShape2->getUserData();
    
    if( pObject1 )
    {
        if( pObject1->getGameObjectInfo().m_ObjectType == EOT_COIN  &&
           pObject2 == 0 )
        {
            m_pGame->getObjectManager()->markObjectsForCleanup(pObject1);
        }
    }
    if( pObject2 )
    {
        if( pObject2->getGameObjectInfo().m_ObjectType == EOT_COIN 
		   && pObject1 == 0)
        {
            m_pGame->getObjectManager()->markObjectsForCleanup(pObject2);
        } 
    }
}

// Called when two Physical Shapes are in contact
void GameCollisionListener::onCollision(PhysicalShape* pShape1, PhysicalShape* pShape2, ContactManifold& manifold)   
{
}

// Called when two Physical Shapes begin to seperate
void GameCollisionListener::onCollisionExit(PhysicalShape* pShape1, PhysicalShape* pShape2, ContactManifold& manifold)
{
    
}