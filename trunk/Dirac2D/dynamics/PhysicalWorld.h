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

class PhysicalWorld
{
public:
	PhysicalWorld();
	
	PhysicalBody* createPhysicalBody(PhysicalAttributes& pAtt);
	
public:
	
	friend class CollisionManager;
	friend class ContactSolver;
	
private:
	dfloat m_CFM, m_ERP;
	vector<PhysicalBody*> m_vecPhysicalBodies;
	
};

END_NAMESPACE_DIRAC2D

#endif