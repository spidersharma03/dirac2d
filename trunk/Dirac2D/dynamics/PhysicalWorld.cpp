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
#include "../geometry/CollisionShape.h"
#include "../geometry/Edge.h"
#include "../geometry/EdgeChain.h"
#include "../collision/CollisionManager.h"

#include "../collision/broadPhase/BroadPhaseCollisionAlgorithm.h"
#include "../collision/broadPhase/NaiveBoradPhaseCollisionAlgorithm.h"
#include "../collision/broadPhase/DynamicTreeBroadPhaseAlgorithm.h"

#include "../dynamics/contacts/ContactSolver.h"
#include "../dynamics/contacts/Contact.h"

#include "../dynamics/joints/DistantConstraint.h"
#include "../dynamics/joints/HingeConstraint.h"

#include "../draw/Renderer.h"

BEGIN_NAMESPACE_DIRAC2D

Vector2f PhysicalWorld::GRAVITY = Vector2f(0.0f,-10.0f);

PhysicalWorld::PhysicalWorld()
{
	m_CollisionManager = new CollisionManager(this);
	//m_pBroadPhaseAlgorithm = new DynamicTreeBroadPhaseAlgorithm(m_CollisionManager);
	m_pBroadPhaseAlgorithm = new NaiveBroadPhaseCollisionAlgorithm(m_CollisionManager);
	m_ContactSolver    = new ContactSolver(this);
	m_Renderer		   = 0;
	
	m_bGravityOn = true;
	m_GravityScale = 1.0f;
	
	m_CFM = 0.0f;
	m_ERP = 0.0f;
	
	m_VelocityIterations = 10;
	
	m_bWarmStart = true;

	m_bDrawShapes = true;
	m_bDrawBoundingBoxes = false;
	m_bDrawContacts = false;
	m_bDrawConstraints = true;
	m_bDrawCentreOfMass = false;
	
	m_PhysicalBodyList = 0;
	m_ConstraintList   = 0;
	m_ContactList	   = 0;
	
	m_PhysicalBodyPool = new MemoryAllocator<PhysicalBody>(MAX_BODIES);
	
	m_DistanceConstraintPool = new MemoryAllocator<DistanceConstraint>(MAX_BODIES/10);
	m_HingeConstraintPool	 = new MemoryAllocator<HingeConstraint>(MAX_BODIES/10);
	
	m_BroadPhaseNodePool = new MemoryAllocator<BroadPhaseNode>(MAX_PROXIES);
}
	
PhysicalBody* PhysicalWorld::createPhysicalBody()
{
	PhysicalBody* pBody = new(m_PhysicalBodyPool->Allocate()) PhysicalBody(this);
	pBody->addToPhysicalWorld(this);
	return pBody;
}

void PhysicalWorld::deletePhysicalBody(PhysicalBody* pBody)
{
	pBody->removeFromPhysicalWorld(this);	
	// Release Memory. do not call delete on this as we didnt allocate it through new operator.
	pBody->~PhysicalBody();
	m_PhysicalBodyPool->Free(pBody);
}

Constraint* PhysicalWorld::createConstraint(CONSTRAINT_TYPE constraintType)
{
	Constraint* constraint;
	// Create Constraint from the Pool.
	switch (constraintType) 
	{
		case ECT_DISTANCE:
			constraint = new( m_DistanceConstraintPool->Allocate() )DistanceConstraint();
			break;
		default:
			break;
	}
	// Add this Constraint to the world list.
	constraint->m_Prev = 0;
	constraint->m_Next = m_ConstraintList;
	
	if( m_ConstraintList )
	{
		m_ConstraintList->m_Prev = constraint;
	}
	m_ConstraintList = constraint;
	
	return constraint;
}

