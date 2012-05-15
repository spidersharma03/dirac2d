#include "../definitions.h"
#include "../maths/MathUtil.h"
#include "../dynamics/contacts/Contact.h"

#include <vector>

using namespace std;

#ifndef _COLLISION_MANAGER_H_
#define _COLLISION_MANAGER_H_

BEGIN_NAMESPACE_DIRAC2D

class PhysicalWorld;
class ContactSolver;
class Contact;

class CollisionManager
{
public:
	CollisionManager(PhysicalWorld*);

	void update();
	
	friend class ContactSolver;
	
	void getContactList( Contact** contacts, dint32& numContacts)
	{
		*contacts = m_ContactList;
		numContacts = m_NumContacts;
	}
	
private:
	PhysicalWorld *m_PhysicalWorld;
	Contact m_ContactList[MAX_CONTACT_POINTS];
	dint32 m_NumContacts;
};

END_NAMESPACE_DIRAC2D

#endif