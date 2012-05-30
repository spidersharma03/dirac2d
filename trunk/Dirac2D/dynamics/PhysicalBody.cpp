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
	
	m_SleepTime = 0.0f;
	m_bSleeping    = false;
	m_bSleepingPolicy = false;
	m_PhysicalShapeList = 0;
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

void PhysicalBody::updateSleepingStatus(dfloat dt)
{
	if( !m_bSleepingPolicy )
		return;
	
	dfloat energy = m_Velocity.lengthSquared() + m_AngularVelocity * m_AngularVelocity;
	if( energy < ENERGY_THRESHOLD )
	{
		m_SleepTime += dt;
	}
	else {
		m_SleepTime = 0.0f;
	}

	if( m_SleepTime > SLEEPING_THRESHOLD )
	{
		m_bSleeping = true;
	}
}

PhysicalShape* PhysicalBody::createPhysicalShape(PhysicalAppearance& pApp)
{
	PhysicalShape* pShape = new PhysicalShape();
	pShape->m_ParentBody = this;
	pShape->m_Restitution = pApp.m_PhysicalAttributes.m_Restitution;
	pShape->m_Friction = pApp.m_PhysicalAttributes.m_Friction;
	pShape->m_Angle = pApp.m_PhysicalAttributes.m_Angle;
	pShape->m_Position = pApp.m_PhysicalAttributes.m_Position;
	
	pShape->m_MassAttributes = pApp.m_MassAttributes;
	
	pShape->m_CollisionShape = pApp.m_CollisionAttributes.m_Shape;
	pShape->m_CollisionFilter = pApp.m_CollisionAttributes.m_Filter;
	
	if( pShape->m_CollisionShape->m_ShapeType == EST_EDGE )
		m_BodyType = EBT_STATIC;
	
	Matrix3f xForm;
	xForm.translate(pShape->m_Position);	
	xForm.rotate(pShape->m_Angle);

	pShape->m_CollisionShape->updateShape(xForm);
	
	pShape->m_Prev = 0;
	pShape->m_Next = m_PhysicalShapeList;
	
	if( m_PhysicalShapeList )
	{
		m_PhysicalShapeList->m_Prev = pShape;
	}
	m_PhysicalShapeList = pShape;
	
	calculateMassAttributes();
	updateAABB();
	
	return pShape;
}

void PhysicalBody::updateTransform()
{
	m_Transform.rotate(m_Angle);
	m_Transform.translate(m_Position);
	
	updateAABB();	
}

void PhysicalBody::updateAABB()
{
	PhysicalShape* pShape = m_PhysicalShapeList;
	while( pShape )
	{
		pShape->m_CollisionShape->updateAABB(m_Transform);
		pShape = pShape->m_Next;
	}
	
	dfloat min_x = 100000.0f;
	dfloat min_y = 100000.0f;
	
	dfloat max_x = -100000.0f;
	dfloat max_y = -100000.0f;
	
	pShape = m_PhysicalShapeList;
	
	dint32 i = 0;
	
	Vector2f vertices[50];
	
	while( pShape )
	{
		vertices[i++] = pShape->m_CollisionShape->m_AABB.m_LowerBounds;
		vertices[i++] = pShape->m_CollisionShape->m_AABB.m_UpperBounds;
		pShape = pShape->m_Next;
	}
	
	pShape = m_PhysicalShapeList;

	for( dint32 j=0; j<i; j++ )
	{
		Vector2f p = vertices[j];
		
		if( min_x > p.x )
			min_x = p.x;
		if( min_y > p.y )
			min_y = p.y;
		
		if( max_x < p.x )
			max_x = p.x;
		if( max_y < p.y )
			max_y = p.y;
	}
	
	m_AABB.m_LowerBounds.set(min_x, min_y);
	m_AABB.m_UpperBounds.set(max_x, max_y);	
}

void PhysicalBody::calculateMassAttributes()
{
	PhysicalShape* shape = m_PhysicalShapeList;
	// Calculate Centre of Mass of the PhysicalBody.
	Vector2f sum_mr;
	dfloat sum_m = 0.0f;
	if( m_BodyType == EBT_DYNAMIC )
	{
		while( shape != 0 )
		{
			shape->calculateMassAttributes();
			sum_m  += shape->m_MassAttributes.m_Mass;
			sum_mr += shape->m_MassAttributes.m_C * shape->m_MassAttributes.m_Mass;
			shape = shape->m_Next;
		}
		dAssert( sum_m > 0.0f );
		m_Centre = sum_mr/sum_m;
	}
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
			shape = shape->m_Next;
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

	//updateAABB();
	//m_Transform.transformAsPoint(m_Centre);
}

END_NAMESPACE_DIRAC2D