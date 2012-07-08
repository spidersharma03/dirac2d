/*
 *  PhysicalShape.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 07/05/12.
 *
 */

#include "PhysicalShape.h"
#include "../geometry/Box.h"
#include "../geometry/Circle.h"
#include "../geometry/Capsule.h"
#include "../common.h"

BEGIN_NAMESPACE_DIRAC2D

PhysicalShape::PhysicalShape(const PhysicalShape& other)
{
	m_Friction = other.m_Friction;
	m_Restitution = other.m_Restitution;
	m_MassAttributes = other.m_MassAttributes;
	m_CollisionFilter = other.m_CollisionFilter;
	m_CollisionShape = other.m_CollisionShape->clone();
	m_ParentBody     = other.m_ParentBody;
}

void PhysicalShape::operator=(PhysicalShape& other)
{
	m_Friction = other.m_Friction;
	m_Restitution = other.m_Restitution;
	m_MassAttributes = other.m_MassAttributes;
	m_CollisionFilter = other.m_CollisionFilter;
	m_CollisionShape = other.m_CollisionShape;
	m_ParentBody     = other.m_ParentBody;
}


PhysicalShape::~PhysicalShape()
{
	delete m_CollisionShape;
}

PhysicalShape* PhysicalShape::clone()
{
	return new PhysicalShape(*this);
}

void PhysicalShape::setAsBox(dfloat width, dfloat height)
{
	m_CollisionShape = new Box(width, height);
}
	
void PhysicalShape::setAsCircle(dfloat radius)
{
	m_CollisionShape = new Circle(radius);
}
	
void PhysicalShape::setAsCapsule( dfloat radius, dfloat height)
{
	m_CollisionShape = new Capsule(radius, height);
}

void PhysicalShape::calculateMassAttributes()
{
	dAssert( m_MassAttributes.m_Density > 0.0f );
	m_MassAttributes.m_Mass = m_MassAttributes.m_Density * m_CollisionShape->m_Area;
	m_MassAttributes.m_I = m_MassAttributes.m_Mass * m_CollisionShape->m_I;
	m_MassAttributes.m_C =  m_CollisionShape->m_Centroid;
}

END_NAMESPACE_DIRAC2D