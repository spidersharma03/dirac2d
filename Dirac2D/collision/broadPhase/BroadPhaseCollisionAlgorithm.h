/*
 *  BroadPhaseCollisionAlgorithm.h
 *  Dirac2D
 *
 *  Created by Prashant on 01/06/12.
 *
 */

#include "../../definitions.h"
#include "../../maths/MathUtil.h"
#include "../../memory/memoryAllocator.h"
#include <new>

#ifndef _BROAD_PHASE_COLLISION_ALGORITHM_H_
#define _BROAD_PHASE_COLLISION_ALGORITHM_H_

/*
 This Class Represents the BoradPhase Collision Detection Algorithm used in Dirac. anyone who wishes to write a new BroadPhase Algorithm Should subclass and implement their own.
 */


BEGIN_NAMESPACE_DIRAC2D

class PhysicalWorld;
class CollisionManager;
class PhysicalShape;

// This structure is used by the BroadPhase as a Proxy to the PhysicalShape
class BroadPhaseNode
{
public:
	BroadPhaseNode()
	{
	}
	
	BroadPhaseNode(const BroadPhaseNode& other)
	{
	}
	
	PhysicalShape* m_PhysicalShape;
	BroadPhaseNode* m_Next;
	BroadPhaseNode* m_Prev;
};


class BroadPhaseCollisionAlgorithm
{
public:
	BroadPhaseCollisionAlgorithm(CollisionManager* collisionManager):m_pCollisionManager(collisionManager)
	{
		m_BroadPhaseNodeList = 0;
		m_BroadPhaseNodePool = new MemoryAllocator<BroadPhaseNode>(MAX_PROXIES);
	}
	
	// Adds a Physical Shape to the Broad Phase Node List
	void addBroadPhaseNode(PhysicalShape* pShape)
	{
		BroadPhaseNode *pBroadPhaseNode = new(m_BroadPhaseNodePool->Allocate()) BroadPhaseNode();
		pBroadPhaseNode->m_PhysicalShape = pShape;
		pBroadPhaseNode->m_Prev = 0;
		pBroadPhaseNode->m_Next = m_BroadPhaseNodeList;
		
		if( m_BroadPhaseNodeList )
		{
			m_BroadPhaseNodeList->m_Prev = pBroadPhaseNode;
		}
		m_BroadPhaseNodeList = pBroadPhaseNode;
	}

	// Removes a Proxy from the Broad Phase Node List
	void remove(PhysicalShape* pShape)
	{
//		BroadPhaseNode* prevNode = pShape->m_Prev;
//		BroadPhaseProxy* nextProxy = pProxy->m_Next;
//		
//		if( prevProxy )
//		{
//			prevProxy->m_Next = nextProxy;
//		}
//		else 
//		{
//			m_ProxyList = nextProxy;
//		}
//		
//		
//		if( nextProxy )
//		{
//			nextProxy->m_Prev = prevProxy;
//		}
	}
	
	virtual void update() = 0;
	

protected:
	
	BroadPhaseNode* m_BroadPhaseNodeList; // List of All BroadPhase Nodes.
	MemoryAllocator<BroadPhaseNode> *m_BroadPhaseNodePool;

	PhysicalWorld* m_PhysicalWorld;
	CollisionManager* m_pCollisionManager;
	
};

END_NAMESPACE_DIRAC2D

#endif
