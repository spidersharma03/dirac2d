/*
 *  CollisionManager.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 30/04/12.
 *
 */

#include "../dynamics/PhysicalWorld.h"
#include "CollisionManager.h"
#include "CollisionDetection.h"
#include "../dynamics/PhysicalBody.h"
#include "../dynamics/PhysicalShape.h"

BEGIN_NAMESPACE_DIRAC2D

CollisionManager::CollisionManager(PhysicalWorld* world) : m_PhysicalWorld(world)
{
	m_ContactPool = new MemoryAllocator<Contact>(MAX_CONTACT_POINTS);
}
	

void CollisionManager::update()
{
	updateCollision();
	
	updateContacts();
}


void CollisionManager::updateCollision()
{
	// clear Contact list.
	m_NumContacts = 0;
	// n2 Collision test
	for( duint32 b1=0; b1<m_PhysicalWorld->m_vecPhysicalBodies.size(); b1++ )
	{
		PhysicalBody* body1 = m_PhysicalWorld->m_vecPhysicalBodies[b1];
		for( duint32 b2=b1; b2<m_PhysicalWorld->m_vecPhysicalBodies.size(); b2++ )
		{
			PhysicalBody* body2 = m_PhysicalWorld->m_vecPhysicalBodies[b2];

			if( body1 == body2 )continue;
						
			dbool res = true;
			
			res = body1->m_AABB.intersectAABB(body2->m_AABB);
			
			if( res )
			{
				Contact* contact = createContact();
				contact->m_PhysicalShape1 = body1->m_PhysicalShapeList;
				contact->m_PhysicalShape2 = body2->m_PhysicalShapeList;
				m_NumContacts++;
			}
		}
	}
}

// Update all the contacts of the PhysicalWorld
void CollisionManager::updateContacts()
{
	Contact* contactList = m_PhysicalWorld->m_ContactList;
	Contact* contact = contactList;
	
	if( !contact )
		return;
	
	while(contact)
	{
		PhysicalBody* body1 = contact->m_PhysicalShape1->m_ParentBody;
		PhysicalBody* body2 = contact->m_PhysicalShape2->m_ParentBody;
		
		// Destroy the Contact if it dosen't persist
		if( !body1->m_AABB.intersectAABB(body2->m_AABB) )
		{
			deleteContact(contact);
		}
		else 
		{
			contact->update();
		}

		contact = contactList->m_Next;
	}
}

// Creates a new Contact by Allocating in the Contact Pool and inserting into the World linked list.
Contact* CollisionManager::createContact()
{
	Contact* c = new(m_ContactPool->Allocate()) Contact();
	c->m_Prev = 0;
	c->m_Next = m_PhysicalWorld->m_ContactList;
	
	if( m_PhysicalWorld->m_ContactList )
	{
		m_PhysicalWorld->m_ContactList->m_Prev = c;
	}
	m_PhysicalWorld->m_ContactList = c;

	return c;
}

// Remove a Contact from the world contact linked list and the Contact Pool.
void CollisionManager::deleteContact(Contact* contact)
{	
	Contact* prevContact = contact->m_Prev;
	Contact* nextContact = contact->m_Next;
	
	if( prevContact )
	{
		prevContact->m_Next = nextContact;
	}

	if( nextContact )
	{
		nextContact->m_Prev = prevContact;
	}
	
	m_ContactPool->Free(contact);
}

END_NAMESPACE_DIRAC2D