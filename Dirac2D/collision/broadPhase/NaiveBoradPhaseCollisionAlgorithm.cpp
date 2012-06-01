/*
 *  NaiveBoradPhaseCollisionAlgorithm.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 01/06/12.
 *
 */

#include "NaiveBoradPhaseCollisionAlgorithm.h"
#include "CollisionManager.h"
#include "../../dynamics/PhysicalShape.h"
#include "../../dynamics/PhysicalBody.h"
#include "../../geometry/CollisionShape.h"
#include "../../dynamics/contacts/Contact.h"
#include <set>

using namespace std;

BEGIN_NAMESPACE_DIRAC2D

NaiveBroadPhaseCollisionAlgorithm::NaiveBroadPhaseCollisionAlgorithm(CollisionManager* collisionManager):
BroadPhaseCollisionAlgorithm(collisionManager)
{
}

void NaiveBroadPhaseCollisionAlgorithm::update()
{
	set<ContactPair>& contactPairPool = m_pCollisionManager->getContactPairPool();
	// n2 Collision test
	BroadPhaseNode* pNode1 = m_BroadPhaseNodeList;
	while( pNode1 )
	{
		PhysicalShape* pShape1 = pNode1->m_PhysicalShape;
		PhysicalBody* pBody1 = pShape1->m_ParentBody;
		BroadPhaseNode* pNode2 = pNode1;
		while( pNode2 )
		{
			PhysicalShape* pShape2 = pNode2->m_PhysicalShape;
			PhysicalBody* pBody2 = pShape2->m_ParentBody;
			if( pBody1 == pBody2 )
			{
				pNode2 = pNode2->m_Next;
				continue;
			}
			
			if( pBody1->m_BodyType == EBT_STATIC && pBody2->m_BodyType == EBT_STATIC )
			{
				pNode2 = pNode2->m_Next;
				continue;
			}
			
			dbool bRes = true;
			
			AABB2f& aabb1 = pShape1->m_CollisionShape->getAABB();
			AABB2f& aabb2 = pShape2->m_CollisionShape->getAABB();
			
			bRes = aabb1.intersectAABB(aabb2);
			
			if( bRes  && (contactPairPool.insert(ContactPair(pShape1, pShape2) )).second )
			{
				m_pCollisionManager->addContact(pShape1, pShape2);
			}
			pNode2 = pNode2->m_Next;
		}  		
		pNode1 = pNode1->m_Next;
	} 
}

END_NAMESPACE_DIRAC2D