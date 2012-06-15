/*
 *  DynamicTreeBroadPhaseAlgorithm.h
 *  Dirac2D
 *
 *  Created by Prashant on 15/06/12.
 *
 */

#include "BroadPhaseCollisionAlgorithm.h"

#ifndef _DYNAMIC_TREE_BROAD_PHASE_ALGORITHM_H_
#define _DYNAMIC_TREE_BROAD_PHASE_ALGORITHM_H_

BEGIN_NAMESPACE_DIRAC2D

class PhysicalWorld;
class CollisionManager;
class DynamicTree;

class DynamicTreeBroadPhaseAlgorithm : public BroadPhaseCollisionAlgorithm
{	
public:
	DynamicTreeBroadPhaseAlgorithm(CollisionManager* collisionManager);
	
	virtual void addBroadPhaseNode(BroadPhaseNode* pBroadPhaseNode);
	
	virtual void removeBroadPhaseNode(BroadPhaseNode* pBroadPhaseNode);
	
	virtual void update();
	
protected:
	DynamicTree* m_DynamicTree;
};


END_NAMESPACE_DIRAC2D

#endif