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
class PhysicalShape;
struct ContactManifold;

/* This Callback is for reporting any AABB overlaps happening in DynamicTree class. Any class which wishes to get the overlapCallback should subclass from this. this calls the 
   overlapCallBack function with the ID of the overlapping node. 	
   For example DynamicTreeBroadPhaseAlgorithm inherits from this class and gets the callback for each potential overlap from the Dynamic Tree.
 */
class OverlapCallBackClass
{
public:
	virtual void overlapCallBack(dint32 overlapNodeID, void* userData = 0) = 0;
};


/* This Callback is for reporting any intersection of a ray with any of the Collision Shape.
   This is called when a ray intersects with any Collision Shape from the PhysicalWorld. it also provides with the Intersection info like hit time and hit Normal.
   
   return 0.0f to terminate the ray cast
   return 1.0 to continue
   return fraction to get the closest hit
 */
class WorldRayIntersectionCallBackClass
{
  public:
	virtual dfloat rayIntersectionCallBack(const RaySegment2f& raySeg, PhysicalShape* pShape, RayIntersectionInfo& info) = 0;
};


/* This Callback is for reporting any intersection of a ray with Collision Shape's AABB.
 This is called when a ray intersects with any AABB from the PhysicalWorld. 
 */

class RayIntersectionCallBackClass
{
public:
	RayIntersectionCallBackClass()
	{
		m_pWorldRayIntersectionCallBackClass = 0;
	}
	
	virtual dfloat rayIntersectionCallBack(const RaySegment2f& raySeg, dint32 overlapNodeID, void* userData = 0) = 0;
	
	friend class PhysicalWorld;
	
protected:
	WorldRayIntersectionCallBackClass *m_pWorldRayIntersectionCallBackClass;
};

/* This is an interface which is responsible for recieving collision call backs.
   make sure not to remove any Physical Bodies/Contacts/Constraints in these call backs or else the program will crash.
   if one of the colliding shape is a sensor then ContactManifold dosen't contain any info. */
class ICollisionLisetner
{
  public:
    // Called when two Physical Shapes begin to touch
    virtual void onCollisionEnter(PhysicalShape*, PhysicalShape*, ContactManifold&) = 0;    

    // Called when two Physical Shapes are in contact
    virtual void onCollision(PhysicalShape*, PhysicalShape*, ContactManifold&) = 0;    

    // Called when two Physical Shapes begin to seperate
    virtual void onCollisionExit(PhysicalShape*, PhysicalShape*, ContactManifold&) = 0;    

};


/*  */
class IContactListener
{
   public:
    virtual void preSolve();
    
    virtual void postSolve();
};

END_NAMESPACE_DIRAC2D

#endif