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
#include <set>

using namespace std;


BEGIN_NAMESPACE_DIRAC2D

	
DynamicTreeBroadPhaseAlgorithm::DynamicTreeBroadPhaseAlgorithm(CollisionManager* collisionManager):
BroadPhaseCollisionAlgorithm(collisionManager)
{
	m_DynamicTree = new DynamicTree();
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
	pBroadPhaseNode->m_ID = m_DynamicTree->createProxy(pBroadPhaseNode->m_CollisionShape->getAABB());
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
	
void DynamicTreeBroadPhaseAlgorithm::update()
{
	//set<ContactPair>& contactPairPool = m_pCollisionManager->getContactPairPool();
	BroadPhaseNode* pNode = m_BroadPhaseNodeList;
	while( pNode )
	{
		PhysicalShape* pShape = pNode->m_PhysicalShape;
				
		m_DynamicTree->intersectAABB(pShape->m_CollisionShape->getAABB(), 0);
		pNode = pNode->m_Next;
	} 
}

void DynamicTreeBroadPhaseAlgorithm::updatePair()
{
}
	

END_NAMESPACE_DIRAC2D
