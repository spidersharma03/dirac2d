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

CollisionManager::CollisionManager(PhysicalWorld* pWorld) : m_PhysicalWorld(pWorld)
{
	m_ContactPool = new MemoryAllocator<Contact>(MAX_CONTACT_POINTS);
    m_pCollisionListener = pWorld->m_pCollisionListener;
}
	
void CollisionManager::update()
{	
	updateContacts();	
}

// Update all the contacts of the PhysicalWorld
void CollisionManager::updateContacts()
{
	Contact* contact = m_PhysicalWorld->m_ContactList;
	
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
		
        //Check for Collision Filters here before updating contacts.
        CollisionFilter& filter1 = contact->m_PhysicalShape1->m_CollisionFilter;
        CollisionFilter& filter2 = contact->m_PhysicalShape2->m_CollisionFilter;
		
		bool bCollide = !((filter1.m_CollisionMask & filter2.m_CollisionBit) != 0  &&
						(filter2.m_CollisionMask & filter1.m_CollisionBit) != 0);
			
		if( !bCollide )	
		{
			deleteContact(contact);
			contact = contact->m_Next;
			continue;
		}
		
		// Destroy the Contact if it dosen't persist
		if( !aabb1.intersectAABB(aabb2) )
		{
			deleteContact(contact);
			// Awake the Bodies
			body1->setSleeping(false);
			body2->setSleeping(false);
		}
		else 
		{
			contact->update(m_pCollisionListener);
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
	PhysicalBody* pBody1 = pNode1->m_PhysicalShape->m_ParentBody;
	PhysicalBody* pBody2 = pNode2->m_PhysicalShape->m_ParentBody;
	
	//printf("%d  %d\n", pBody1->m_ContactEdgeList, pBody2->m_ContactEdgeList);

	if( pBody1 == pBody2 )
		return;
	
	ContactEdge* contactList = pBody2->m_ContactEdgeList;
	while (contactList) 
	{
		if( contactList->pBody == pBody1 )
		{
			CollisionShape* pShape1 = contactList->contact->m_CollisionShape1;
			CollisionShape* pShape2 = contactList->contact->m_CollisionShape2;
			if( pShape1 == pNode1->m_CollisionShape && pShape2 == pNode2->m_CollisionShape )
			{
				return;
			}
			if( pShape2 == pNode1->m_CollisionShape && pShape1 == pNode2->m_CollisionShape )
			{
				return;
			}
		}
		contactList = contactList->m_Next;
	}
	
	Contact* contact = createContact();
	
	contact->m_PhysicalShape1 = pNode1->m_PhysicalShape;
	contact->m_PhysicalShape2 = pNode2->m_PhysicalShape;

	contact->m_CollisionShape1 = pNode1->m_CollisionShape;
	contact->m_CollisionShape2 = pNode2->m_CollisionShape;

	contact->m_ContactEdge1.contact = contact;
	contact->m_ContactEdge1.pBody = pBody2;
	
	contact->m_ContactEdge1.m_Prev = 0;
	contact->m_ContactEdge1.m_Next = pBody1->m_ContactEdgeList;
	
	if( pBody1->m_ContactEdgeList )
	{
		pBody1->m_ContactEdgeList->m_Prev = &contact->m_ContactEdge1;
	}
	pBody1->m_ContactEdgeList = &contact->m_ContactEdge1;
	
	contact->m_ContactEdge2.contact = contact;
	contact->m_ContactEdge2.pBody = pBody1;

	contact->m_ContactEdge2.m_Prev = 0;
	contact->m_ContactEdge2.m_Next = pBody2->m_ContactEdgeList;
	
	if( pBody2->m_ContactEdgeList )
	{
		pBody2->m_ContactEdgeList->m_Prev = &contact->m_ContactEdge2;
	}
	pBody2->m_ContactEdgeList = &contact->m_ContactEdge2;
	
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
	
	// Remove Contact from body1's list
	PhysicalBody* pBody1 = contact->m_PhysicalShape1->m_ParentBody;
	if( contact->m_ContactEdge1.m_Prev )
	{
		contact->m_ContactEdge1.m_Prev->m_Next = contact->m_ContactEdge1.m_Next;
	}
	else 
	{
		pBody1->m_ContactEdgeList = contact->m_ContactEdge1.m_Next;
	}
	if( contact->m_ContactEdge1.m_Next )
	{
		contact->m_ContactEdge1.m_Next->m_Prev = contact->m_ContactEdge1.m_Prev;
	}
	
	// Remove Contact from body2's list
	PhysicalBody* pBody2 = contact->m_PhysicalShape2->m_ParentBody;
	if( contact->m_ContactEdge2.m_Prev )
	{
		contact->m_ContactEdge2.m_Prev->m_Next = contact->m_ContactEdge2.m_Next;
	}
	else 
	{
		pBody2->m_ContactEdgeList = contact->m_ContactEdge2.m_Next;
	}
	if( contact->m_ContactEdge2.m_Next )
	{
		contact->m_ContactEdge2.m_Next->m_Prev = contact->m_ContactEdge2.m_Prev;
	}
	
	m_ContactPool->Free(contact);
}

END_NAMESPACE_DIRAC2D