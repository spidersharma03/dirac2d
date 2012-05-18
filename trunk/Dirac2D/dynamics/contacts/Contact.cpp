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
	dbool bRes = intersectShapes(m_PhysicalShape1->m_CollisionShape, m_PhysicalShape1->m_ParentBody->m_Transform, m_PhysicalShape1->m_CollisionShape, m_PhysicalShape1->m_ParentBody->m_Transform, &m_Manifold);
	if( bRes )
	{
		
	}
}

END_NAMESPACE_DIRAC2D
