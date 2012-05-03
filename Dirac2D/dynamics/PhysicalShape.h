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

#ifndef _PHYSICAL_SHAPE_H_
#define _PHYSICAL_SHAPE_H_

BEGIN_NAMESPACE_DIRAC2D

class PhysicalBody;
class CollisionShape;

class PhysicalShape
{
protected:
	PhysicalShape()
	{
	}
	
public:
	PhysicalBody* m_ParentBody;

	dfloat m_Friction;
	dfloat m_Elasticity;
	
	MassAttributes m_MassAttributes;
	
	CollisionShape* m_CollisionShape;
	CollisionFilter m_CollisionFilter;
	
	Vector2f m_Position;
	dfloat m_Angle;

protected:
	// pointer to the next shape from the parent body.
	PhysicalShape* m_NextShape;
	
public:
	friend class PhysicalBody;
};

END_NAMESPACE_DIRAC2D

#endif