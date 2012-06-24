/*
 *  DynamicTreeBroadPhaseAlgorithm.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 15/06/12.
 *
 */

#include "../CollisionManager.h"
#include "../../dynamics/PhysicalShape.h"
#include "../../dynamics/PhysicalBody.h"
#include "../../geometry/CollisionShape.h"
#include "../../dynamics/contacts/Contact.h"

#include "DynamicTreeBroadPhaseAlgorithm.h"
#include "DynamicTree.h"
#include <algorithm>

using namespace std;


BEGIN_NAMESPACE_DIRAC2D

	
DynamicTreeBroadPhaseAlgorithm::DynamicTreeBroadPhaseAlgorithm(CollisionManager* collisionManager):
BroadPhaseCollisionAlgorithm(collisionManager)
{
	m_DynamicTree = new DynamicTree();
	m_AlgorithmType = EAT_DYNAMIC_TREE;
}
	
void DynamicTreeBroadPhaseAlgorithm::addBroadPhaseNode(BroadPhaseNode* pBroadPhaseNode)
{
	pBroadPhaseNode->m_Prev = 0;
	pBroadPhaseNode->m_Next = m_BroadPhaseNodeList;
	
	if( m_BroadPhaseNodeList )
	{
		m_BroadPhaseNodeList->m_Prev = pBroadPhaseNode;
	}
	m_BroadPhaseNodeList = pBroadPhaseNode;
	pBroadPhaseNode->m_ID = m_DynamicTree->createProxy(pBroadPhaseNode->m_CollisionShape->getAABB(), pBroadPhaseNode);
}
	
void DynamicTreeBroadPhaseAlgorithm::removeBroadPhaseNode(BroadPhaseNode* pBroadPhaseNode)
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
	m_DynamicTree->removeProxy(pBroadPhaseNode->m_ID);
}
	

//void DynamicTreeBroadPhaseAlgorithm::update()
//{
//	// Update all the Proxy AABB's in the Dynamic Tree.
//	BroadPhaseNode* pNode = m_BroadPhaseNodeList;
//	while( pNode )
//	{
//		m_QueryID = pNode->m_ID;
//		
//		PhysicalShape* pShape = pNode->m_PhysicalShape;
//		
//		m_DynamicTree->updateProxy(pShape->m_CollisionShape->getAABB(), pNode->m_ID);
//		pNode = pNode->m_Next;
//	} 
//	
//	// Check for Overlaps with the Dynamic Tree.
//	pNode = m_BroadPhaseNodeList;
//	while( pNode )
//	{
//		m_QueryID = pNode->m_ID;
//		PhysicalShape* pShape = pNode->m_PhysicalShape;
//		m_vecOverlappedId.clear();
//		m_vecOverlappedId.reserve(10);
//		m_DynamicTree->overlapAABB(pShape->m_CollisionShape->getAABB(), m_vecOverlappedId );
//		
//		for( dint32 i=0; i<m_vecOverlappedId.size(); i++ )
//		{
//			dint32 overlapNodeID = m_vecOverlappedId[i];
//			
//			if( m_QueryID == overlapNodeID )
//				continue;
//			
//			BroadPhaseNode* pNode1 = (BroadPhaseNode*)m_DynamicTree->getUserData(m_QueryID);
//			BroadPhaseNode* pNode2 = (BroadPhaseNode*)m_DynamicTree->getUserData(overlapNodeID);
//			
//			PhysicalBody* pBody1 = pNode1->m_PhysicalShape->m_ParentBody;
//			PhysicalBody* pBody2 = pNode2->m_PhysicalShape->m_ParentBody;
//			
//			if( pBody1 == pBody2 )
//				continue;
//			
//			set<ContactPair>& contactPairPool = m_pCollisionManager->getContactPairPool();
//			
//			if( (contactPairPool.insert(ContactPair(pNode1->m_CollisionShape, pNode2->m_CollisionShape) )).second )
//			{
//				m_pCollisionManager->addContactPair(pNode1, pNode2);
//			}
//		}
//		pNode = pNode->m_Next;
//	} 
//}

void DynamicTreeBroadPhaseAlgorithm::update()
{
	m_PairCount = 0;
	// Update all the Proxy AABB's in the Broad Phase Nodes.
	BroadPhaseNode* pNode = m_BroadPhaseNodeList;
	while( pNode )
	{		
		pNode->m_AABB = pNode->m_CollisionShape->getAABB();
		pNode = pNode->m_Next;
	} 
	
	// Update all the Proxy AABB's in the Dynamic Tree.
	pNode = m_BroadPhaseNodeList;
	while( pNode )
	{			
		m_DynamicTree->updateProxy(pNode->m_AABB, pNode->m_ID);
		pNode = pNode->m_Next;
	} 
	
	// Check for Overlaps with the Dynamic Tree.
	pNode = m_BroadPhaseNodeList;
	while( pNode )
	{
		m_QueryID = pNode->m_ID;
		m_DynamicTree->overlapAABB(pNode->m_AABB, this );
		pNode = pNode->m_Next;
	} 
	
		// 
	for( dint32 p=0; p<m_PairCount; p++ )
	{
		BroadPhasePair& pair = m_Pairs[p];
		m_pCollisionManager->addContactPair(pair.m_Node1, pair.m_Node2);
	}

}


void DynamicTreeBroadPhaseAlgorithm::overlapCallBack(dint32 overlapNodeID)
{
	if( m_QueryID == overlapNodeID )
		return;
	
	BroadPhaseNode* pNode1 = (BroadPhaseNode*)m_DynamicTree->getUserData(m_QueryID);
	BroadPhaseNode* pNode2 = (BroadPhaseNode*)m_DynamicTree->getUserData(overlapNodeID);
	
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
	
//	if( pNode1->m_AABB.intersectAABB(pNode2->m_CollisionShape->getAABB()) 
//	   && (contactPairPool.insert(ContactPair(pNode1->m_CollisionShape, pNode2->m_CollisionShape) )).second )
//	{
//		m_pCollisionManager->addContactPair(pNode1, pNode2);
//	}
	
}

void DynamicTreeBroadPhaseAlgorithm::overlapAABB( AABB2f& queryAABB, OverlapCallBackClass* callBack )
{
	BroadPhaseNode* pNode = m_BroadPhaseNodeList;
	while( pNode )
	{									
		m_DynamicTree->overlapAABB(queryAABB, callBack );
		pNode = pNode->m_Next;
	} 
}

void DynamicTreeBroadPhaseAlgorithm::intersectRay( RayIntersectionCallBackClass* callBack)
{
}

END_NAMESPACE_DIRAC2D
