/*
 *  PhysicalWorld.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 03/05/12.
 *
 */

#include "dynamics/PhysicalWorld.h"
#include "dynamics/PhysicalBody.h"
#include "dynamics/PhysicalShape.h"
#include "geometry/CollisionShape.h"
#include "geometry/Edge.h"
#include "geometry/EdgeChain.h"
#include "collision/CollisionManager.h"

#include "collision/broadPhase/BroadPhaseCollisionAlgorithm.h"
#include "collision/broadPhase/NaiveBoradPhaseCollisionAlgorithm.h"
#include "collision/broadPhase/DynamicTreeBroadPhaseAlgorithm.h"

#include "dynamics/contacts/ContactSolver.h"
#include "dynamics/contacts/Contact.h"

#include "dynamics/joints/DistantConstraint.h"
#include "dynamics/joints/MouseConstraint.h"
#include "dynamics/joints/HingeConstraint.h"
#include "dynamics/joints/WeldConstraint.h"
#include "dynamics/joints/CatenaryConstraint.h"
#include "dynamics/joints/CatenaryConstraintFixedRotation.h"
#include "dynamics/joints/LineConstraint.h"
#include "dynamics/joints/PrismaticConstraint.h"
#include "dynamics/joints/WheelConstraint.h"
#include "dynamics/joints/PulleyConstraint.h"
#include "dynamics/joints/MotorConstraint.h"
#include "dynamics/joints/MinMaxConstraint.h"


#include "draw/Renderer.h"

#ifndef WIN32 
#include <sys/time.h>
#ifdef IOS
#else
#include<GLUT/glut.h>
#endif
#else
#include<GL/glut.h>
#endif

BEGIN_NAMESPACE_DIRAC2D

Vector2f PhysicalWorld::GRAVITY = Vector2f(0.0f,-1.0f);

PhysicalWorld::PhysicalWorld()
{
    m_pCollisionListener = new CCollisionListener(); // Default Collision Listener
	m_pRayIntersectionCallback = 0;
	
	m_CollisionManager = new CollisionManager(this);
	m_pBroadPhaseAlgorithm = new DynamicTreeBroadPhaseAlgorithm(m_CollisionManager);
	//m_pBroadPhaseAlgorithm = new NaiveBroadPhaseCollisionAlgorithm(m_CollisionManager);
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
	m_bDrawContacts = true;
	m_bDrawConstraints = true;
	m_bDrawCentreOfMass = true;
	
	m_PhysicalBodyList = 0;
	m_ConstraintList   = 0;
	m_ContactList	   = 0;
	
    m_pBlockAllocator  = new MemoryBlockAllocator();
	m_PhysicalBodyPool = new MemoryAllocator<PhysicalBody>(MAX_BODIES);
	m_PhysicaShapePool = new MemoryAllocator<PhysicalShape>(MAX_PHYSICAL_SHAPES);
    
	m_DistanceConstraintPool = new MemoryAllocator<DistanceConstraint>(MAX_BODIES/10);
	m_HingeConstraintPool	 = new MemoryAllocator<HingeConstraint>(MAX_BODIES/10);
	m_WeldConstraintPool     = new MemoryAllocator<WeldConstraint>(MAX_BODIES/10);
	m_CatenaryConstraintPool     = new MemoryAllocator<CatenaryConstraint>(MAX_BODIES/10);
	m_CatenaryConstraintFixedRotationPool     = new MemoryAllocator<CatenaryConstraintFixedRotation>(MAX_BODIES/10);
	m_LineConstraintPool     = new MemoryAllocator<LineConstraint>(MAX_BODIES/10);
	m_WheelConstraintPool    = new MemoryAllocator<WheelConstraint>(MAX_BODIES/10);
	m_MotorConstraintPool    = new MemoryAllocator<MotorConstraint>(MAX_BODIES/10);
	m_PrismaticConstraintPool = new MemoryAllocator<PrismaticConstraint>(MAX_BODIES/10);
	m_PulleyConstraintPool    = new MemoryAllocator<PulleyConstraint>(MAX_BODIES/10);
	m_MinMaxConstraintPool    = new MemoryAllocator<MinMaxConstraint>(MAX_BODIES/10);
	
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
    // Remove All the Contacts attached to this body.
    ContactEdge* cEdge = pBody->m_ContactEdgeList;
    while (cEdge) 
    {
        ContactEdge* ce = cEdge;
        cEdge = ce->m_Next;
        m_CollisionManager->deleteContact(ce->contact);
    }
    // Remove All the Constraints attached to this body.
    ConstraintEdge* constraintEdge = pBody->m_ConstraintEdgeList;
    while (constraintEdge) 
    {
        ConstraintEdge* ce = constraintEdge;
        constraintEdge = ce->m_Next;
        deleteConstraint(ce->m_pConstraint);
    }
    
    // Remove All the Physical Shapes attached to this body.
    PhysicalShape* pShape = pBody->m_PhysicalShapeList;
    while (pShape) 
    {
        PhysicalShape* pShape_ = pShape;
        pShape = pShape_->m_Next;
        pBody->deletePhysicalShape(pShape_);
    }
    
	pBody->removeFromPhysicalWorld(this);	
	// Release Memory. do not call delete on this as we didnt allocate it through new operator.
	pBody->~PhysicalBody();
	m_PhysicalBodyPool->Free(pBody);
}

