#include "../definitions.h"
#include "../memory/memoryAllocator.h"
#include "../maths/MathUtil.h"
#include <set>

using namespace std;

#ifndef _COLLISION_MANAGER_H_
#define _COLLISION_MANAGER_H_

BEGIN_NAMESPACE_DIRAC2D

class PhysicalWorld;
class ContactSolver;
class Contact;
struct ContactPair;

class CollisionManager
{
public:
	CollisionManager(PhysicalWorld*);

	void update();

	friend class ContactSolver;
	
protected:
	
	void updateCollision();
	
	void updateContacts();
	
	Contact* createContact();
	
	void deleteContact(Contact* contact);
	
private:
	PhysicalWorld *m_PhysicalWorld;
	dint32 m_NumContacts;
	MemoryAllocator<Contact> *m_ContactPool;
	set<ContactPair> m_ContactPairSet;
};

END_NAMESPACE_DIRAC2D

#endif