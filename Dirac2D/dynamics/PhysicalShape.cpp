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

BEGIN_NAMESPACE_DIRAC2D

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

END_NAMESPACE_DIRAC2D