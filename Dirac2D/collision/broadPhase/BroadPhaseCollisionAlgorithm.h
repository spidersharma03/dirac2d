/*
 *  BroadPhaseCollisionAlgorithm.h
 *  Dirac2D
 *
 *  Created by Prashant on 01/06/12.
 *
 */

#include "../../CallBacks.h"
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
class BroadPhaseNode
{
public:
	AABB2f m_AABB;
	PhysicalShape* m_PhysicalShape;
	CollisionShape* m_CollisionShape;
	BroadPhaseNode* m_Next;
	BroadPhaseNode* m_Prev;
	dint32 m_ID;
};

struct BroadPhasePair
{
	BroadPhaseNode* m_Node1;
	BroadPhaseNode* m_Node2;
};

enum ALGORITHM_TYPE { EAT_NAIVE, EAT_DYNAMIC_TREE, EAT_SAP };

class BroadPhaseCollisionAlgorithm 
{
public:
	BroadPhaseCollisionAlgorithm(CollisionManager* collisionManager):m_pCollisionManager(collisionManager)
	{
		m_BroadPhaseNodeList = 0;
		m_PairCapacity = 64;
		m_PairCount = 0;
		m_Pairs = (BroadPhasePair*)malloc(m_PairCapacity*sizeof(BroadPhasePair));
	}
	
	// Adds a Physical Shape to the Broad Phase Node List
	virtual void addBroadPhaseNode(BroadPhaseNode* pBroadPhaseNode) = 0;
	
	// Removes a Proxy from the Broad Phase Node List
	virtual void removeBroadPhaseNode(BroadPhaseNode* pBroadPhaseNode) = 0;
	
	virtual void update() = 0;
	
	virtual void overlapAABB( AABB2f& queryAABB, OverlapCallBackClass* callBack ) = 0;
	
	virtual void intersectRay( const Ray2f& ray, RayIntersectionCallBackClass* callBack) = 0;
	
	ALGORITHM_TYPE getAlgorithmType()
	{
		return m_AlgorithmType;
	}
protected:
	
	BroadPhaseNode* m_BroadPhaseNodeList; // List of All BroadPhase Nodes.
	BroadPhasePair* m_Pairs;
	dint32 m_PairCount, m_PairCapacity;
	
	PhysicalWorld* m_PhysicalWorld;
	CollisionManager* m_pCollisionManager;
	ALGORITHM_TYPE m_AlgorithmType;
};

END_NAMESPACE_DIRAC2D

#endif
