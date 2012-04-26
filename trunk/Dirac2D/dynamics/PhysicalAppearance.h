/*
 *  PhysicalAppearance.h
 *  Dirac2D
 *
 *
 */

#include "../definitions.h"
#include "../Settings.h"
#include "../maths/MathUtil.h"

#ifndef _PHYSICAL_APPEARANCE_H_
#define _PHYSICAL_APPEARANCE_H_

BEGIN_NAMESPACE_DIRAC2D

class Shape;

// This Structure Contains all the physical attributes to construct a Physical Body
struct PhysicalAttributes
{
	dfloat m_Friction;
	dfloat e;
};

struct CollisionAttributes
{
	Shape* m_Shape;
};

struct MassAttributes
{
	dfloat m_Mass;
	dfloat m_I;
	Vector2f m_C;
};

class PhysicalAppearance
{
public:
	PhysicalAppearance();
		
public:
	PhysicalAttributes m_PhysicalAttributes;
	CollisionAttributes m_CollisionAttributes;
};

END_NAMESPACE_DIRAC2D

#endif