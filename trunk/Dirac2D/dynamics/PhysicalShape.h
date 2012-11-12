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
class BroadPhaseNode;

class PhysicalShape
{
protected:
	PhysicalShape()
	{
		m_Friction = 0.0f;
		m_Restitution = 0.0f;
		m_Next = m_Prev = 0;
		m_CollisionShape = 0;
        m_bIsSensor = false;
	}
	
	PhysicalShape(const PhysicalShape& other);

	void operator=(PhysicalShape& other);

	PhysicalShape* clone();

	void setAsBox(dfloat width, dfloat height);
	
	void setAsCircle(dfloat radius);
	
	void setAsCapsule( dfloat radius, dfloat height);
	
	void calculateMassAttributes();
	
    dbool m_bIsSensor;
public:
    virtual ~PhysicalShape();

	PhysicalBody* m_ParentBody;

	dfloat m_Friction;
	dfloat m_Restitution;
	
	MassAttributes m_MassAttributes;
	
	CollisionShape* m_CollisionShape;
	CollisionFilter m_CollisionFilter;
    
	// pointer to the next shape from the parent body.
	PhysicalShape* m_Next;
	PhysicalShape* m_Prev;

    inline dbool isSensor()
    {
        return m_bIsSensor;
    }
public:
	friend class PhysicalBody;
};

END_NAMESPACE_DIRAC2D

#endif