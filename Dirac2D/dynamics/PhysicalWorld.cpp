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
}
	
PhysicalBody* PhysicalWorld::createPhysicalBody()
{
	PhysicalBody* pBody = new PhysicalBody(this);
	m_vecPhysicalBodies.push_back(pBody);
	return pBody;
}
	
PhysicalBody* PhysicalWorld::createPhysicalBody(PhysicalAppearance& pApp)
{
	PhysicalBody* pBody = new PhysicalBody(this);
	m_vecPhysicalBodies.push_back(pBody);
	return pBody;
}

void PhysicalWorld::Step(dfloat dt)
{
	Vector2f totalForce;
	if( m_bGravityOn )
		totalForce += GRAVITY * m_GravityScale;
	
	// Integrate/Advance the velocities by time step. 
	for( duint32 b=0; b<m_vecPhysicalBodies.size(); b++ )
	{
		if( m_vecPhysicalBodies[b]->m_BodyType == EBT_DYNAMIC )
			m_vecPhysicalBodies[b]->m_Velocity += totalForce * dt;
	}
	// Collision detection	
	m_CollisionManager->update();
	
	// Initialize the Solver.
	m_ContactSolver->buildJacobian();
	// Correct the velocities.
	for( dint32 iter=0; iter<m_VelocityIterations; iter++ )
		m_ContactSolver->correctVelocities();
	
	// Integrate/Advance the positions by time step. 
	for( duint32 b=0; b<m_vecPhysicalBodies.size(); b++ )
	{
		if( m_vecPhysicalBodies[b]->m_BodyType == EBT_DYNAMIC )
		{
			m_vecPhysicalBodies[b]->m_Centre += m_vecPhysicalBodies[b]->m_Velocity * dt;
			m_vecPhysicalBodies[b]->m_Angle  += m_vecPhysicalBodies[b]->m_AngularVelocity * dt;
			m_vecPhysicalBodies[b]->updateTransform();
		}
	}
}

void PhysicalWorld::draw()
{
	if( !m_Renderer )
		return;
	// Draw Physical Bodies
	for( duint32 b=0; b<m_vecPhysicalBodies.size(); b++ )
	{
		PhysicalBody* pBody = m_vecPhysicalBodies[b];
		m_Renderer->setTransform(pBody->m_Transform);
		if( pBody->m_BodyType == EBT_DYNAMIC )
			m_Renderer->setColor(255, 255, 255);
		if( pBody->m_BodyType == EBT_STATIC )
			m_Renderer->setColor(0, 255, 0);
		//m_Renderer->drawShape(pBody->m_PhysicalShapeList->m_CollisionShape);
		m_Renderer->setColor(255, 255, 0);
		
		Matrix3f Identity;
		m_Renderer->setTransform(Identity);
		m_Renderer->drawAABB(pBody->m_AABB);
	}
	// Draw Constraints/Joints
	
	// Draw Contacts
	m_Renderer->setColor(255, 0, 0);
	m_Renderer->setPointSize(4.0f);
	Matrix3f I;
	m_Renderer->setTransform(I);
	Contact* contacts = 0;
	dint32 numContacts = 0;
	m_CollisionManager->getContactList(&contacts, numContacts);
		
	for( dint32 c=0; c<numContacts; c++ )
	{
		Contact& contact = contacts[c];
		m_Renderer->drawPoint(contact.m_Manifold.m_ContactPoints[0].m_Point);
		if( contact.m_Manifold.m_NumContacts > 1 )
			m_Renderer->drawPoint(contact.m_Manifold.m_ContactPoints[1].m_Point);			
	}
}

void PhysicalWorld::setRenderer(Renderer* renderer)
{
	m_Renderer = renderer;
}
	
END_NAMESPACE_DIRAC2D
