/*
 *  PhysicalWorld.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 03/05/12.
 *
 */

#include "PhysicalWorld.h"
#include "PhysicalBody.h"
#include "PhysicalShape.h"
#include "../collision/CollisionManager.h"
#include "../dynamics/contacts/ContactSolver.h"
#include "../dynamics/contacts/Contact.h"
#include "../draw/Renderer.h"

BEGIN_NAMESPACE_DIRAC2D

Vector2f PhysicalWorld::GRAVITY = Vector2f(0.0f,-10.0f);

PhysicalWorld::PhysicalWorld()
{
	m_CollisionManager = new CollisionManager(this);
	m_ContactSolver    = new ContactSolver(this);
	m_Renderer		   = 0;
	
	m_bGravityOn = true;
	m_GravityScale = 1.0f;
	
	m_CFM = 0.0f;
	m_ERP = 0.0f;
	
	m_VelocityIterations = 10;
	
	m_ContactList = 0;
	m_bWarmStart = true;

	m_bDrawShapes = true;
	m_bDrawBoundingBoxes = false;
	m_bDrawContacts = true;
	m_bDrawConstraints = false;
	m_bDrawCentreOfMass = false;
	
	m_PhysicalBodyList = 0;
	m_PhysicalBodyPool = new MemoryAllocator<PhysicalBody>(MAX_BODIES);
}
	
PhysicalBody* PhysicalWorld::createPhysicalBody()
{
	PhysicalBody* pBody = new(m_PhysicalBodyPool->Allocate()) PhysicalBody(this);
	
	pBody->m_Prev = 0;
	pBody->m_Next = m_PhysicalBodyList;
	
	if( m_PhysicalBodyList )
	{
		m_PhysicalBodyList->m_Prev = pBody;
	}
	m_PhysicalBodyList = pBody;
	
	return pBody;
}

void PhysicalWorld::deletePhysicalBody(PhysicalBody* pBody)
{
	PhysicalBody* prevBody = pBody->m_Prev;
	PhysicalBody* nextBody = pBody->m_Next;
	
	if( prevBody )
	{
		prevBody->m_Next = nextBody;
	}
	else 
	{
		m_PhysicalBodyList = nextBody;
	}
	
	
	if( nextBody )
	{
		nextBody->m_Prev = prevBody;
	}
	
	
	m_PhysicalBodyPool->Free(pBody);
}

PhysicalBody* PhysicalWorld::createPhysicalBody(PhysicalAppearance& pApp)
{
	PhysicalBody* pBody = new(m_PhysicalBodyPool->Allocate()) PhysicalBody(this);
	
	return pBody;
}

void PhysicalWorld::Step(dfloat dt)
{
	Vector2f totalForce;
	if( m_bGravityOn )
		totalForce += GRAVITY * m_GravityScale;
	
	// Integrate/Advance the velocities by time step. 
	PhysicalBody* pBody = m_PhysicalBodyList;
	while( pBody )
	{
		if( pBody->m_BodyType == EBT_DYNAMIC )
			pBody->m_Velocity += totalForce * dt;
		pBody = pBody->m_Next;
	}
	// Collision detection	
	m_CollisionManager->update();
	
	// Initialize the Solver.
	m_ContactSolver->buildJacobian();
	
	// Correct the velocities.
	for( dint32 iter=0; iter<m_VelocityIterations; iter++ )
		m_ContactSolver->correctVelocities();
	
	// Integrate/Advance the positions by time step. 
	pBody = m_PhysicalBodyList;
	while( pBody )
	{
		pBody->updateSleepingStatus(dt);
		
		if( pBody->m_BodyType == EBT_DYNAMIC && !pBody->m_bSleeping)
		{
			pBody->m_Centre += pBody->m_Velocity * dt;
			pBody->m_Angle  += pBody->m_AngularVelocity * dt;
			pBody->updateTransform();
		}
		pBody = pBody->m_Next;
	}
	
}

void PhysicalWorld::draw()
{
	if( !m_Renderer )
		return;
	// Draw Physical Bodies
	PhysicalBody *pBody = m_PhysicalBodyList;
	Vector2f p0, p1;

	Matrix3f Identity;

	while( pBody )
	{
		if( m_bDrawShapes )
		{
			m_Renderer->setTransform(pBody->m_Transform);
			if( pBody->m_BodyType == EBT_DYNAMIC )
				m_Renderer->setColor(255, 255, 255);
			if( pBody->m_bSleeping )
				m_Renderer->setColor(0, 255, 255);
			if( pBody->m_BodyType == EBT_STATIC )
				m_Renderer->setColor(0, 255, 0);
			
			m_Renderer->drawShape(pBody->m_PhysicalShapeList->m_CollisionShape);
		}
		// draw centre of mass
		if( m_bDrawCentreOfMass )
		{
			m_Renderer->setTransform(Identity);
			m_Renderer->setColor(0, 255, 0);
			p0 = pBody->m_Centre - Vector2f(0.01f, 0.0f); p1 = pBody->m_Centre + Vector2f(0.01f, 0.0f);
			m_Renderer->drawLine(p0, p1);
			p0 = pBody->m_Centre - Vector2f(0.0f, 0.01f); p1 = pBody->m_Centre + Vector2f(0.0f, 0.01f);
			m_Renderer->setColor(255, 0, 0);
			m_Renderer->drawLine(p0, p1);
		}
		
		if( m_bDrawBoundingBoxes )
		{
			m_Renderer->setTransform(Identity);
			m_Renderer->setColor(255, 255, 0);
			m_Renderer->drawAABB(pBody->m_AABB);
		}
		pBody = pBody->m_Next;
	}
	// Draw Constraints/Joints
	
	// Draw Contacts
	if( m_bDrawContacts )
	{
		m_Renderer->setTransform(Identity);
		m_Renderer->setColor(255, 0, 0);
		m_Renderer->setPointSize(4.0f);
		
		Contact* contact = m_ContactList;
		
		while(contact)
		{
			if( contact->m_NumContactConstraints == 0 )
			{
				contact = contact->m_Next;
				continue;
			}
			
			m_Renderer->drawPoint(contact->m_Manifold.m_ContactPoints[0].m_Point);
			if( contact->m_NumContactConstraints > 1 )
				m_Renderer->drawPoint(contact->m_Manifold.m_ContactPoints[1].m_Point);
			
			contact = contact->m_Next;
		}
	}
}

void PhysicalWorld::setRenderer(Renderer* renderer)
{
	m_Renderer = renderer;
}
	
END_NAMESPACE_DIRAC2D
