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

#include <vector>

using namespace std;

#ifndef _PHYSICAL_WORLD_H_
#define _PHYSICAL_WORLD_H_

BEGIN_NAMESPACE_DIRAC2D

class PhysicalBody;
class CollisionManager;
class ContactSolver;

class PhysicalWorld
{
public:
	PhysicalWorld();
	
	PhysicalBody* createPhysicalBody();

	PhysicalBody* createPhysicalBody(PhysicalAppearance& pApp);

	void Step(dfloat dt);
	
	inline void setGravityOn(dbool btrue)
	{
		m_bGravityOn = btrue;
	}
	
	inline void addExternalForce( Vector2f& fext )
	{
		m_TotalForce += fext;
	}
	
public:
	
	friend class CollisionManager;
	friend class ContactSolver;
	
private:
	CollisionManager* m_CollisionManager;
	ContactSolver*	  m_ContactSolver;
	
	vector<PhysicalBody*> m_vecPhysicalBodies;
	
	static Vector2f GRAVITY;
	
	Vector2f m_TotalForce;
	Vector2f m_TotalTorque;
	
	dfloat m_CFM, m_ERP;
	dbool m_bGravityOn;
	
	dint16 m_VelocityIterations;
};

END_NAMESPACE_DIRAC2D

#endif