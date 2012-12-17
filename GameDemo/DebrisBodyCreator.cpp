//
//  DebrisBodyCreator.cpp
//  Dirac2D
//
//  Created by Prashant Sharma on 15/12/12.
//

#include "DebrisBodyCreator.h"
#include "FirstGame.h"
#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "Crate.h"
#include "DebrisBody.h"

DebrisInfo m_DebrisInfoArray[10];

DebrisBodyCreator::DebrisBodyCreator(FirstGame* pGame)
{
    m_pGame = pGame;
    
    float size = 0.1f;
    for (int i=0; i<10; i++) 
    {
        float theta = 0.0f;
        int numSides = RANDOM_NUMBER(3.0f, 5.0f);
        float dTheta = 2*PI/(numSides+1);
        m_DebrisInfoArray[i].numVertices = numSides;
        size = RANDOM_NUMBER(0.06, 0.2);
        for (int j=0; j<numSides; j++) 
        {
            m_DebrisInfoArray[i].vertices[j] = Vector2f(size*cos(theta), size*sin(theta));
            theta += dTheta + RANDOM_NUMBER(PI/15.0f, PI/10.0f);
        }
    }

}

void DebrisBodyCreator::createDebrisBodies( const GameObjectInfo& gInfo, const Vector2f& position, int numBodies )
{
    switch (gInfo.m_ObjectType) 
    {
        case EOT_CRATE:
            createCrateDebris(position, numBodies);
            break;
            
        default:
            break;
    }
}


void DebrisBodyCreator::createCrateDebris( const Vector2f& position, int numBodies )
{
    for( int i=0; i<numBodies; i++ )
    {
        DebrisInfo debrisInfo;
        int n = RANDOM_NUMBER(1.0f, 10.0f);
        debrisInfo = m_DebrisInfoArray[n];
        float x = RANDOM_NUMBER(-1.0f, 1.0f);
        float y = RANDOM_NUMBER(0.3f, 1.0f);
        debrisInfo.m_Position = position;
        debrisInfo.m_Velocity = Vector2f(x,y);
        debrisInfo.m_Velocity *= RANDOM_NUMBER(10.0f, 15.0f);
        debrisInfo.m_AngularVelocity = RANDOM_NUMBER(-5.0f,5.0f);
        GameObject* pObject = m_pGame->getObjectFactory()->createObject(debrisInfo);
        m_pGame->getObjectManager()->add(pObject);
    }
}
