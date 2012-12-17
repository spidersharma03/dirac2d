/*
 *  GunRayCastCallback.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 12/12/12.
 *
 */

#include "GunRayCastCallback.h"
#include "../Dirac2D/Dirac2D.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "FirstGame.h"

USE_NAMESPACE_DIRAC2D

GunRayCastClosestCallBack::GunRayCastClosestCallBack(FirstGame* pGame)
{
	m_pGame = pGame;
	m_pGame->getPhysicalWorld()->setRayIntersectionListener(this);
	m_pObject = 0;
}

// Any Intersection with the Ray cast will be reported here.
float GunRayCastClosestCallBack::rayIntersectionCallBack(const RaySegment2f& raySeg, PhysicalShape* pShape, RayIntersectionInfo& info)
{	
	GameObject* pObject = (GameObject*)pShape->getUserData();
    //printf("%f  %d", info.m_HitT, pObject);
    //if( pObject )
    //    printf("  %d\n", pObject->getGameObjectInfo().m_ObjectType);

    if( pObject )
    {
		if( pObject->getGameObjectInfo().m_ObjectType != EOT_CRATE )
        {
			return -1.0f;
		}
        
		m_pObject = pObject;
		m_IntersectionInfo = info;
    }
    else
    {
        return -1.0f;
    }
	return info.m_HitT;
}