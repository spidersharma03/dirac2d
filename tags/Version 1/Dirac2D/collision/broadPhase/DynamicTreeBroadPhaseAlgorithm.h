/*
 *  DynamicTreeBroadPhaseAlgorithm.h
 *  Dirac2D
 *
 *  Created by Prashant on 15/06/12.
 *
 */

#include "BroadPhaseCollisionAlgorithm.h"
#include <vector>

using namespace std;

#ifndef _DYNAMIC_TREE_BROAD_PHASE_ALGORITHM_H_
#define _DYNAMIC_TREE_BROAD_PHASE_ALGORITHM_H_

BEGIN_NAMESPACE_DIRAC2D

class PhysicalWorld;
class CollisionManager;
class DynamicTree;

class DynamicTreeBroadPhaseAlgorithm : public BroadPhaseCollisionAlgorithm, OverlapCallBackClass
{	
public:
	DynamicTreeBroadPhaseAlgorithm(CollisionManager* collisionManager);
	
	virtual void addBroadPhaseNode(BroadPhaseNode* pBroadPhaseNode);
	
	virtual void removeBroadPhaseNode(BroadPhaseNode* pBroadPhaseNode);
	
	virtual void update();
		
	virtual void overlapAABB( AABB2f& queryAABB, OverlapCallBackClass* callBack );
	
	virtual void intersectRay( RayIntersectionCallBackClass* callBack);
	
	virtual void overlapCallBack(dint32 overlapNodeID);
	
	DynamicTree* getDynamicTree()
	{
		return m_DynamicTree;
	}
protected:
	DynamicTree* m_DynamicTree;
	dint32 m_QueryID;
	vector<dint32> m_vecOverlappedId;
};


END_NAMESPACE_DIRAC2D

#endif