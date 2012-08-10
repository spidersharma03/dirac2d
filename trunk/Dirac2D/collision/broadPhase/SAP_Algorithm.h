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
	struct SAP_Box;
	
	struct EndPoint
	{
		SAP_Box* m_Box;
		dfloat m_Value;
		dint32 m_bIsMin;
	};
	
	struct SAP_Box 
	{
		EndPoint* m_Min[2];
		EndPoint* m_Max[2];
	};
	
public:
	SAPAlgorithm(CollisionManager* collisionManager);
	
	virtual void addBroadPhaseNode(BroadPhaseNode* pBroadPhaseNode);
	
	virtual void removeBroadPhaseNode(BroadPhaseNode* pBroadPhaseNode);
	
	virtual void update();

	virtual void overlapAABB( AABB2f& queryAABB, OverlapCallBackClass* callBack );
	
	virtual void intersectRay( const Ray2f& ray, RayIntersectionCallBackClass* callBack);
};

END_NAMESPACE_DIRAC2D

#endif