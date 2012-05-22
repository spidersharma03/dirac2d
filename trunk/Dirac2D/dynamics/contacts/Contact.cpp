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

#include <stdio.h>

BEGIN_NAMESPACE_DIRAC2D

void Contact::update()
{
	ContactManifold oldManifold = m_Manifold;
	ContactConstraint oldConstraint[MAX_CONTACTS];
	oldConstraint[0].m_NormalImpulse = m_ContactConstraint[0].m_NormalImpulse;
	oldConstraint[0].m_TangentImpulse = m_ContactConstraint[0].m_TangentImpulse;

	oldConstraint[1].m_NormalImpulse = m_ContactConstraint[1].m_NormalImpulse;
	oldConstraint[1].m_TangentImpulse = m_ContactConstraint[1].m_TangentImpulse;
	
	dbool bRes = intersectShapes(m_PhysicalShape1->m_CollisionShape, m_PhysicalShape1->m_ParentBody->m_Transform, m_PhysicalShape2->m_CollisionShape, m_PhysicalShape2->m_ParentBody->m_Transform, &m_Manifold);
	if( bRes )
	{
		m_ContactNormal = m_Manifold.m_ContactNormal;
		m_NumContactConstraints = m_Manifold.m_NumContacts;
		m_ContactConstraint[0].m_NormalImpulse = 0.0f;
		m_ContactConstraint[0].m_TangentImpulse = 0.0f;
		m_ContactConstraint[1].m_NormalImpulse = 0.0f;
		m_ContactConstraint[1].m_TangentImpulse = 0.0f;
		
		for( dint32 i=0; i<m_Manifold.m_NumContacts; i++ )
		{
			m_ContactPoint[i] = m_Manifold.m_ContactPoints[i];
		}
		// Compare old and new manifold, and if they match, copy old impulses into new impulses to start with a good impulse guess.( warm start).
		// generally contacts are temporally coherent, and this can really boost the quality of the Contact Solver's output.
		for( dint32 n=0; n<m_Manifold.m_NumContacts; n++ )
		{
			ContactPoint& cp_new = m_Manifold.m_ContactPoints[n];
			//printf("%d  %d   ", cp_new.m_ID.edgeIndex.m_Index1, cp_new.m_ID.edgeIndex.m_Index2 );//, cp_new.m_ID.edgeIndex.m_Type1, cp_new.m_ID.edgeIndex.m_Type2);
			for( dint32 o=0; o<oldManifold.m_NumContacts; o++ )
			{
				ContactPoint& cp_old = oldManifold.m_ContactPoints[o];
				if( cp_new.m_ID.m_Key == cp_old.m_ID.m_Key )
				{
					m_ContactConstraint[n].m_NormalImpulse = oldConstraint[n].m_NormalImpulse;
					m_ContactConstraint[n].m_TangentImpulse = oldConstraint[n].m_TangentImpulse;
					//m_ContactConstraint[n].m_TangentImpulse = 0.0f;
					//printf("Matched ID's %d\n", o);
				}
				else 
				{
					//printf("No Matching\n");
				}

			}
		}
		//printf("\n");
	}
	else 
	{
		m_NumContactConstraints = 0;
	}

}

END_NAMESPACE_DIRAC2D