/*
 *  SAP_Algorithm.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 11/06/12.
 *
 */

#include "collision/broadPhase/SAP_Algorithm.h"

BEGIN_NAMESPACE_DIRAC2D

SAPAlgorithm::SAPAlgorithm(CollisionManager* collisionManager):
BroadPhaseCollisionAlgorithm(collisionManager)
{
	m_AlgorithmType = EAT_SAP;
}

void SAPAlgorithm::addBroadPhaseNode(BroadPhaseNode* pBroadPhaseNode)
{
}

void SAPAlgorithm::removeBroadPhaseNode(BroadPhaseNode* pBroadPhaseNode)
{
}

void SAPAlgorithm::update()
{
}

void SAPAlgorithm::overlapAABB( AABB2f& queryAABB, OverlapCallBackClass* callBack )
{
}

void SAPAlgorithm::intersectRay( const Ray2f& ray,RayIntersectionCallBackClass* callBack)
{
}

void SAPAlgorithm::intersectRaySegment(const RaySegment2f& raySegment, RayIntersectionCallBackClass* callBack)
{
}

END_NAMESPACE_DIRAC2D