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
class CollisionShape;

// This structure is used by the BroadPhase
class ContactNode
{
public:
	PhysicalShape* m_PhysicalShape;
	CollisionShape* m_CollisionShape;
	ContactNode* m_Next;
	ContactNode* m_Prev;
};


class BroadPhaseCollisionAlgorithm
{
public:
	BroadPhaseCollisionAlgorithm(CollisionManager* collisionManager):m_pCollisionManager(collisionManager)
	{
		m_BroadPhaseNodeList = 0;
	}
	
	// Adds a Physical Shape to the Broad Phase Node List
	void addBroadPhaseNode(ContactNode* pBroadPhaseNode)
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
	void removeBroadPhaseNode(ContactNode* pBroadPhaseNode)
	{
		ContactNode* prevNode = pBroadPhaseNode->m_Prev;
		ContactNode* nextNode = pBroadPhaseNode->m_Next;
		
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
	
	virtual void update() = 0;
	

protected:
	
	ContactNode* m_BroadPhaseNodeList; // List of All BroadPhase Nodes.

	PhysicalWorld* m_PhysicalWorld;
	CollisionManager* m_pCollisionManager;
	
};

END_NAMESPACE_DIRAC2D

#endif
