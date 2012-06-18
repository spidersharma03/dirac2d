/*
 *  CollisionManager.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 30/04/12.
 *
 */

#include "../dynamics/PhysicalWorld.h"
#include "CollisionManager.h"
#include "../dynamics/PhysicalBody.h"
#include "../dynamics/PhysicalShape.h"
#include "../dynamics/contacts/Contact.h"
#include "../geometry/CollisionShape.h"
#include "broadPhase/BroadPhaseCollisionAlgorithm.h"

BEGIN_NAMESPACE_DIRAC2D

CollisionManager::CollisionManager(PhysicalWorld* world) : m_PhysicalWorld(world)
{
	m_ContactPool = new MemoryAllocator<Contact>(MAX_CONTACT_POINTS);
}
	
void CollisionManager::update()
{	
	updateContacts();	
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
		
		CollisionShape* pShape1 = contact->m_CollisionShape1;
		CollisionShape* pShape2 = contact->m_CollisionShape2;
		
		AABB2f& aabb1 = pShape1->getAABB();
		AABB2f& aabb2 = pShape2->getAABB();
		
		// Destroy the Contact if it dosen't persist
		if( !aabb1.intersectAABB(aabb2) )
		{
			// Erase Contact pair.
			m_ContactPairSet.erase(ContactPair(pShape1, pShape2));
		
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

void CollisionManager::addContact(PhysicalShape* pShape1, PhysicalShape* pShape2)
{
	Contact* contact = createContact();
	contact->m_PhysicalShape1 = pShape1;
	contact->m_PhysicalShape2 = pShape2;
	
	// Awake the Bodies
	contact->m_PhysicalShape1->m_ParentBody->setSleeping(false);
	contact->m_PhysicalShape2->m_ParentBody->setSleeping(false);
}

void CollisionManager::addContactPair(BroadPhaseNode* pNode1, BroadPhaseNode* pNode2)
{
	Contact* contact = createContact();
	contact->m_PhysicalShape1 = pNode1->m_PhysicalShape;
	contact->m_PhysicalShape2 = pNode2->m_PhysicalShape;

	contact->m_CollisionShape1 = pNode1->m_CollisionShape;
	contact->m_CollisionShape2 = pNode2->m_CollisionShape;

	// Awake the Bodies
	contact->m_PhysicalShape1->m_ParentBody->setSleeping(false);
	contact->m_PhysicalShape2->m_ParentBody->setSleeping(false);
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