/*
 *  PhysicalBody.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 03/05/12.
 *
 */

#include "PhysicalWorld.h"
#include "PhysicalBody.h"
#include "PhysicalShape.h"
#include "contacts/Contact.h"
#include "../geometry/CollisionShape.h"
#include "../collision/broadPhase/BroadPhaseCollisionAlgorithm.h"

BEGIN_NAMESPACE_DIRAC2D

PhysicalBody::PhysicalBody(PhysicalWorld* world) : m_PhysicalWorld(world)
{
	m_Angle = 0.0f;
	m_LinearDamping = 0.0f;
	m_AngularDamping = 0.0f;
	m_AngularVelocity = 0.0f;
	m_BodyType = EBT_DYNAMIC;
	m_Mass = m_InvMass = 1.0f;
	m_I = m_InvI = 1.0f;
	m_bIsAlive = false;
	
	m_SleepTime = 0.0f;
	m_bSleeping    = false;
	m_bSleepingPolicy = false;
	m_PhysicalShapeList = 0;
	m_ContactEdgeList = 0;
}

PhysicalBody::PhysicalBody(const PhysicalBody& other)
{
	m_Centre = other.m_Centre;
	m_Position = other.m_Position;
	m_Velocity = other.m_Velocity;
	m_AngularVelocity = other.m_AngularVelocity;
	m_Angle = other.m_Angle;
	m_LinearDamping = other.m_LinearDamping;
	m_AngularDamping = other.m_AngularDamping;
	m_Transform = other.m_Transform;
	m_BodyType = other.m_BodyType;
	m_Mass = other.m_Mass;
	m_InvMass = other.m_InvMass;
	m_I = other.m_I;
	m_InvI = other.m_InvI;
	m_bIsAlive = other.m_bIsAlive;
		
	m_bSleepingPolicy = other.m_bSleepingPolicy;
	m_bSleeping = other.m_bSleeping;
	m_SleepTime = other.m_SleepTime;
	m_PhysicalWorld = other.m_PhysicalWorld;
	
	PhysicalShape* pShape = other.m_PhysicalShapeList;
	
	m_PhysicalShapeList = 0;
	m_ContactEdgeList = 0;

	// Clone All the Physical Shapes attached to this PhysicalBody
	while (pShape) 
	{
		PhysicalShape* cloneShape = pShape->clone();
		cloneShape->m_Next = m_PhysicalShapeList;
		
		if( m_PhysicalShapeList )
			m_PhysicalShapeList->m_Prev = cloneShape;
		
		m_PhysicalShapeList = cloneShape;
		cloneShape->m_ParentBody = this;
		// Add to Broad Phase
		m_PhysicalWorld->addToBroadPhase(cloneShape);	
		pShape = pShape->m_Next;
	}

	m_Force = other.m_Force;
	m_AABB = other.m_AABB;
}

void PhysicalBody::operator=(const PhysicalBody& other)
{
	m_Centre = other.m_Centre;
	m_Position = other.m_Position;
	m_Velocity = other.m_Velocity;
	m_AngularVelocity = other.m_AngularVelocity;
	m_Angle = other.m_Angle;
	m_LinearDamping = other.m_LinearDamping;
	m_AngularDamping = other.m_AngularDamping;
	m_Transform = other.m_Transform;
	m_BodyType = other.m_BodyType;
	m_Mass = other.m_Mass;
	m_InvMass = other.m_InvMass;
	m_I = other.m_I;
	m_InvI = other.m_InvI;
	m_bIsAlive = other.m_bIsAlive;

	
	m_bSleepingPolicy = other.m_bSleepingPolicy;
	m_bSleeping = other.m_bSleeping;
	m_SleepTime = other.m_SleepTime;
	m_PhysicalWorld = other.m_PhysicalWorld;

	PhysicalShape* pShape = other.m_PhysicalShapeList;
	
	m_PhysicalShapeList = 0;
	
	while (pShape) 
	{
		PhysicalShape* cloneShape = pShape;
		cloneShape->m_Next = m_PhysicalShapeList;
		
		if( m_PhysicalShapeList )
			m_PhysicalShapeList->m_Prev = cloneShape;
		
		m_PhysicalShapeList = cloneShape;
		
		pShape = pShape->m_Next;
	}
	
	m_Force = other.m_Force;
	m_AABB = other.m_AABB;	
}

PhysicalBody* PhysicalBody::clone()
{
	// Create a Clone of the Body
	PhysicalBody* pBody = new(m_PhysicalWorld->m_PhysicalBodyPool->Allocate()) PhysicalBody(*this);
	// Body is not added to the world.
	pBody->m_bIsAlive = false;
	return pBody;
}

// Adds this Body to PhysicalWorld.
void PhysicalBody::addToPhysicalWorld(PhysicalWorld* pWorld)
{
	if( m_bIsAlive ) // return if the body was already added.
		return;
	
	m_bIsAlive = true;
	this->m_Prev = 0;
	this->m_Next = pWorld->m_PhysicalBodyList;
	
	if( pWorld->m_PhysicalBodyList )
	{
		pWorld->m_PhysicalBodyList->m_Prev = this;
	}
	pWorld->m_PhysicalBodyList = this;
}

void PhysicalBody::removeFromPhysicalWorld(PhysicalWorld* pWorld)
{
	if( m_Prev )
	{
		m_Prev->m_Next = m_Next;
	}
	else 
	{
		pWorld->m_PhysicalBodyList = m_Next;
	}
	if( m_Next )
	{
		m_Next->m_Prev = m_Prev;
	}
	m_bIsAlive = false;
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
	
	pShape->m_MassAttributes = pApp.m_MassAttributes;
	
	pShape->m_CollisionShape = pApp.m_CollisionAttributes.m_Shape;
	pShape->m_CollisionFilter = pApp.m_CollisionAttributes.m_Filter;
	
	if( pShape->m_CollisionShape->m_ShapeType == EST_EDGE  )
		m_BodyType = EBT_STATIC;
	
	Matrix3f xForm;
	xForm.translate(pApp.m_PhysicalAttributes.m_Position);	
	xForm.rotate(pApp.m_PhysicalAttributes.m_Angle);

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
	
	m_PhysicalWorld->addToBroadPhase(pShape);

	return pShape;
}

// Update the Body Transform.
/*
  we need to be careful as there might be offsets in the vertices defined for a Shape.
  So the Body Transform should be::
  Tinv * PositionXform * RotXform * T
  where T, Tinv are the local centre offset transforms.
  PositionXform represents the Body's position
  RotXform represents the Body's Angle.
  Implementation Below is the less readable form of the above concatenation only.
 */
void PhysicalBody::updateTransform()
{	
	dfloat cs = cos(m_Angle); dfloat sn = sin(m_Angle);
	
	m_Transform.col1.x = cs; m_Transform.col2.x = -sn;
	m_Transform.col1.y = sn; m_Transform.col2.y = cs;
	m_Transform.col3.x = -cs * m_Centre.x + sn * m_Centre.y + m_Centre.x + m_Position.x;
	m_Transform.col3.y = -sn * m_Centre.x - cs * m_Centre.y + m_Centre.y + m_Position.y;
	
	updateAABB();	
}

PhysicalBody::~PhysicalBody()
{
	dAssert(0);
	PhysicalShape* pShape = m_PhysicalShapeList;
	while( pShape ) 
	{
		delete pShape;
	}
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
			dAssert( m_I > 0.0f );
			dAssert( m_Mass > 0.0f );
			shape = shape->m_Next;
		}
		
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