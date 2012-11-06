//
//  Vehicle.cpp
//  Dirac2D
//
//  Created by Prashant Sharma on 27/10/12.
//

#include "SimpleVehicle.h"
#include "FirstGame.h"

SimpleVehicle::SimpleVehicle(FirstGame* pGame)
{
    m_pGame = pGame;
    
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

void SimpleVehicle::init()
{
    PhysicalWorld* pWorld = m_pGame->getPhysicalWorld();
    // Create Ground Body
	//PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
//	pBodyGround->setPosition(Vector2f(1.0f,-0.7f));
//	pBodyGround->m_BodyType = EBT_STATIC;
//	PhysicalAppearance pApp;
//    pApp.m_PhysicalAttributes.m_Friction = 1.0f;
//	
//	dint32 edgeCount = 30;
//	Vector2f vertices[30];
//	dfloat chainLength = 3.0f;
//	dfloat ex = 15*chainLength/2;
//	dfloat ey = 0.0f;
//	dfloat dx = 10*chainLength/(edgeCount-1);
//	
//	vertices[0] = Vector2f(-ex,0.0f);
//	vertices[1] = Vector2f(0.0f,0.8f);
//	vertices[2] = Vector2f(ex,0.0f);
//	
//	for( dint32 e=0; e<edgeCount; e++ )
//	{
//		vertices[e] = Vector2f(ex,ey);
//		ex -= dx;
//		ey = -0.0*sin(e*1.39);
//	}
//	
//	pApp.m_CollisionAttributes.m_Shape = new EdgeChain(vertices, edgeCount);
//	pBodyGround->createPhysicalShape(pApp);
    
	for( dint32 i=0; i< 1; i++ )
	{
		m_pVehicleBody = pWorld->createPhysicalBody();
		
		float xPos = 15.0f;
		float yPos = 1.0f;
		m_pVehicleBody->setPosition(Vector2f(1.0f+xPos,1.1f+yPos));		
		
		PhysicalAppearance pApp;
		pApp.m_MassAttributes.m_Density = 50.0f;
		//pApp.m_CollisionAttributes.m_Shape = new Capsule(0.1f,0.6);
		pApp.m_CollisionAttributes.m_Shape = new Circle(0.3f);
		m_pVehicleBody->createPhysicalShape(pApp);
		
		
//		PhysicalBody* circle1 = pWorld->createPhysicalBody();
//		PhysicalBody* circle2 = pWorld->createPhysicalBody();
//        circle1->setPosition(Vector2f(1.3f+xPos,0.8f));		
//		circle2->setPosition(Vector2f(1.0f-0.3f+xPos,0.8f));		
//		
//		pApp.m_MassAttributes.m_Density = 500.0f;
//		pApp.m_CollisionAttributes.m_Shape = new Circle(0.15f);
//		
//		circle1->createPhysicalShape(pApp);
//		
//		pApp.m_CollisionAttributes.m_Shape = new Circle(0.15f);
//        pApp.m_PhysicalAttributes.m_Friction = 1.0f;
//		circle2->createPhysicalShape(pApp);
//		
//		WheelConstraint* lc1 = (WheelConstraint*)pWorld->createConstraint(ECT_WHEEL);
//		lc1->m_PhysicalBody1 = m_pVehicleBody;
//		lc1->m_PhysicalBody2 = circle1;
//		lc1->m_Anchor = Vector2f(1.3f+xPos,0.8f);
//		lc1->m_LocalAxis = Vector2f(1.0f,-1.0f);
//		lc1->initialize();
//		lc1->m_Erp = 50.0f;
//		lc1->m_Cfm = 0.0f;
//		
//		WheelConstraint* lc2 = (WheelConstraint*)pWorld->createConstraint(ECT_WHEEL);
//		lc2->m_PhysicalBody1 = m_pVehicleBody;
//		lc2->m_PhysicalBody2 = circle2;
//		lc2->m_Anchor = Vector2f(1.0f-0.3f+xPos,0.8f);
//		lc2->m_LocalAxis = Vector2f(-1.0f,-1.0f);
//		lc2->initialize();
//		lc2->m_Erp = 50.0f;
//		lc2->m_Cfm = 0.0f;
		
        //m_pMotor = (MotorConstraint*)pWorld->createConstraint(ECT_MOTOR);
        //m_pMotor->m_PhysicalBody1 = circle2;
//		m_pMotor->m_MaxTorque = 100.0f;
//		m_pMotor->m_Speed = 0.0f;
//		m_pMotor->initialize();
	}

}

void SimpleVehicle::initFromFile(const char* fileName)
{
    
}

void SimpleVehicle::update(float dt)
{
    m_Position = m_pVehicleBody->m_Position;
	m_pVehicleBody->applyImpulse(Vector2f(0.05f,0.0f), Vector2f(-0.0f,0.0f));
}

void SimpleVehicle::render()
{
}