Constraint* PhysicalWorld::createConstraint(const ConstraintInfo& constraintInfo)
{
	Constraint* constraint;
	// Create Constraint from the Pool.
	switch (constraintInfo.m_Type) 
	{
		case ECT_DISTANCE:
			constraint = new( m_pBlockAllocator->Allocate(sizeof(DistanceConstraint)) )DistanceConstraint((DistanceConstraintInfo&)constraintInfo);
			break;
		case ECT_MOUSE:
			constraint = new MouseConstraint((MouseConstraintInfo&)constraintInfo);
			break;
		case ECT_HINGE:
			constraint = new( m_HingeConstraintPool->Allocate() )HingeConstraint((HingeConstraintInfo&)constraintInfo);
			break;
		case ECT_WELD:
			constraint = new( m_WeldConstraintPool->Allocate() )WeldConstraint((WeldConstraintInfo&)constraintInfo);
			break;
		case ECT_CATENARY:
			constraint = new( m_CatenaryConstraintPool->Allocate() )CatenaryConstraint((CatenaryConstraintInfo&)constraintInfo);
			break;
		case ECT_CATENARY_FIXED_ROTATION:
			constraint = new( m_CatenaryConstraintFixedRotationPool->Allocate() )
            CatenaryConstraintFixedRotation((CatenaryConstraintFixedRotationInfo&)constraintInfo);
			break;
		case ECT_LINE:
			constraint = new( m_LineConstraintPool->Allocate() )LineConstraint((LineConstraintInfo&)constraintInfo);
			break;
		case ECT_WHEEL:
			constraint = new( m_WheelConstraintPool->Allocate() )WheelConstraint((WheelConstraintInfo&)constraintInfo);
			break;
		case ECT_PRISMATIC:
			constraint = new( m_PrismaticConstraintPool->Allocate() )PrismaticConstraint((PrismaticConstraintInfo&)constraintInfo);
			break;
		case ECT_PULLEY:
			constraint = new( m_PulleyConstraintPool->Allocate() )PulleyConstraint((PulleyConstraintInfo&)constraintInfo);
			break;
		case ECT_MIN_MAX:
			constraint = new( m_MinMaxConstraintPool->Allocate() )MinMaxConstraint((MinMaxConstraintInfo&)constraintInfo);
			break;
		case ECT_MOTOR:
			constraint = new( m_MotorConstraintPool->Allocate() )MotorConstraint((MotorConstraintInfo&)constraintInfo);
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
	
    
    PhysicalBody* pBody1 = constraint->m_PhysicalBody1;
    PhysicalBody* pBody2 = constraint->m_PhysicalBody2;
    
    // Add this constraint to Physical Bodies constraint list
    // Add to body 1
	constraint->m_ConstraintEdge1.m_pConstraint = constraint;
	constraint->m_ConstraintEdge1.pBody = constraint->m_PhysicalBody2;
	
	constraint->m_ConstraintEdge1.m_Prev = 0;
    if( pBody1 )
        constraint->m_ConstraintEdge1.m_Next = pBody1->m_ConstraintEdgeList;
	
    if( pBody1 )
    {
        if( pBody1->m_ConstraintEdgeList )
        {
            pBody1->m_ConstraintEdgeList->m_Prev = &constraint->m_ConstraintEdge1;
        }
        pBody1->m_ConstraintEdgeList = &constraint->m_ConstraintEdge1;
	}
    // Add to body 2
	constraint->m_ConstraintEdge2.m_pConstraint = constraint;
	constraint->m_ConstraintEdge2.pBody = pBody1;
    
	constraint->m_ConstraintEdge2.m_Prev = 0;
    if( pBody2 )
        constraint->m_ConstraintEdge2.m_Next = pBody2->m_ConstraintEdgeList;
	
    if( pBody2 )
    {
	if( pBody2->m_ConstraintEdgeList )
	{
		pBody2->m_ConstraintEdgeList->m_Prev = &constraint->m_ConstraintEdge2;
	}
        pBody2->m_ConstraintEdgeList = &constraint->m_ConstraintEdge2;
    }
    
	return constraint;
}

void PhysicalWorld::deleteConstraint(Constraint* pConstraint)
{
    switch(pConstraint->m_Type)
    {
    case ECT_DISTANCE:
        m_DistanceConstraintPool->Free((DistanceConstraint*)pConstraint);    
        break;
    case ECT_HINGE:
        m_HingeConstraintPool->Free((HingeConstraint*)pConstraint);
        break;
    case ECT_WELD:
        m_WeldConstraintPool->Free((WeldConstraint*)pConstraint);
        break;
    case ECT_CATENARY:
        m_CatenaryConstraintPool->Free((CatenaryConstraint*)pConstraint);
        break;
    case ECT_CATENARY_FIXED_ROTATION:
        m_CatenaryConstraintFixedRotationPool->Free((CatenaryConstraintFixedRotation*)pConstraint);
        break;
    case ECT_LINE:
        m_LineConstraintPool->Free((LineConstraint*)pConstraint);
        break;
    case ECT_WHEEL:
        m_WheelConstraintPool->Free((WheelConstraint*)pConstraint);
        break;
    case ECT_PRISMATIC:
        m_PrismaticConstraintPool->Free((PrismaticConstraint*)pConstraint);
        break;
    case ECT_PULLEY:
        m_PulleyConstraintPool->Free((PulleyConstraint*)pConstraint);
        break;
    case ECT_MIN_MAX:
        m_MinMaxConstraintPool->Free((MinMaxConstraint*)pConstraint);
        break;
    case ECT_MOTOR:
        m_MotorConstraintPool->Free((MotorConstraint*)pConstraint);
        break;
    default:
        break;
	}
    // Remove constraint from Physical World
    Constraint* prevConstraint = pConstraint->m_Prev;
	Constraint* nextConstraint = pConstraint->m_Next;
	
	if( prevConstraint )
	{
		prevConstraint->m_Next = nextConstraint;
	}
	else 
	{
		m_ConstraintList = nextConstraint;
	}
    
    
	if( nextConstraint )
	{
		nextConstraint->m_Prev = prevConstraint;
	}
    
    // Remove Constraint from body1's list
	PhysicalBody* pBody1 = pConstraint->m_PhysicalBody1;
    if( !pBody1 )
        return;
    
	if( pConstraint->m_ConstraintEdge1.m_Prev )
	{
		pConstraint->m_ConstraintEdge1.m_Prev->m_Next = pConstraint->m_ConstraintEdge1.m_Next;
	}
	else 
	{
        pBody1->m_ConstraintEdgeList = pConstraint->m_ConstraintEdge1.m_Next;
	}
	if( pConstraint->m_ConstraintEdge1.m_Next )
	{
		pConstraint->m_ConstraintEdge1.m_Next->m_Prev = pConstraint->m_ConstraintEdge1.m_Prev;
	}
	
	// Remove Constraint from body2's list
	PhysicalBody* pBody2 = pConstraint->m_PhysicalBody2;
    if( !pBody2 )
        return;
    
	if( pConstraint->m_ConstraintEdge2.m_Prev )
	{
		pConstraint->m_ConstraintEdge2.m_Prev->m_Next = pConstraint->m_ConstraintEdge2.m_Next;
	}
	else 
	{
		pBody2->m_ConstraintEdgeList = pConstraint->m_ConstraintEdge2.m_Next;
	}
	if( pConstraint->m_ConstraintEdge2.m_Next )
	{
		pConstraint->m_ConstraintEdge2.m_Next->m_Prev = pConstraint->m_ConstraintEdge2.m_Prev;
	}
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
	
	// Initialize all the Constraints.
	Constraint* pConstraint = m_ConstraintList;
	while (pConstraint) 
	{
		pConstraint->buildJacobian(dt);
		pConstraint = pConstraint->m_Next;
	}

	// Initialize the Contact Solver.
	m_ContactSolver->buildJacobian(dt);
	
	// Correct the velocities of Physical Bodies due to Constraints.
	for( dint32 iter=0; iter<m_VelocityIterations; iter++ )
	{
		pConstraint = m_ConstraintList;
		while (pConstraint) 
		{
			pConstraint->correctVelocities();
            pConstraint = pConstraint->m_Next;
            
            // Correct the velocities of the Physical Bodies due to Contacts.
            m_ContactSolver->correctVelocities();
		}
	}
	
	// Integrate/Advance the positions by time step. 
	pBody = m_PhysicalBodyList;
	while( pBody )
	{
		// Limit Body's Linear and Angular velocities.
		pBody->m_AngularVelocity = pBody->m_AngularVelocity > MAX_ANGULAR_SPEED ? MAX_ANGULAR_SPEED : pBody->m_AngularVelocity;
		
		// Introduce Damping. this is implicit euler based damping. for semi implicit euler use v = v * ( 1 - k * h ), h is time step and k is damping constant.
		pBody->m_Velocity		 = pBody->m_Velocity / ( 1.0f + pBody->m_LinearDamping * dt );
		pBody->m_AngularVelocity = pBody->m_AngularVelocity / ( 1.0f + pBody->m_AngularDamping * dt );
		
		pBody->updateSleepingStatus(dt);
		
		if( ( pBody->m_BodyType == EBT_DYNAMIC || pBody->m_BodyType == EBT_KINEMATIC )  && !pBody->m_bSleeping)
		{
			pBody->m_Position += pBody->m_Velocity * dt;
			if( !pBody->m_bFixedRotation )
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
				Matrix3f xForm =  pBody->m_Transform;
#ifndef IOS
                glPushMatrix();
#endif
				m_Renderer->setTransform(xForm);
				if( pBody->m_BodyType == EBT_DYNAMIC )
					m_Renderer->setColor(255, 255, 255);
				if( pBody->m_bSleeping )
					m_Renderer->setColor(0, 255, 255);
				if( pBody->m_BodyType == EBT_STATIC )
					m_Renderer->setColor(0, 255, 0);
				if( pBody->m_BodyType == EBT_KINEMATIC )
					m_Renderer->setColor(0, 255, 255);
				
				m_Renderer->drawShape(pShape->m_CollisionShape);
#ifndef IOS
                glPopMatrix();
#endif
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
#ifndef IOS
			glPushMatrix();
#endif
            m_Renderer->setTransform(xForm);
			m_Renderer->setColor(0, 255, 0);
			p0 = pBody->m_Centre - Vector2f(0.01f, 0.0f); p1 = pBody->m_Centre + Vector2f(0.01f, 0.0f);
			m_Renderer->drawLine(p0, p1);
			p0 = pBody->m_Centre - Vector2f(0.0f, 0.01f); p1 = pBody->m_Centre + Vector2f(0.0f, 0.01f);
			m_Renderer->setColor(255, 0, 0);
			m_Renderer->drawLine(p0, p1);
#ifndef IOS
            glPopMatrix();
#endif
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
			if( pConstraint->m_Type == ECT_MOUSE )
			{
				MouseConstraint* mc = (MouseConstraint*)pConstraint;
				Vector2f p0, p1;
				if( mc->m_PhysicalBody1 )
				{
					p0 = mc->getMousePosition();
					p1 = mc->getTargetPickPosition();
					m_Renderer->drawLine(p0, p1);
					
					m_Renderer->setColor(255, 255, 0);
					m_Renderer->setPointSize(4.0f);
					m_Renderer->drawPoint(p1);
					m_Renderer->setPointSize(1.0f);
					m_Renderer->setColor(255, 255, 255);
				}
			}
			if( pConstraint->m_Type == ECT_MIN_MAX )
			{
				MinMaxConstraint* mc = (MinMaxConstraint*)pConstraint;
				Vector2f p0 = mc->m_Anchor1, p1 = mc->m_Anchor2;
				if( mc->m_PhysicalBody1 )
				{
					p0 += mc->m_PhysicalBody1->m_Centre;
					mc->m_PhysicalBody1->getTransform().transformAsPoint(p0);
				}
				if( mc->m_PhysicalBody2 )
				{
					p1 += mc->m_PhysicalBody2->m_Centre;
					mc->m_PhysicalBody2->getTransform().transformAsPoint(p1);
				}
				m_Renderer->drawLine(p0, p1);
			}
			if( pConstraint->m_Type == ECT_HINGE || pConstraint->m_Type == ECT_WELD )
			{
				HingeConstraint* hc = (HingeConstraint*)pConstraint;
				Vector2f anchorPoint;// = hc->m_Anchor;
				Vector2f p0, p1;
				if( hc->m_PhysicalBody1 )
				{
					Vector2f c = hc->m_PhysicalBody1->m_Centre;
					hc->m_PhysicalBody1->getTransform().transformAsPoint(c);
					p0 = c;
					anchorPoint = c + hc->m_r1;
				}
				if( hc->m_PhysicalBody2 )
				{
					Vector2f c = hc->m_PhysicalBody2->m_Centre;
					hc->m_PhysicalBody2->getTransform().transformAsPoint(c);
					p1 = c;
					anchorPoint = c + hc->m_r2;
				}
				if( hc->m_PhysicalBody1 )
					m_Renderer->drawLine(p0, anchorPoint);
				if( hc->m_PhysicalBody2 )
					m_Renderer->drawLine(p1, anchorPoint);
				m_Renderer->setColor(255, 0, 255);
				m_Renderer->setPointSize(3.0f);
				m_Renderer->drawPoint(anchorPoint);
			}
			if( pConstraint->m_Type == ECT_CATENARY || pConstraint->m_Type == ECT_CATENARY_FIXED_ROTATION )
			{
				CatenaryConstraint* cc = (CatenaryConstraint*)pConstraint;
				m_Renderer->setColor(255, 0, 255);
				m_Renderer->setPointSize(3.0f);
				m_Renderer->drawPoint(cc->m_FixedPoint1);
				m_Renderer->drawPoint(cc->m_FixedPoint2);
				m_Renderer->setPointSize(1.0f);
				
				Vector2f c = cc->m_PhysicalBody1->m_Position;
				Vector2f ap = cc->m_Anchor;
				cc->m_PhysicalBody1->getTransform().transformAsVector(ap);
				c += ap;
				m_Renderer->setColor(255, 255, 255);
				m_Renderer->drawLine(c, cc->m_FixedPoint1);
				m_Renderer->drawLine(c, cc->m_FixedPoint2);
			}
			if( pConstraint->m_Type == ECT_LINE)
			{
				LineConstraint* lc = (LineConstraint*)pConstraint;
				Vector2f anchorPoint;// = hc->m_Anchor;
				Vector2f p0, p1;
				if( lc->m_PhysicalBody1 )
				{
					Vector2f c = lc->m_PhysicalBody1->m_Centre;
					lc->m_PhysicalBody1->getTransform().transformAsPoint(c);
					p0 = c;
					anchorPoint = c + lc->m_r1;
				}
				if( lc->m_PhysicalBody2 )
				{
					Vector2f c = lc->m_PhysicalBody2->m_Centre;
					lc->m_PhysicalBody2->getTransform().transformAsPoint(c);
					p1 = c;
					anchorPoint = c + lc->m_r2;
				}
				if( lc->m_PhysicalBody1 )
					m_Renderer->drawLine(p0, anchorPoint);
				if( lc->m_PhysicalBody2 )
					m_Renderer->drawLine(p1, anchorPoint);
				m_Renderer->setColor(255, 0, 255);
				m_Renderer->setPointSize(3.0f);
				m_Renderer->drawPoint(anchorPoint);
			}
			if( pConstraint->m_Type == ECT_WHEEL )
			{
				WheelConstraint* lc = (WheelConstraint*)pConstraint;
				Vector2f anchorPoint;// = hc->m_Anchor;
				Vector2f p0, p1;
				if( lc->m_PhysicalBody1 )
				{
					Vector2f c = lc->m_PhysicalBody1->m_Centre;
					lc->m_PhysicalBody1->getTransform().transformAsPoint(c);
					p0 = c;
					anchorPoint = c + lc->m_r1;
				}
				if( lc->m_PhysicalBody2 )
				{
					Vector2f c = lc->m_PhysicalBody2->m_Centre;
					lc->m_PhysicalBody2->getTransform().transformAsPoint(c);
					p1 = c;
					anchorPoint = c + lc->m_r2;
				}
				if( lc->m_PhysicalBody1 )
					m_Renderer->drawLine(p0, anchorPoint);
				if( lc->m_PhysicalBody2 )
					m_Renderer->drawLine(p1, anchorPoint);
				m_Renderer->setColor(255, 0, 255);
				m_Renderer->setPointSize(3.0f);
				m_Renderer->drawPoint(anchorPoint);
			}
			if( pConstraint->m_Type == ECT_PRISMATIC )
			{
				PrismaticConstraint* lc = (PrismaticConstraint*)pConstraint;
				Vector2f anchorPoint;// = hc->m_Anchor;
				Vector2f p0, p1;
				if( lc->m_PhysicalBody1 )
				{
					Vector2f c = lc->m_PhysicalBody1->m_Centre;
					lc->m_PhysicalBody1->getTransform().transformAsPoint(c);
					p0 = c;
					anchorPoint = c + lc->m_r1;
				}
				if( lc->m_PhysicalBody2 )
				{
					Vector2f c = lc->m_PhysicalBody2->m_Centre;
					lc->m_PhysicalBody2->getTransform().transformAsPoint(c);
					p1 = c;
					anchorPoint = c + lc->m_r2;
				}
				if( lc->m_PhysicalBody1 )
					m_Renderer->drawLine(p0, anchorPoint);
				if( lc->m_PhysicalBody2 )
					m_Renderer->drawLine(p1, anchorPoint);
				m_Renderer->setColor(255, 0, 255);
				m_Renderer->setPointSize(3.0f);
				m_Renderer->drawPoint(anchorPoint);
			}
			if( pConstraint->m_Type == ECT_PULLEY )
			{
				PulleyConstraint* pc = (PulleyConstraint*)pConstraint;
				m_Renderer->setColor(255, 0, 255);
				m_Renderer->setPointSize(3.0f);
				m_Renderer->drawPoint(pc->m_FixedPoint1);
				m_Renderer->drawPoint(pc->m_FixedPoint2);
				m_Renderer->setPointSize(1.0f);
				
				Vector2f c1 = pc->m_PhysicalBody1->m_Position;
				Vector2f ap1 = pc->m_Anchor1;
				pc->m_PhysicalBody1->getTransform().transformAsVector(ap1);
				c1 += ap1;
				
				Vector2f c2 = pc->m_PhysicalBody2->m_Position;
				Vector2f ap2 = pc->m_Anchor2;
				pc->m_PhysicalBody2->getTransform().transformAsVector(ap2);
				c2 += ap2;
				
				m_Renderer->setColor(255, 255, 255);
				m_Renderer->drawLine(c1, pc->m_FixedPoint1);
				m_Renderer->drawLine(c2, pc->m_FixedPoint2);
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
			pNode->m_CollisionShape->m_pBroadPhaseNode = pNode;
			pNode->m_AABB = pNode->m_CollisionShape->getAABB();
			m_pBroadPhaseAlgorithm->addBroadPhaseNode(pNode);
		}
	}
	else 
	{
		pNode = new(m_BroadPhaseNodePool->Allocate()) BroadPhaseNode();
		pNode->m_PhysicalShape = pShape;
		pNode->m_CollisionShape = pShape->m_CollisionShape;
		pNode->m_CollisionShape->m_pBroadPhaseNode = pNode;
		pNode->m_AABB = pShape->m_CollisionShape->getAABB();
		m_pBroadPhaseAlgorithm->addBroadPhaseNode(pNode);
	}
}

void PhysicalWorld::removeFromBroadPhase( PhysicalShape* pShape)
{
	if( pShape->m_CollisionShape->getShapeType() == EST_EDGE_CHAIN )
	{
		EdgeChain* edgeChain = (EdgeChain*)pShape->m_CollisionShape;
		dint32 numEdges = edgeChain->getNumEdges();
		for (dint32 e=0; e<numEdges; e++) 
		{
			CollisionShape* cShape = (CollisionShape*)edgeChain->getEdge(e);
			m_pBroadPhaseAlgorithm->removeBroadPhaseNode(cShape->m_pBroadPhaseNode);
			m_BroadPhaseNodePool->Free(cShape->m_pBroadPhaseNode);
		}
	}
	else 
	{
		m_pBroadPhaseAlgorithm->removeBroadPhaseNode(pShape->m_CollisionShape->m_pBroadPhaseNode);
		m_BroadPhaseNodePool->Free(pShape->m_CollisionShape->m_pBroadPhaseNode);
	}
}

PhysicalBody* PhysicalWorld::pickBodyFromScreenCoordinates(Vector2f p)
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

void PhysicalWorld::WorldOverlapAABBCallBackClass::overlapCallBack(dint32 overlapNodeID, void* userData)
{
}

dfloat PhysicalWorld::CRayIntersectionCallBackClass::rayIntersectionCallBack(const RaySegment2f& raySeg, dint32 nodeID, void* userData)
{
	if( m_pWorldRayIntersectionCallBackClass )
	{
		RayIntersectionInfo info;
		BroadPhaseNode* pNode = (BroadPhaseNode*)userData;
		CollisionShape* cShape = pNode->m_CollisionShape;
		if( cShape->intersectRaySegment(pNode->m_PhysicalShape->m_ParentBody->m_Transform, raySeg, info) )
			return m_pWorldRayIntersectionCallBackClass->rayIntersectionCallBack(raySeg ,pNode->m_PhysicalShape, info);
	}
	else 
	{
		return 0.0f;
	}

	return -1.0f;
}

dfloat PhysicalWorld::CWorldRayIntersectionCallBackClass::rayIntersectionCallBack(const RaySegment2f& raySeg, PhysicalShape* pShape, RayIntersectionInfo& info)
{
	return 0.0f;
}

void PhysicalWorld::overlapAABB( AABB2f& queryAABB, OverlapCallBackClass* pOverlapCallBackClass )
{
	if( pOverlapCallBackClass )
		m_pBroadPhaseAlgorithm->overlapAABB(queryAABB, pOverlapCallBackClass);
	//m_OverlapCallBackClass.overlapCallBack(
}

void PhysicalWorld::intersectRaySegment( const RaySegment2f& raySeg )
{
	m_pBroadPhaseAlgorithm->intersectRaySegment(raySeg, &m_RayIntersectionCallBackClass);
}

void PhysicalWorld::intersectRaySegment( const RaySegment2f& raySeg, RayIntersectionCallBackClass* callBack )
{
	m_pBroadPhaseAlgorithm->intersectRaySegment(raySeg, callBack);
}

void PhysicalWorld::setCollisionListener( ICollisionLisetner* pCollisionListener )
{
    m_pCollisionListener = pCollisionListener; 
    m_CollisionManager->m_pCollisionListener = pCollisionListener;
}

//void PhysicalWorld::setRayIntersectionListener(WorldRayIntersectionCallBackClass* pRayIntersectionCallback)
//{
//	m_RayIntersectionCallBackClass.m_pWorldRayIntersectionCallBackClass = pRayIntersectionCallback;
//}

void PhysicalWorld::intersectRaySegment( const RaySegment2f& raySeg, WorldRayIntersectionCallBackClass* callBack )
{
	m_RayIntersectionCallBackClass.m_pWorldRayIntersectionCallBackClass = callBack;
	m_pBroadPhaseAlgorithm->intersectRaySegment(raySeg, &m_RayIntersectionCallBackClass);
}

END_NAMESPACE_DIRAC2D
