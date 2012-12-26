#include "../definitions.h"
#include "../memory/memoryAllocator.h"
#include "../memory/memoryAllocatorNew.h"
#include "../maths/MathUtil.h"


#ifndef _COLLISION_MANAGER_H_
#define _COLLISION_MANAGER_H_

BEGIN_NAMESPACE_DIRAC2D

class BroadPhaseCollisionAlgorithm;
class PhysicalWorld;
class PhysicalShape;
class ContactSolver;
class Contact;
class BroadPhaseNode;
class ICollisionLisetner;

struct ContactPair;


class CollisionManager
{
public:
	CollisionManager(PhysicalWorld*);

	void update();
	
	void addContact(PhysicalShape* pShape1, PhysicalShape* pShape2);

	void addContactPair(BroadPhaseNode* pNode1, BroadPhaseNode* pNode2);
	
    friend class PhysicalWorld;
protected:
		
	void updateContacts();
	
	Contact* createContact();
	
	void deleteContact(Contact* contact);

	
private:
	PhysicalWorld *m_PhysicalWorld;
	MemoryAllocatorNew<Contact> *m_ContactPool;
    ICollisionLisetner* m_pCollisionListener;
};

END_NAMESPACE_DIRAC2D

#endif