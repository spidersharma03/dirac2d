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
void SimpleVehicle::init()
{
    PhysicalWorld* pWorld = m_pGame->getPhysicalWorld();
    // Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
    //pBodyGround->setAngle(PI_4/10);
	pBodyGround->setPosition(Vector2f(1.0f,-0.7f));
	pBodyGround->m_BodyType = EBT_STATIC;
	//dfloat groundWidth = 1.2f;  
    //dfloat groundHeight = 0.02f;
	//Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	PhysicalAppearance pApp;
    pApp.m_PhysicalAttributes.m_Friction = 1.0f;
	
	dint32 edgeCount = 30;
	Vector2f vertices[30];
	dfloat chainLength = 3.0f;
	dfloat ex = 15*chainLength/2;
	dfloat ey = 0.0f;
	dfloat dx = 10*chainLength/(edgeCount-1);
	
	vertices[0] = Vector2f(-ex,0.0f);
	vertices[1] = Vector2f(0.0f,0.8f);
	vertices[2] = Vector2f(ex,0.0f);
	
	for( dint32 e=0; e<edgeCount; e++ )
	{
		vertices[e] = Vector2f(ex,ey);
		ex -= dx;
		ey = -0.0*sin(e*1.39);
	}
	
	pApp.m_CollisionAttributes.m_Shape = new EdgeChain(vertices, edgeCount);
	pBodyGround->createPhysicalShape(pApp);
    
	for( dint32 i=0; i< 1; i++ )
	{
		m_pVehicleBody = pWorld->createPhysicalBody();
		PhysicalBody* circle1 = pWorld->createPhysicalBody();
		PhysicalBody* circle2 = pWorld->createPhysicalBody();
        
		//pBox1->setAngle(PI_4);
		//pBox1->m_BodyType = EBT_STATIC;
		
		m_pVehicleBody->setPosition(Vector2f(0.0f,0.0f));		
		circle1->setPosition(Vector2f(0.3f,-0.2f));		
		circle2->setPosition(Vector2f(-0.3f,-0.2f));		
		
		PhysicalAppearance pApp;
		//dfloat boxWidth = 0.3f; 
        //dfloat boxHeight = 0.04f;
		//Vector2f vertices[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
		//pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
		pApp.m_CollisionAttributes.m_Shape = new Capsule(0.03f, 0.6f);
		m_pVehicleBody->createPhysicalShape(pApp);
		pApp.m_CollisionAttributes.m_Shape = new Circle(0.1f);
		
		circle1->createPhysicalShape(pApp);
		
		pApp.m_CollisionAttributes.m_Shape = new Circle(0.1f);
        
		circle2->createPhysicalShape(pApp);
		
		WheelConstraint* lc1 = (WheelConstraint*)pWorld->createConstraint(ECT_WHEEL);
		lc1->m_PhysicalBody1 = m_pVehicleBody;
		lc1->m_PhysicalBody2 = circle1;
		lc1->m_Anchor = Vector2f(0.3f,-0.2f);
		lc1->m_LocalAxis = Vector2f(1.0f,-1.0f);
		lc1->initialize();
		lc1->m_Erp = 50.0f;
		lc1->m_Cfm = 4.0f;
		
		WheelConstraint* lc2 = (WheelConstraint*)pWorld->createConstraint(ECT_WHEEL);
		lc2->m_PhysicalBody1 = m_pVehicleBody;
		lc2->m_PhysicalBody2 = circle2;
		lc2->m_Anchor = Vector2f(-0.3f,-0.2f);
		lc2->m_LocalAxis = Vector2f(-1.0f,-1.0f);
		lc2->initialize();
		lc2->m_Erp = 50.0f;
		lc2->m_Cfm = 4.0f;
		
        m_pMotor = (MotorConstraint*)pWorld->createConstraint(ECT_MOTOR);
        m_pMotor->m_PhysicalBody1 = circle2;
		m_pMotor->m_MaxTorque = 10.0f;
		m_pMotor->m_Speed = 5.0f;
		m_pMotor->initialize();
	}

}

void SimpleVehicle::initFromFile(const char* fileName)
{
    
}

void SimpleVehicle::update(float dt)
{
    m_Position = m_pVehicleBody->m_Position;
}

void SimpleVehicle::render()
{
}
