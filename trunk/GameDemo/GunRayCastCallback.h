/*
 *  GunRayCastCallback.h
 *  Dirac2D
 *
 *  Created by Prashant on 12/12/12.
 *
 */

#ifndef _GUN_RAY_CAST_CALL_BACK_
#define _GUN_RAY_CAST_CALL_BACK_


#include "Dirac2D.h"

USE_NAMESPACE_DIRAC2D

class FirstGame;
class GameObject;

class GunRayCastClosestCallBack : public WorldRayIntersectionCallBackClass
{
public:
	GunRayCastClosestCallBack(FirstGame* pGame );
	
	virtual float rayIntersectionCallBack(const RaySegment2f& raySeg, PhysicalShape* pShape, RayIntersectionInfo& info);
	
    friend class SimpleGun;
private:
	GameObject* m_pObject; // Closest Object Picked
	RayIntersectionInfo m_IntersectionInfo; // Closest Intersection Info
protected:
	FirstGame* m_pGame;
};

class GunRayCastManyCallBack : public WorldRayIntersectionCallBackClass
{
public:
	GunRayCastManyCallBack(FirstGame* pGame );
	
	virtual float rayIntersectionCallBack(const RaySegment2f& raySeg, PhysicalShape* pShape, RayIntersectionInfo& info);
	
	friend class LaserGun2;
private:
	GameObject* m_pObject[10]; // Objects Picked
	RayIntersectionInfo m_IntersectionInfo[10]; // Closest Intersection Info
	int m_ObjectCounter;
	
	int m_MaxObjectPickCount;
protected:
	FirstGame* m_pGame;
};

#endif