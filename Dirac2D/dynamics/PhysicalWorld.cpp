/*
 *  PhysicalWorld.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 03/05/12.
 *
 */

#include "PhysicalWorld.h"
#include "PhysicalBody.h"
#include "../collision/CollisionManager.h"
#include "../dynamics/contacts/ContactSolver.h"

BEGIN_NAMESPACE_DIRAC2D

Vector2f PhysicalWorld::GRAVITY = Vector2f(0.0f,-10.0f);

PhysicalWorld::PhysicalWorld()
{
	m_CollisionManager = new CollisionManager(this);
	m_ContactSolver    = new ContactSolver();
	
	m_bGravityOn = true;
	
	m_CFM = 0.0f;
	m_ERP = 0.0f;
	
	m_VelocityIterations = 10;
}
	
PhysicalBody* PhysicalWorld::createPhysicalBody()
{
	
	return 0;
}
	
PhysicalBody* PhysicalWorld::createPhysicalBody(PhysicalAppearance& pApp)
{
	PhysicalBody* pBody = new PhysicalBody(this,0);
	return pBody;
}

void PhysicalWorld::Step(dfloat dt)
{
	Vector2f totalForce;
	if( m_bGravityOn )
		totalForce += GRAVITY;
	
	// Integrate/Advance the velocities by time step. 
	for( duint32 b=0; b<m_vecPhysicalBodies.size(); b++ )
	{
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
		m_vecPhysicalBodies[b]->m_Centre += m_vecPhysicalBodies[b]->m_Velocity * dt;
		m_vecPhysicalBodies[b]->m_Angle  += m_vecPhysicalBodies[b]->m_AngularVelocity * dt;
		m_vecPhysicalBodies[b]->updateTransform();
	}
}

	
END_NAMESPACE_DIRAC2D
