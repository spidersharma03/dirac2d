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

class CollisionManager
{
public:
	CollisionManager(PhysicalWorld*);

	void update();
	
	friend class ContactSolver;
	
private:
	PhysicalWorld *m_PhysicalWorld;
	vector<Contact> m_vecContacts;
};

END_NAMESPACE_DIRAC2D

#endif