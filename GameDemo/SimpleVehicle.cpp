//
//  Vehicle.cpp
//  Dirac2D
//
//  Created by Prashant Sharma on 27/10/12.
//

#include "SimpleVehicle.h"
#include "SimpleGun.h"
#include "FirstGame.h"

SimpleVehicle::SimpleVehicle(FirstGame* pGame)
{
    m_pGame = pGame;
    	
	m_pGun = new SimpleGun(m_pGame);
	
    init();
}

void SimpleVehicle::applyForce( const Vector2f& force )
{
	m_pVehicleBody->applyForce(force);
}

void SimpleVehicle::applyImpulse( const Vector2f& impulse)
{
	m_pVehicleBody->applyImpulse(impulse, Vector2f(0.0f,0.0f));
}

void SimpleVehicle::applyTorqueImpulse( const float torqueImpulse)
{
	m_pVehicleBody->applyImpulsiveTorque(torqueImpulse);
}

void SimpleVehicle::init()
{
    PhysicalWorld* pWorld = m_pGame->getPhysicalWorld();
        
	for( dint32 i=0; i< 1; i++ )
	{
		m_pVehicleBody = pWorld->createPhysicalBody();
		
		float xPos = 15.0f;
		float yPos = 0.0f;
		m_pVehicleBody->setPosition(Vector2f(1.0f+xPos,1.05f+yPos));		
		
		PhysicalAppearance pApp;
        pApp.m_CollisionAttributes.m_Filter.m_CollisionBit = EOCB_PHYSICAL_BODY;
        pApp.m_CollisionAttributes.m_Filter.m_CollisionMask = EOCB_PARTICLE_DEBRIS;

		pApp.m_MassAttributes.m_Density = 10.0f;
		pApp.m_CollisionAttributes.m_Shape = new Capsule(0.07f,0.6);
		//pApp.m_CollisionAttributes.m_Shape = new Circle(0.3f);
		PhysicalShape* pShape = m_pVehicleBody->createPhysicalShape(pApp);
        //pShape->m_CollisionFilter.m_CollisionBit = EOCB_PHYSICAL_BODY;
        //pShape->m_CollisionFilter.m_CollisionMask = EOCB_PARTICLE_DEBRIS;
        
		m_pVehicleBody->m_AngularDamping = 50.0f;
		//m_pVehicleBody->m_LinearDamping = 2.0f;
		
		PhysicalBody* circle1 = pWorld->createPhysicalBody();
		PhysicalBody* circle2 = pWorld->createPhysicalBody();
        circle1->setPosition(Vector2f(1.3f+xPos,0.8f+yPos));		
		circle2->setPosition(Vector2f(1.0f-0.3f+xPos,0.8f+yPos));		
		
		pApp.m_MassAttributes.m_Density = 200.0f;
		pApp.m_CollisionAttributes.m_Shape = new Circle(0.1f);
		
		pShape = circle1->createPhysicalShape(pApp);
		//pShape->m_CollisionFilter.m_CollisionBit = EOCB_PHYSICAL_BODY;
        //pShape->m_CollisionFilter.m_CollisionMask = EOCB_PARTICLE_DEBRIS;
        
		pApp.m_CollisionAttributes.m_Shape = new Circle(0.1f);
        pApp.m_PhysicalAttributes.m_Friction = 1.0f;
		
        pShape = circle2->createPhysicalShape(pApp);
		//pShape->m_CollisionFilter.m_CollisionBit = EOCB_PHYSICAL_BODY;
        //pShape->m_CollisionFilter.m_CollisionMask = EOCB_PARTICLE_DEBRIS;
        
		WheelConstraint* lc1 = (WheelConstraint*)pWorld->createConstraint(ECT_WHEEL);
		lc1->m_PhysicalBody1 = m_pVehicleBody;
		lc1->m_PhysicalBody2 = circle1;
		lc1->m_Anchor = Vector2f(1.3f+xPos,0.8f);
		lc1->m_LocalAxis = Vector2f(1.0f,-1.0f);
		lc1->initialize();
		lc1->m_Erp = 10.0f;
		lc1->m_Cfm = 8.0f;
		
		WheelConstraint* lc2 = (WheelConstraint*)pWorld->createConstraint(ECT_WHEEL);
		lc2->m_PhysicalBody1 = m_pVehicleBody;
		lc2->m_PhysicalBody2 = circle2;
		lc2->m_Anchor = Vector2f(1.0f-0.3f+xPos,0.8f);
		lc2->m_LocalAxis = Vector2f(-1.0f,-1.0f);
		lc2->initialize();
		lc2->m_Erp = 10.0f;
		lc2->m_Cfm = 8.0f;
		
        m_pMotor = (MotorConstraint*)pWorld->createConstraint(ECT_MOTOR);
        m_pMotor->m_PhysicalBody1 = circle2;
		m_pMotor->m_MaxTorque = 100.0f;
		m_pMotor->m_Speed = 50.0f;
		m_pMotor->initialize();
	}

}

void SimpleVehicle::initFromFile(const char* fileName)
{
    
}

void SimpleVehicle::update(float dt)
{
	m_Position = m_pVehicleBody->m_Position;
	m_Transform = m_pVehicleBody->getTransform();
	
	m_pGun->update(dt);
}

void SimpleVehicle::render()
{
	m_pGun->render();
}

void SimpleVehicle::shoot()
{
	m_pGun->shoot();
}
