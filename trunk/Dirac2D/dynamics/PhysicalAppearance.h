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

class CollisionShape;

/*
 All the Attributes related to the Physical Shape are declared. PhysicalAppearance is a Collection of all the Attributes.
 PhysicalAttributes contains all the attributes related to PhysicalShape. eg friction, elasticity.
 CollisionAttributes contains Shape data and Collision Filters
 MassAttributes contains mass, moment of inertia, density and centre of mass of the PhysicalShape.
 */

// This Structure Contains all the physical attributes to construct a Physical Shape
struct PhysicalAttributes
{
	Vector2f m_Position;
	dfloat m_Angle;
	dfloat m_Friction;
	dfloat m_Elasticity;
};

struct CollisionFilter
{
};

struct CollisionAttributes
{
	CollisionShape* m_Shape;
	CollisionFilter m_Filter;
};

struct MassAttributes
{
	dfloat m_Mass;
	dfloat m_I;
	dfloat m_Density;
	Vector2f m_C;
};

class PhysicalAppearance
{
public:
	PhysicalAppearance()
	{
	}
		
public:
	PhysicalAttributes m_PhysicalAttributes;
	CollisionAttributes m_CollisionAttributes;
	MassAttributes m_MassAttributes;
};

END_NAMESPACE_DIRAC2D

#endif