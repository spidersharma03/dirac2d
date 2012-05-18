/*
 *  PhysicalBody.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 03/05/12.
 *
 */

#include "PhysicalBody.h"
#include "PhysicalShape.h"
#include "../geometry/CollisionShape.h"

BEGIN_NAMESPACE_DIRAC2D

PhysicalBody::PhysicalBody(PhysicalWorld* world)
{
	m_Angle = 0.0f;
	m_LinearDamping = 0.0f;
	m_AngularDamping = 0.0f;
	m_AngularVelocity = 0.0f;
	m_BodyType = EBT_DYNAMIC;
	m_Mass = m_InvMass = 1.0f;
	m_I = m_InvI = 1.0f;
	
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
	pShape->m_ParentBody = this;
	pShape->m_NextShape = 0;
	pShape->m_Elasticity = pApp.m_PhysicalAttributes.m_Elasticity;
	pShape->m_Friction = pApp.m_PhysicalAttributes.m_Friction;
	pShape->m_Angle = pApp.m_PhysicalAttributes.m_Angle;
	pShape->m_Position = pApp.m_PhysicalAttributes.m_Position;
	
	pShape->m_MassAttributes = pApp.m_MassAttributes;
	
	pShape->m_CollisionShape = pApp.m_CollisionAttributes.m_Shape;
	pShape->m_CollisionFilter = pApp.m_CollisionAttributes.m_Filter;
	
	
	m_PhysicalShapeList = pShape;
	m_PhysicalShapeList->m_NextShape = 0;
	
	calculateMassAttributes();
	updateAABB();
	
	return pShape;
}

void PhysicalBody::updateTransform()
{
	m_Transform.rotate(m_Angle);
	m_Transform.translate(m_Centre);
	m_PhysicalShapeList->m_CollisionShape->updateAABB(m_Transform);
	m_AABB = m_PhysicalShapeList->m_CollisionShape->m_AABB;
}

void PhysicalBody::updateAABB()
{
	m_PhysicalShapeList->m_CollisionShape->updateAABB(m_Transform);
	m_AABB = m_PhysicalShapeList->m_CollisionShape->m_AABB;
}

void PhysicalBody::calculateMassAttributes()
{
	PhysicalShape* shape = m_PhysicalShapeList;
	// Calculate Centre of Mass of the PhysicalBody.
	Vector2f sum_mr;
	dfloat sum_m = 0.0f;
	while( shape != 0 )
	{
		shape->calculateMassAttributes();
		sum_m  += shape->m_MassAttributes.m_Mass;
		sum_mr += shape->m_MassAttributes.m_C * shape->m_MassAttributes.m_Mass;
		shape = m_PhysicalShapeList->m_NextShape;
	}
	dAssert( sum_m > 0.0f );
	m_Centre = sum_mr/sum_m;
	
	// Calculate Moment of Inertia and Mass of the PhysicalBody.
	shape = m_PhysicalShapeList;
	m_I = 0.0f;
	m_Mass = 0.0f;
	if( m_BodyType == EBT_DYNAMIC )
	{
		while( shape != 0 )
		{
			Vector2f R = shape->m_MassAttributes.m_C - m_Centre;
			dfloat r2 = R.lengthSquared();
			m_I += ( shape->m_MassAttributes.m_I + shape->m_MassAttributes.m_Mass * r2);
			m_Mass += shape->m_MassAttributes.m_Mass;
			shape = m_PhysicalShapeList->m_NextShape;
		}
		dAssert( m_Mass > 0.0f );
		dAssert( m_I > 0.0f );
		
		m_InvMass = 1.0f/m_Mass;
		m_InvI    = 1.0f/m_I;
	}
	else
	{
		m_InvMass = 0.0f;
		m_InvI    = 0.0f;
	}

	updateAABB();
	m_Transform.transformAsPoint(m_Centre);
}

END_NAMESPACE_DIRAC2D