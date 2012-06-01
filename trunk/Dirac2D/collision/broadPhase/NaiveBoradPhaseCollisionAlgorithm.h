/*
 *  NaiveBoradPhaseCollisionAlgorithm.h
 *  Dirac2D
 *
 *  Created by Prashant on 01/06/12.
 *
 */

#ifndef _NAIVE_BROAD_PHASE_COLLISION_ALGORITHM_H_
#define _NAIVE_BROAD_PHASE_COLLISION_ALGORITHM_H_

#include "../../definitions.h"
#include "../../maths/MathUtil.h"

#include "BroadPhaseCollisionAlgorithm.h"

BEGIN_NAMESPACE_DIRAC2D

class PhysicalWorld;
class CollisionManager;

class NaiveBroadPhaseCollisionAlgorithm : public BroadPhaseCollisionAlgorithm
{
public:
	NaiveBroadPhaseCollisionAlgorithm(CollisionManager* collisionManager);
	
	virtual void update();
	
protected:
	PhysicalWorld* m_PhysicalWorld;
};

END_NAMESPACE_DIRAC2D

#endif