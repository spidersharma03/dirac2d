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
#include "../dynamics/contacts/Contact.h"


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
	// n2 Collision test
	PhysicalBody* pBody1 = m_PhysicalWorld->m_PhysicalBodyList;
	while( pBody1 )
	{
		PhysicalShape* pShape1 = pBody1->m_PhysicalShapeList;
		while( pShape1 )
		{
			pShape1 = pShape1->m_Next;
		}
		
		
		PhysicalBody* pBody2 = pBody1;
		while( pBody2 )
		{
			if( pBody1 == pBody2 )
			{
				pBody2 = pBody2->m_Next;
				continue;
			}

			PhysicalShape* pShape2 = pBody2->m_PhysicalShapeList;
			while( pShape2 )
			{
				pShape2 = pShape2->m_Next;
			}
			
			if( pBody1->m_BodyType == EBT_STATIC && pBody2->m_BodyType == EBT_STATIC )
			{
				pBody2 = pBody2->m_Next;
				continue;
			}
						
			dbool res = true;
						
			res = pBody1->m_AABB.intersectAABB(pBody2->m_AABB);
			
			if( res && (m_ContactPairSet.insert( ContactPair(pBody1->m_PhysicalShapeList, pBody2->m_PhysicalShapeList) )).second)
			{
				Contact* contact = createContact();
				contact->m_PhysicalShape1 = pBody1->m_PhysicalShapeList;
				contact->m_PhysicalShape2 = pBody2->m_PhysicalShapeList;
				// Awake the Bodies
				pBody1->setSleeping(false);
				pBody2->setSleeping(false);
			}
			pBody2 = pBody2->m_Next;
		}
		pBody1 = pBody1->m_Next;
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
			// Erase Contact pair.
			m_ContactPairSet.erase(ContactPair(body1->m_PhysicalShapeList, body2->m_PhysicalShapeList));
			deleteContact(contact);
			// Awake the Bodies
			body1->setSleeping(false);
			body2->setSleeping(false);
		}
		else 
		{
			contact->update();
		}

		contact = contact->m_Next;
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
	else 
	{
		m_PhysicalWorld->m_ContactList = nextContact;
	}


	if( nextContact )
	{
		nextContact->m_Prev = prevContact;
	}
	
	
	m_ContactPool->Free(contact);
}

END_NAMESPACE_DIRAC2D