/*
 *  SimpleGun.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 12/12/12.
 *
 */

#include "SimpleGun.h"
#include "SimpleVehicle.h"
#include "FirstGame.h"
#include "GunRayCastCallback.h"
#include "ObjectManager.h"

SimpleGun::SimpleGun(FirstGame* pGame) : Gun(pGame)
{
	m_ShootingInterval = 10.0f;
	m_pGunRayCastClosestCallBack = new GunRayCastClosestCallBack(m_pGame);
	m_ShootingRadius = 5.0f;
	m_ShootingImpactRadius = 1.0f;
	m_ShootingImpact = 4.0f;
}

void SimpleGun::shoot(const Vector2f& direction)
{	
    //if( time - initTime > m_ShootingInterval )
    {
        // Shoot Ray/Gun
		RaySegment2f raySegment;
		raySegment.m_Start = m_ShootingOrigin;
		raySegment.m_End   = m_ShootingOrigin + m_ShootingTarget;
		
		m_pGame->getPhysicalWorld()->intersectRaySegment(raySegment, m_pGunRayCastClosestCallBack);
		
        //printf("%d\n", m_pGunRayCastClosestCallBack->m_pObject);
        
		// If an intersection is found.
		if( m_pGunRayCastClosestCallBack->m_pObject )
		{
			if( m_pGunRayCastClosestCallBack->m_pObject->getGameObjectInfo().m_ObjectType == EOT_CRATE )
			{
				// Mark the Crate for Removal.
				m_pGame->getObjectManager()->markObjectsForCleanup(m_pGunRayCastClosestCallBack->m_pObject);                
                m_pGame->explodeDebris(m_pGunRayCastClosestCallBack->m_pObject->getGameObjectInfo(), m_pGunRayCastClosestCallBack->m_pObject->getPosition());
                m_pGunRayCastClosestCallBack->m_pObject = 0;

				// Query for Objects within shooting impact radius, and give them impulse
				Vector2f poi = m_pGunRayCastClosestCallBack->m_IntersectionInfo.m_HitPoint;
				AABB2f queryAABB;
				queryAABB.m_LowerBounds = poi - Vector2f(m_ShootingImpactRadius,m_ShootingImpactRadius);
				queryAABB.m_UpperBounds = poi + Vector2f(m_ShootingImpactRadius,m_ShootingImpactRadius);
				
				m_pGame->getPhysicalWorld()->overlapAABB(queryAABB, m_pGame);
				m_pGame->applyImpulse(poi,m_ShootingImpact);
			}
            else
            {
              m_pGunRayCastClosestCallBack->m_pObject = 0;  
            }
		}
	} 
}

void SimpleGun::render()
{
	m_pGame->getRenderer()->drawLine(m_ShootingOrigin, m_ShootingOrigin + m_ShootingTarget);
}

void SimpleGun::update(float dt)
{
	m_ShootingOrigin = m_pGame->getVehicle()->getPosition() + Vector2f(0.02f,-0.03f);
	
	Matrix3f xForm = m_pGame->getVehicle()->getTransform();
	m_ShootingTarget = Vector2f(m_ShootingRadius,0.0f);
	xForm.transformAsVector(m_ShootingTarget);
}