void PhysicalWorld::deleteConstraint(Constraint* pConstraint)
{
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
	// Broad Phase. here the potential contact pairs are created/culled.
	m_pBroadPhaseAlgorithm->update();
	// Collision detection. here the contacts are created/removed.
	m_CollisionManager->update();
	
	// Initialize the Solver.
	m_ContactSolver->buildJacobian();
	
	// Correct the velocities.
	for( dint32 iter=0; iter<m_VelocityIterations; iter++ )
		m_ContactSolver->correctVelocities();
	
	// Initialize all the Constraints.
	Constraint* pConstraint = m_ConstraintList;
	while (pConstraint) 
	{
		pConstraint->buildJacobian();
		pConstraint = pConstraint->m_Next;
	}

	// Correct the velocities of Physical Bodies due to Constraints.
	pConstraint = m_ConstraintList;
	for( dint32 iter=0; iter<m_VelocityIterations; iter++ )
	{
		while (pConstraint) 
		{
			pConstraint->correctVelocities();
			pConstraint = pConstraint->m_Next;
		}
	}
	
	// Integrate/Advance the positions by time step. 
	pBody = m_PhysicalBodyList;
	while( pBody )
	{
		// Introduce Damping. this is implicit euler based damping. for semi implicit euler use v = v * ( 1 - k * h ), h is time step and k is damping constant.
		pBody->m_Velocity		 = pBody->m_Velocity / ( 1.0f + pBody->m_LinearDamping * dt );
		pBody->m_AngularVelocity = pBody->m_AngularVelocity / ( 1.0f + pBody->m_AngularDamping * dt );
		
		pBody->updateSleepingStatus(dt);
		
		if( pBody->m_BodyType == EBT_DYNAMIC && !pBody->m_bSleeping)
		{
			pBody->m_Position += pBody->m_Velocity * dt;
			pBody->m_Angle    += pBody->m_AngularVelocity * dt;
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
		PhysicalShape* pShape = pBody->m_PhysicalShapeList;
		while( pShape )
		{
			if( m_bDrawShapes )
			{
				Matrix3f xForm =  pBody->m_Transform;// * pBody->m_PhysicalShapeList->m_RotOffsetTransform;
				m_Renderer->setTransform(xForm);
				if( pBody->m_BodyType == EBT_DYNAMIC )
					m_Renderer->setColor(255, 255, 255);
				if( pBody->m_bSleeping )
					m_Renderer->setColor(0, 255, 255);
				if( pBody->m_BodyType == EBT_STATIC )
					m_Renderer->setColor(0, 255, 0);
				
				m_Renderer->drawShape(pShape->m_CollisionShape);
			}
			if( m_bDrawBoundingBoxes )
			{
				m_Renderer->setTransform(Identity);
				m_Renderer->setColor(255, 255, 0);
				AABB2f& aabb = pShape->m_CollisionShape->getAABB();
				m_Renderer->drawAABB(aabb);
				
				if( pShape->m_CollisionShape->getShapeType() == EST_EDGE_CHAIN )
				{
					EdgeChain* edgeChain = (EdgeChain*)pShape->m_CollisionShape;
					for( dint32 e=0; e<edgeChain->getNumEdges(); e++ )
					{
						Edge* edge = edgeChain->getEdge(e);
						m_Renderer->drawAABB(edge->getAABB());
					}
				}
			}
			pShape = pShape->m_Next;
		}
		
		if( m_bDrawBoundingBoxes )
		{
			m_Renderer->setTransform(Identity);
			m_Renderer->setColor(255, 255, 0);
			m_Renderer->drawAABB(pBody->m_AABB);
		}
		// draw centre of mass of the Body
		if( m_bDrawCentreOfMass )
		{
			Matrix3f xForm = pBody->m_Transform;// * pBody->m_PhysicalShapeList->m_RotOffsetTransform;
			m_Renderer->setTransform(xForm);
			m_Renderer->setColor(0, 255, 0);
			p0 = pBody->m_Centre - Vector2f(0.01f, 0.0f); p1 = pBody->m_Centre + Vector2f(0.01f, 0.0f);
			m_Renderer->drawLine(p0, p1);
			p0 = pBody->m_Centre - Vector2f(0.0f, 0.01f); p1 = pBody->m_Centre + Vector2f(0.0f, 0.01f);
			m_Renderer->setColor(255, 0, 0);
			m_Renderer->drawLine(p0, p1);
		}
		
		pBody = pBody->m_Next;
	}
	// Draw Constraints/Joints
	if( m_bDrawConstraints )
	{
		Constraint* pConstraint = m_ConstraintList;
		while (pConstraint) 
		{
			Matrix3f xForm;
			m_Renderer->setTransform(xForm);
			m_Renderer->setColor(200, 200, 150);
			
			if( pConstraint->m_Type == ECT_DISTANCE )
			{
				DistanceConstraint* dc = (DistanceConstraint*)pConstraint;
				Vector2f p0 = dc->m_Anchor1, p1 = dc->m_Anchor2;
				if( dc->m_PhysicalBody1 )
				{
					p0 += dc->m_PhysicalBody1->m_Centre;
					dc->m_PhysicalBody1->getTransform().transformAsPoint(p0);
				}
				if( dc->m_PhysicalBody2 )
				{
					p1 += dc->m_PhysicalBody2->m_Centre;
					dc->m_PhysicalBody2->getTransform().transformAsPoint(p1);
				}
				m_Renderer->drawLine(p0, p1);
			}
			pConstraint = pConstraint->m_Next;
		}
	}
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
	
void PhysicalWorld::addToBroadPhase( PhysicalShape* pShape)
{
	BroadPhaseNode* pNode = 0;
	if( pShape->m_CollisionShape->getShapeType() == EST_EDGE_CHAIN )
	{
		EdgeChain* edgeChain = (EdgeChain*)pShape->m_CollisionShape;
		dint32 numEdges = edgeChain->getNumEdges();
		for (dint32 e=0; e<numEdges; e++) 
		{
			pNode = new(m_BroadPhaseNodePool->Allocate()) BroadPhaseNode();
			pNode->m_PhysicalShape = pShape;
			pNode->m_CollisionShape = (CollisionShape*)edgeChain->getEdge(e);
			pNode->m_AABB = pNode->m_CollisionShape->getAABB();
			m_pBroadPhaseAlgorithm->addBroadPhaseNode(pNode);
		}
	}
	else 
	{
		pNode = new(m_BroadPhaseNodePool->Allocate()) BroadPhaseNode();
		pNode->m_PhysicalShape = pShape;
		pNode->m_CollisionShape = pShape->m_CollisionShape;
		pNode->m_AABB = pShape->m_CollisionShape->getAABB();
		m_pBroadPhaseAlgorithm->addBroadPhaseNode(pNode);
	}
}

PhysicalBody* PhysicalWorld::pickBodyFromPoint(Vector2f p)
{
	PhysicalBody* pBody = m_PhysicalBodyList;
	while (pBody) 
	{
		Matrix3f xForm = pBody->getTransform();
		Vector2f testPoint = p;
		// Transform the Point into Body's space.
		testPoint *= xForm;
		PhysicalShape* pShape = pBody->m_PhysicalShapeList;
		while (pShape) 
		{
			dbool bRes = pShape->m_CollisionShape->isPointInside(testPoint);
			if( bRes )
				return pBody;
			pShape = pShape->m_Next;
		}
		pBody = pBody->m_Next;
	}
	return 0;
}

END_NAMESPACE_DIRAC2D
