/*
 *  NaiveBoradPhaseCollisionAlgorithm.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 01/06/12.
 *
 */

#include "NaiveBoradPhaseCollisionAlgorithm.h"
#include "../CollisionManager.h"
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

// Adds a Physical Shape to the Broad Phase Node List
void NaiveBroadPhaseCollisionAlgorithm::addBroadPhaseNode(BroadPhaseNode* pBroadPhaseNode)
{
	pBroadPhaseNode->m_Prev = 0;
	pBroadPhaseNode->m_Next = m_BroadPhaseNodeList;
	
	if( m_BroadPhaseNodeList )
	{
		m_BroadPhaseNodeList->m_Prev = pBroadPhaseNode;
	}
	m_BroadPhaseNodeList = pBroadPhaseNode;
}

// Removes a Proxy from the Broad Phase Node List
void NaiveBroadPhaseCollisionAlgorithm::removeBroadPhaseNode(BroadPhaseNode* pBroadPhaseNode)
{
	BroadPhaseNode* prevNode = pBroadPhaseNode->m_Prev;
	BroadPhaseNode* nextNode = pBroadPhaseNode->m_Next;
	
	if( prevNode )
	{
		prevNode->m_Next = nextNode;
	}
	else 
	{
		m_BroadPhaseNodeList = nextNode;
	}
	
	if( nextNode )
	{
		nextNode->m_Prev = prevNode;
	}
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
			
			AABB2f& aabb1 = pNode1->m_CollisionShape->getAABB();
			AABB2f& aabb2 = pNode2->m_CollisionShape->getAABB();
			
			bRes = aabb1.intersectAABB(aabb2);
			
			if( bRes  && (contactPairPool.insert(ContactPair(pNode1->m_CollisionShape, pNode2->m_CollisionShape) )).second )
			{
				m_pCollisionManager->addContactPair(pNode1, pNode2);
			}
			pNode2 = pNode2->m_Next;
		}  		
		pNode1 = pNode1->m_Next;
	} 
}


void NaiveBroadPhaseCollisionAlgorithm::overlapAABB( AABB2f& queryAABB, OverlapCallBackClass* callBack )
{
	BroadPhaseNode* pNode = m_BroadPhaseNodeList;
	while( pNode )
	{									
		if( queryAABB.intersectAABB(pNode->m_CollisionShape->getAABB()) )
			callBack->overlapCallBack(pNode->m_ID);

		pNode = pNode->m_Next;
	} 
}

void NaiveBroadPhaseCollisionAlgorithm::intersectRay( RayIntersectionCallBackClass* callBack)
{
}

END_NAMESPACE_DIRAC2D