/*
 *  PhysicalBody.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 03/05/12.
 *
 */

#include "PhysicalBody.h"
#include "PhysicalShape.h"

BEGIN_NAMESPACE_DIRAC2D

PhysicalBody::PhysicalBody(PhysicalWorld* world, PhysicalShape* shape)
{
	m_Angle = 0.0f;
	m_LinearDamping = 0.0f;
	m_AngularDamping = 0.0f;
	m_Type = EBT_DYNAMIC;
	m_Mass = m_InvMass = 1.0f;
	m_I = m_InvI = 1.0f;
	
	PhysicalAppearance pApp;
	pApp.m_CollisionAttributes.m_Shape = shape->m_CollisionShape;
	//PhysicalShape* pShape = new PhysicalShape();
}

void PhysicalBody::applyForce( Vector2f& force )
{
}

void PhysicalBody::applyForce( Vector2f& force, Vector2f& point )
{
}

void PhysicalBody::applyImpulse( Vector2f& impulse )
{
}

void PhysicalBody::applyImpulse( Vector2f& impulse, Vector2f& point )
{
}

void PhysicalBody::applyTorque( Vector2f& torque )
{
}

PhysicalShape* PhysicalBody::createPhysicalShape(PhysicalAppearance& pApp)
{
	PhysicalShape* pShape = new PhysicalShape();
	pShape->m_NextShape = 0;
	pShape->m_Elasticity = pApp.m_PhysicalAttributes.m_Elasticity;
	pShape->m_Friction = pApp.m_PhysicalAttributes.m_Friction;
	pShape->m_Angle = pApp.m_PhysicalAttributes.m_Angle;
	pShape->m_Position = pApp.m_PhysicalAttributes.m_Position;
	
	pShape->m_MassAttributes = pApp.m_MassAttributes;
	
	pShape->m_CollisionShape = pApp.m_CollisionAttributes.m_Shape;
	pShape->m_CollisionFilter = pApp.m_CollisionAttributes.m_Filter;
	
	return pShape;
}

void PhysicalBody::updateTransform()
{
}

END_NAMESPACE_DIRAC2D