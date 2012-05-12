/*
 *  CollisionManager.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 30/04/12.
 *
 */

#include "../dynamics/PhysicalWorld.h"
#include "CollisionManager.h"
#include "CollisionDetection.h"
#include "../dynamics/PhysicalBody.h"
#include "../dynamics/PhysicalShape.h"

BEGIN_NAMESPACE_DIRAC2D

CollisionManager::CollisionManager(PhysicalWorld* world) : m_PhysicalWorld(world)
{
}
	
void CollisionManager::update()
{
	// clear Contact list.
	m_NumContacts = 0;
	
	// n2 Collision test
	for( duint32 b1=0; b1<m_PhysicalWorld->m_vecPhysicalBodies.size(); b1++ )
	{
		PhysicalBody* body1 = m_PhysicalWorld->m_vecPhysicalBodies[b1];
		for( duint32 b2=b1; b2<m_PhysicalWorld->m_vecPhysicalBodies.size(); b2++ )
		{
			PhysicalBody* body2 = m_PhysicalWorld->m_vecPhysicalBodies[b2];

			if( body1 == body2 )continue;
			
			ContactManifold manifold;
			dbool res = intersectShapes( body1->m_PhysicalShapeList->m_CollisionShape, body1->m_Transform, body2->m_PhysicalShapeList->m_CollisionShape, body2->m_Transform, &manifold);
			if( res )
			{
				for( dint32 i=0; i<manifold.m_NumContacts; i++ )
				{
					Contact& contact = m_ContactList[m_NumContacts];
					contact.m_Manifold = manifold;
					contact.m_PhysicalShape1 = body1->m_PhysicalShapeList;
					contact.m_PhysicalShape2 = body2->m_PhysicalShapeList;
					contact.m_ContactPoint = manifold.m_ContactPoints[i];
					contact.m_ContactNormal = manifold.m_ContactNormal;
					m_NumContacts++;
				}
			}
		}
	}
}

END_NAMESPACE_DIRAC2D