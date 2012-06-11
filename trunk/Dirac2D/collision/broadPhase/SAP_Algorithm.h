/*
 *  SAP_Algorithm.h
 *  Dirac2D
 *
 *  Created by Prashant on 11/06/12.
 *
 */

#ifndef _SAP_ALGORITHM_H_
#define _SAP_ALGORITHM_H_

#include "../../definitions.h"
#include "../../maths/MathUtil.h"

#include "BroadPhaseCollisionAlgorithm.h"

BEGIN_NAMESPACE_DIRAC2D

class PhysicalWorld;
class CollisionManager;

class SAPAlgorithm : public BroadPhaseCollisionAlgorithm
{
public:
	SAPAlgorithm(CollisionManager* collisionManager);
	
	virtual void update();
	
protected:
	PhysicalWorld* m_PhysicalWorld;
};

END_NAMESPACE_DIRAC2D

#endif