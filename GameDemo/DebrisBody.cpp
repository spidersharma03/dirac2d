//
//  DebrisBody.cpp
//  Dirac2D
//
//  Created by Prashant Sharma on 14/12/12.
//


#include "DebrisBody.h"
#include "FirstGame.h"
#include "ObjectManager.h"

FirstGame* DebrisBody::m_pGame = 0;
int DebrisBody::m_DebrisCount = 0;

DebrisBody::DebrisBody(DebrisInfo dInfo, FirstGame* pGame)
{
    m_ObjectInfo.m_ObjectType = EOT_DEBRIS;
    m_pGame = pGame;
    
    PhysicalAppearance pApp;
    pApp.m_PhysicalAttributes.m_Restitution = 0.5f;
	pApp.m_MassAttributes.m_Density = AVG_OBJECT_DENSITY;
	m_pBody = m_pGame->getPhysicalWorld()->createPhysicalBody();
    m_pBody->setPosition(dInfo.m_Position);
    m_pBody->setVelocity(dInfo.m_Velocity);
    m_pBody->setAngularVelocity(dInfo.m_AngularVelocity);
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(dInfo.vertices, dInfo.numVertices);

	PhysicalShape* pShape = m_pBody->createPhysicalShape(pApp);
    pShape->setUserData(this);
    pShape->m_CollisionFilter.m_CollisionBit = EOCB_PARTICLE_DEBRIS;
    pShape->m_CollisionFilter.m_CollisionMask = EOCB_PARTICLE_DEBRIS | EOCB_PHYSICAL_BODY;  
    m_BounceCount = 0;
    m_DebrisCount++;
}

void DebrisBody::update(float dt)
{
    m_Position = m_pBody->m_Position;
}

void DebrisBody::onCollisionEnter( GameObject* pObject, ContactManifold& manifold )
{
    if( !pObject ) 
    {
        m_BounceCount++;
        if( m_BounceCount > 3 )
        {
            m_pGame->getObjectManager()->markObjectsForCleanup(this);
        }
    }
}

void DebrisBody::onCollisionExit( GameObject* pObject, ContactManifold& manifold )
{
    
}

DebrisBody::~DebrisBody()
{
    m_pGame->getPhysicalWorld()->deletePhysicalBody(m_pBody);
    m_pBody = 0;
    m_DebrisCount--;
}