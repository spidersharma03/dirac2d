/*
 *  PhysicalWorld.h
 *  Dirac2D
 *
 *  Created by Prashant on 24/04/12.
 *
 */

#include "../definitions.h"
#include "../Settings.h"
#include "../maths/MathUtil.h"
#include "PhysicalAppearance.h"
#include "../memory/memoryAllocator.h"

#include <vector>

using namespace std;

#ifndef _PHYSICAL_WORLD_H_
#define _PHYSICAL_WORLD_H_

BEGIN_NAMESPACE_DIRAC2D

class PhysicalBody;
class CollisionManager;
class ContactSolver;
class Contact;
class Renderer;

class PhysicalWorld
{
public:
	PhysicalWorld();
	
	PhysicalBody* createPhysicalBody();

	PhysicalBody* createPhysicalBody(PhysicalAppearance& pApp);

	void deletePhysicalBody(PhysicalBody* pBody);

	void Step(dfloat dt);
	
	void draw();
	
	void setRenderer(Renderer*);
	
	inline void setGravityOn(dbool btrue)
	{
		m_bGravityOn = btrue;
	}
	
	inline void addExternalForce( Vector2f& fext )
	{
		m_TotalForce += fext;
	}
	
	void setWarmStart( dbool bWarmStart )
	{
		m_bWarmStart = bWarmStart;
	}
public:
	
	friend class CollisionManager;
	friend class ContactSolver;
	
private:
	CollisionManager* m_CollisionManager;
	ContactSolver*	  m_ContactSolver;
	Renderer*         m_Renderer;

	Contact* m_ContactList;  // Doubly linked list of all the Contacts in this world.
	
	PhysicalBody* m_PhysicalBodyList;
	
	static Vector2f GRAVITY;
	
	Vector2f m_TotalForce;
	Vector2f m_TotalTorque;
	
	dfloat m_CFM, m_ERP;
	dbool m_bGravityOn;
	dfloat m_GravityScale;
	
	dint16 m_VelocityIterations;
	
	dbool m_bDrawAABB;
	
	dbool m_bWarmStart;
	
	MemoryAllocator<PhysicalBody> *m_PhysicalBodyPool;
	
	dbool m_bDrawShapes;
	dbool m_bDrawBoundingBoxes;
	dbool m_bDrawContacts;
	dbool m_bDrawConstraints;
	dbool m_bDrawCentreOfMass;
};

END_NAMESPACE_DIRAC2D

#endif