/*
 *  NaiveBoradPhaseCollisionAlgorithm.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 01/06/12.
 *
 */

#include "collision/broadPhase/NaiveBoradPhaseCollisionAlgorithm.h"
#include "collision/CollisionManager.h"
#include "dynamics/PhysicalShape.h"
#include "dynamics/PhysicalBody.h"
#include "geometry/CollisionShape.h"
#include "dynamics/contacts/Contact.h"
#include <string>

BEGIN_NAMESPACE_DIRAC2D

NaiveBroadPhaseCollisionAlgorithm::NaiveBroadPhaseCollisionAlgorithm(CollisionManager* collisionManager):
BroadPhaseCollisionAlgorithm(collisionManager)
{
	m_AlgorithmType = EAT_NAIVE;
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
	m_PairCount = 0;
	BroadPhaseNode* pNode1 = m_BroadPhaseNodeList;
	while( pNode1 )
	{
		pNode1->m_AABB = pNode1->m_CollisionShape->getAABB();
		pNode1 = pNode1->m_Next;
	}
	// n2 Collision test
	pNode1 = m_BroadPhaseNodeList;
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
			
			if( pBody1->m_BodyType != EBT_DYNAMIC && pBody2->m_BodyType != EBT_DYNAMIC )
			{
				pNode2 = pNode2->m_Next;
				continue;
			}
			
			dbool bRes = true;
			
			bRes = pNode1->m_AABB.intersectAABB(pNode2->m_AABB);
			
			if( bRes )
			{
				if( m_PairCount >= m_PairCapacity )
				{
					m_PairCapacity *= 2;
					BroadPhasePair* newPairs = (BroadPhasePair*)malloc(m_PairCapacity*sizeof(BroadPhasePair));
					memcpy(newPairs, m_Pairs, m_PairCount * sizeof(BroadPhasePair));
					free(m_Pairs);
					m_Pairs = newPairs;
				}
				m_Pairs[m_PairCount].m_Node1 = pNode1;
				m_Pairs[m_PairCount].m_Node2 = pNode2;
				m_PairCount++;
				
			}
			pNode2 = pNode2->m_Next;
		}  		
		pNode1 = pNode1->m_Next;
	} 
	
	for( dint32 p=0; p<m_PairCount; p++ )
	{
		BroadPhasePair& pair = m_Pairs[p];
		m_pCollisionManager->addContactPair(pair.m_Node1, pair.m_Node2);
	}
}


void NaiveBroadPhaseCollisionAlgorithm::overlapAABB( AABB2f& queryAABB, OverlapCallBackClass* callBack )
{
	BroadPhaseNode* pNode = m_BroadPhaseNodeList;
	while( pNode )
	{									
		if( queryAABB.intersectAABB(pNode->m_CollisionShape->getAABB()) )
			callBack->overlapCallBack(pNode->m_ID, pNode);

		pNode = pNode->m_Next;
	} 
}

void NaiveBroadPhaseCollisionAlgorithm::intersectRay(const Ray2f& ray, RayIntersectionCallBackClass* callBack)
{
	BroadPhaseNode* pNode = m_BroadPhaseNodeList;
	while( pNode )
	{									
		//if( pNode->m_CollisionShape->intersectRay(ray) )
		//	callBack->rayIntersectionCallBack(pNode->m_ID);
		
		pNode = pNode->m_Next;
	} 
}

void NaiveBroadPhaseCollisionAlgorithm::intersectRaySegment(const RaySegment2f& raySegment, RayIntersectionCallBackClass* callBack)
{
	BroadPhaseNode* pNode = m_BroadPhaseNodeList;
	while( pNode )
	{									
		//if( pNode->m_CollisionShape->intersectRay(raySegment) )
			callBack->rayIntersectionCallBack(raySegment, pNode->m_ID);
		
		pNode = pNode->m_Next;
	} 
}

END_NAMESPACE_DIRAC2D