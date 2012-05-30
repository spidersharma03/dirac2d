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