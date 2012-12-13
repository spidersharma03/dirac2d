/*
 *  PhysicalAppearance.h
 *  Dirac2D
 *
 *
 */

#include "../definitions.h"
#include "../Settings.h"
#include "../maths/Vector2.h"

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
	dfloat m_Restitution;
	
	PhysicalAttributes()
	{
		m_Angle = 0.0f;
		m_Friction = 0.4f;
		m_Restitution = 0.0f;
	}
};

struct CollisionFilter
{
	CollisionFilter()
	{
		m_CollisionBit = 0xFFFFFFFF;
		m_CollisionMask = 0x0;
	}
	
	CollisionFilter( const CollisionFilter& other)
	{
		m_CollisionBit  = other.m_CollisionBit;
		m_CollisionMask = other.m_CollisionMask;
	}
	
	duint32 m_CollisionBit;
	duint32 m_CollisionMask;
};

struct CollisionAttributes
{
	CollisionShape* m_Shape;
	CollisionFilter m_Filter;
	
	CollisionAttributes()
	{
		m_Shape = 0;
	}
};

struct MassAttributes
{
	dfloat m_Mass;
	dfloat m_I;
	dfloat m_Density;
	Vector2f m_C;
	
	MassAttributes()
	{
		m_Density = 100.0f;
		m_Mass = m_I = 0.0f;
	}
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