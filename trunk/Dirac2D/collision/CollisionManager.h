#include "definitions.h"
#include "maths/MathUtil.h"

#ifndef _COLLISION_MANAGER_H_
#define _COLLISION_MANAGER_H_

BEGIN_NAMESPACE_DIRAC2D

class PhysicalWorld;

class CollisionManager
{
public:
	CollisionManager(PhysicalWorld*);

private:
	PhysicalWorld m_PhysicalWorld;
};

END_NAMESPACE_DIRAC2D

#endif