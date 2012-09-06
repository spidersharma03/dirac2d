/*
 *  CallBacks.h
 *  Dirac2D
 *
 *  Created by Prashant on 18/06/12.
 *
 */

#include "Settings.h"
#include "definitions.h"
#include "maths/MathUtil.h"


#ifndef _CALL_BACKS_H_
#define _CALL_BACKS_H_

BEGIN_NAMESPACE_DIRAC2D

class CollisionShape;

/* This Callback is for reporting any AABB overlaps happening in DynamicTree class. Any class which wishes to get the overlapCallback should subclass from this. this calls the 
   overlapCallBack function with the ID of the overlapping node. 	
   For example DynamicTreeBroadPhaseAlgorithm inherits from this class and gets the callback for each potential overlap from the Dynamic Tree.
 */
class OverlapCallBackClass
{
public:
	virtual void overlapCallBack(dint32 overlapNodeID, void* userData = 0) = 0;
};

/* This Callback is for reporting any intersection of a ray with Dynamic Tree AABB.
 */

class RayIntersectionCallBackClass
{
public:
	virtual void rayIntersectionCallBack(dint32 overlapNodeID, void* userData = 0) = 0;
};

/* This Callback is for reporting any intersection of a ray with any of the Collision Shape.
 */
class WorldRayIntersectionCallBackClass
{
  public:
	virtual void rayIntersectionCallBack(CollisionShape* pShape, RayIntersectionInfo& info) = 0;
};

END_NAMESPACE_DIRAC2D

#endif