/*
 *  Contact.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 18/05/12.
 *
 */

#include "Contact.h"
#include "../PhysicalShape.h"
#include "../PhysicalBody.h"
#include "../../collision/CollisionDetection.h"

BEGIN_NAMESPACE_DIRAC2D

void Contact::update()
{
	ContactManifold oldManifold = m_Manifold;
	dbool bRes = intersectShapes(m_PhysicalShape1->m_CollisionShape, m_PhysicalShape1->m_ParentBody->m_Transform, m_PhysicalShape2->m_CollisionShape, m_PhysicalShape2->m_ParentBody->m_Transform, &m_Manifold);
	m_ContactNormal = m_Manifold.m_ContactNormal;
	m_NumContactConstraints = m_Manifold.m_NumContacts;
	for( dint32 i=0; i<m_Manifold.m_NumContacts; i++ )
	{
		m_ContactPoint[i] = m_Manifold.m_ContactPoints[i];
	}
	if( bRes )
	{
		
	}
}

END_NAMESPACE_DIRAC2D
