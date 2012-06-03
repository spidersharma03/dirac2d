#include "../definitions.h"
#include "../memory/memoryAllocator.h"
#include "../maths/MathUtil.h"
#include <set>

using namespace std;

#ifndef _COLLISION_MANAGER_H_
#define _COLLISION_MANAGER_H_

BEGIN_NAMESPACE_DIRAC2D

class BroadPhaseCollisionAlgorithm;
class PhysicalWorld;
class PhysicalShape;
class ContactSolver;
class Contact;
class ContactNode;
struct ContactPair;

typedef set<ContactPair> ContactPairPool;


class CollisionManager
{
public:
	CollisionManager(PhysicalWorld*);

	void update();
	
	void addContact(PhysicalShape* pShape1, PhysicalShape* pShape2);

	void addContactPair(ContactNode* pNode1, ContactNode* pNode2);
	
	ContactPairPool& getContactPairPool()
	{
		return m_ContactPairSet;
	}
	
protected:
	
	void updateCollision();
	
	void updateContacts();
	
	Contact* createContact();
	
	void deleteContact(Contact* contact);

	
private:
	PhysicalWorld *m_PhysicalWorld;
	MemoryAllocator<Contact> *m_ContactPool;
	ContactPairPool m_ContactPairSet;
};

END_NAMESPACE_DIRAC2D

#endif