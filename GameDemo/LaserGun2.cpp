/*
 *  LaserGun2.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 17/12/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "LaserGun2.h"
#include "SimpleVehicle.h"
#include "FirstGame.h"
#include "GunRayCastCallback.h"
#include "ObjectManager.h"

LaserGun2::LaserGun2(FirstGame* pGame) : Gun(pGame)
{
	m_ShootingInterval = 10.0f;
	m_pGunRayCastManyCallBack = new GunRayCastManyCallBack(m_pGame);
	m_ShootingRadius = 10.0f;
	m_ShootingImpactRadius = 1.0f;
	m_ShootingImpact = 2.0f;
}

static int count = 100;


void LaserGun2::shoot(const Vector2f& direction)
{	
	count = 255;
    {
        // Shoot Ray/Gun
		RaySegment2f raySegment;
		raySegment.m_Start = m_ShootingOrigin;
		raySegment.m_End   = m_ShootingOrigin + direction * m_ShootingRadius;//m_ShootingTarget;
		m_ShootingTarget = direction * m_ShootingRadius;
		
		m_pGame->getPhysicalWorld()->intersectRaySegment(raySegment, m_pGunRayCastManyCallBack);
		        
		// If an intersection is found.
		for( int i=0; i<m_pGunRayCastManyCallBack->m_ObjectCounter; i++ )
		{
			GameObject* pObject = m_pGunRayCastManyCallBack->m_pObject[i];
			
			if( pObject )
			{
				if( pObject->getGameObjectInfo().m_ObjectType == EOT_CRATE )
				{
					// Mark the Crate for Removal.
					m_pGame->getObjectManager()->markObjectsForCleanup(pObject);                
					m_pGame->explodeDebris(pObject->getGameObjectInfo(), pObject->getPosition());
					m_pGunRayCastManyCallBack->m_pObject[i] = 0;
					
					// Query for Objects within shooting impact radius, and give them impulse
					Vector2f poi = m_pGunRayCastManyCallBack->m_IntersectionInfo[i].m_HitPoint;
					AABB2f queryAABB;
					queryAABB.m_LowerBounds = poi - Vector2f(m_ShootingImpactRadius,m_ShootingImpactRadius);
					queryAABB.m_UpperBounds = poi + Vector2f(m_ShootingImpactRadius,m_ShootingImpactRadius);
					
					m_pGame->getPhysicalWorld()->overlapAABB(queryAABB, m_pGame);
					m_pGame->applyImpulse(poi,m_ShootingImpact);
				}
				else
				{
					m_pGunRayCastManyCallBack->m_pObject[i] = 0;  
				}
			}
			m_pGunRayCastManyCallBack->m_pObject[i] = 0;
		}
		for( int i=0; i<m_pGunRayCastManyCallBack->m_MaxObjectPickCount; i++ )
			m_pGunRayCastManyCallBack->m_pObject[i] = 0;
		
		m_pGunRayCastManyCallBack->m_ObjectCounter = 0;
	} 
}

void LaserGun2::render()
{
	glPushMatrix();
	if( count > 0 )
	{
		count--;
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_pGame->getRenderer()->setColor(255, 0, 0, count);
		m_pGame->getRenderer()->drawLine(m_ShootingOrigin, m_ShootingOrigin + m_ShootingTarget);
		//m_pGame->getRenderer()->setColor(255, 255, 0, count);
		//m_pGame->getRenderer()->setPointSize(6.0f);
		//m_pGame->getRenderer()->drawPoint(m_pGame->getMousePositionWorldSpace());
		//m_pGame->getRenderer()->setPointSize(1.0f);
		glDisable(GL_BLEND);
	}
	glPopMatrix();
}

void LaserGun2::update(float dt)
{
	m_ShootingOrigin = m_pGame->getVehicle()->getPosition();// + Vector2f(0.02f,-0.03f);
	//Vector2f direction = m_pGame->getMousePositionWorldSpace() - m_pGame->getVehicle()->getPosition();
	//direction.normalize();
	
	//Matrix3f xForm = m_pGame->getVehicle()->getTransform();
	//m_ShootingTarget = Vector2f(m_ShootingRadius,0.0f);
	//xForm.transformAsVector(m_ShootingTarget);
	
	//m_ShootingTarget = direction * m_ShootingRadius;
}
