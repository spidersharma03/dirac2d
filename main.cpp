#include <iostream>

#ifndef WIN32 
#include <sys/time.h>
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif

#include "Dirac2D/Dirac2D.h"

USE_NAMESPACE_DIRAC2D

void initScene();
void changeSize(int w, int h);
void renderScene(void);

PhysicalWorld* pWorld;

dfloat dt = 1.0f/600.0f;

dint32 windowWidth   = 800;
dint32 windowHeight = 600;

void demo1()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.8f));
	//pBodyGround->setAngle(M_PI_4/5);
	pBodyGround->m_BodyType = EBT_STATIC;
	
	PhysicalAppearance pApp;
	dfloat groundWidth = 1.0f; dfloat groundHeight = 0.02f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	// Create Box
	PhysicalBody* pBodyBox = pWorld->createPhysicalBody();
	//pBodyBox->m_BodyType = EBT_STATIC;
	pBodyBox->setPosition(Vector2f(0.0f,0.1f));
	pBodyBox->setAngle(-PI_2);
	dfloat boxWidth = 0.051f; dfloat boxHeight = 0.051f;
	Vector2f verticesBox[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox, 4);
	pBodyBox->createPhysicalShape(pApp);
}

void demo2()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.8f));
	//pBodyGround->setAngle(M_PI_4/5);
	pBodyGround->m_BodyType = EBT_STATIC;
	
	PhysicalAppearance pApp;
	dfloat groundWidth = 2.0f; dfloat groundHeight = 0.02f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	// Create Boxes
	dfloat y = -0.8f;
	for( int i=0; i<10; i++ )
	{
		PhysicalBody* pBodyBox = pWorld->createPhysicalBody();
		//pBodyBox->m_BodyType = EBT_STATIC;
		y += 0.2f;
		pBodyBox->setPosition(Vector2f(0.0f,y));
		//pBodyBox->setAngle(M_PI_4*0.9);
		dfloat boxWidth = 0.07f; dfloat boxHeight = 0.07f;
		Vector2f verticesBox[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
		pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox, 4);
		pBodyBox->createPhysicalShape(pApp);
	}
}

void demo3()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.8f));
	//pBodyGround->setAngle(M_PI_4/5);
	pBodyGround->m_BodyType = EBT_STATIC;
	
	PhysicalAppearance pApp;
	dfloat groundWidth = 2.0f; dfloat groundHeight = 0.02f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	// Create Boxes
	dfloat y = -0.8f;
	dfloat x = -1.0f;
	dint32 n = 20;
	dfloat dx = 0.0f;
	for(int j=0; j<20;j++ )
	{
		y += 0.2f;
		x = -2.0 + dx;
		dx += 0.1f;
		for( int i=0; i<n; i++ )
		{
			PhysicalBody* pBodyBox = pWorld->createPhysicalBody();
			//pBodyBox->m_BodyType = EBT_STATIC;
			x += 0.2f;
			pBodyBox->setPosition(Vector2f(x,y));
			//pBodyBox->setAngle(PI_4*0.9);
			dfloat boxWidth = 0.07f; dfloat boxHeight = 0.071f;
			Vector2f verticesBox[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
			pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox,4);
			pBodyBox->createPhysicalShape(pApp);
		}
		n--;
	}
}

void demo4()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.8f));
	//pBodyGround->setAngle(-M_PI_4/15);
	pBodyGround->m_BodyType = EBT_STATIC;
	
	PhysicalAppearance pApp;
	dfloat groundWidth = 1.0f; dfloat groundHeight = 0.02f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	// Create Circles
	dfloat y = -0.3f;
	for( int i=0; i<10; i++ )
	{
		PhysicalBody* pBodyCircle = pWorld->createPhysicalBody();
		//pBodyCircle->m_BodyType = EBT_STATIC;
		pBodyCircle->setPosition(Vector2f(0.0f,y));
		dfloat radius = 0.051f;
		pApp.m_CollisionAttributes.m_Shape = new Circle(radius);
		pBodyCircle->createPhysicalShape(pApp);
		y += 0.3f;
	}
	
	// Create Boxes
	y = -0.3f;
	for( int i=0; i<10; i++ )
	{
		PhysicalBody* pBodyBox = pWorld->createPhysicalBody();
		//pBodyBox->m_BodyType = EBT_STATIC;
		y += 0.2f;
		pBodyBox->setPosition(Vector2f(0.1f,y));
		//pBodyBox->setAngle(-PI_2*1.0);
		dfloat boxWidth = 0.05f; dfloat boxHeight = 0.05f;
		Vector2f verticesBox[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
		pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox, 4);
		pBodyBox->createPhysicalShape(pApp);
	}
}

void demo5()
{
	{
		// Create Ground Body
		PhysicalBody* pBodyGround1 = pWorld->createPhysicalBody();
		pBodyGround1->setPosition(Vector2f(1.5f,1.0f));
		pBodyGround1->setAngle(PI_4/5);
		pBodyGround1->m_BodyType = EBT_STATIC;
		
		PhysicalAppearance pApp;
		dfloat groundWidth = 0.4f; dfloat groundHeight = 0.02f;
		Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
		pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(vertices, 4);
		pBodyGround1->createPhysicalShape(pApp);
		
		PhysicalBody* pBodyGround2 = pWorld->createPhysicalBody();
		pBodyGround2->setPosition(Vector2f(0.0f,-0.0f));
		pBodyGround2->m_BodyType = EBT_STATIC;
		groundWidth = 1.50f;
		Vector2f vertices2[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
		pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(vertices2, 4);
		pBodyGround2->createPhysicalShape(pApp);
		
		PhysicalBody* pBodyGround3 = pWorld->createPhysicalBody();
		pBodyGround3->setPosition(Vector2f(0.0f,-1.0f));
		pBodyGround3->m_BodyType = EBT_STATIC;
		groundWidth = 2.50f;
		Vector2f vertices3[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
		pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(vertices3, 4);
		pBodyGround3->createPhysicalShape(pApp);
		
		PhysicalBody* pBodyGround4 = pWorld->createPhysicalBody();
		pBodyGround4->setPosition(Vector2f(-1.6f,-0.3f));
		pBodyGround4->setAngle(-PI_4);
		pBodyGround4->m_BodyType = EBT_STATIC;
		groundWidth = 0.20f;
		Vector2f vertices4[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
		pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(vertices4, 4);
		pBodyGround4->createPhysicalShape(pApp);
		
		// Create Boxes
		dfloat y = 0.35f;
		dfloat x = -1.3f;
		dint32 n = 10;
		for(int j=0; j<n;j++ )
		{
			x += 0.2f;
			for( int i=0; i<1; i++ )
			{
				PhysicalBody* pBodyBox = pWorld->createPhysicalBody();
				//pBodyBox->m_BodyType = EBT_STATIC;
				pBodyBox->setPosition(Vector2f(x,y));
				//pBodyBox->setAngle(PI_4*0.9);
				dfloat boxWidth = 0.02f; dfloat boxHeight = 0.2f;
				Vector2f verticesBox[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
				pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox, 4);
				pBodyBox->createPhysicalShape(pApp);
			}
		}
		
		// Create Boxes
		y = -0.75f;
		x = -1.3f;
		n = 10;
		for(int j=0; j<n;j++ )
		{
			x += 0.3f;
			for( int i=0; i<1; i++ )
			{
				PhysicalBody* pBodyBox = pWorld->createPhysicalBody();
				//pBodyBox->m_BodyType = EBT_STATIC;
				pBodyBox->setPosition(Vector2f(x,y));
				//pBodyBox->setAngle(PI_4*0.9);
				dfloat boxWidth = 0.02f; dfloat boxHeight = 0.2f;
				Vector2f verticesBox[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
				pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox, 4);
				pBodyBox->createPhysicalShape(pApp);
			}
		}
		
		// Create Circles
		y = 1.2f;
		for( int i=0; i<1; i++ )
		{
			PhysicalBody* pBodyCircle = pWorld->createPhysicalBody();
			//pBodyCircle->m_BodyType = EBT_STATIC;
			pBodyCircle->setPosition(Vector2f(1.5f,y));
			dfloat radius = 0.1f;
			pApp.m_CollisionAttributes.m_Shape = new Circle(radius);
			pBodyCircle->createPhysicalShape(pApp);
			y += 0.3f;
		}
		
		PhysicalBody* pBodyCircle2 = pWorld->createPhysicalBody();
		//pBodyCircle->m_BodyType = EBT_STATIC;
		pBodyCircle2->setPosition(Vector2f(-1.5f,y));
		dfloat radius = 0.1f;
		pApp.m_CollisionAttributes.m_Shape = new Circle(radius);
		pBodyCircle2->createPhysicalShape(pApp);
	}
}

void demo6()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.8f));
	//pBodyGround->setAngle(M_PI_4/5);
	pBodyGround->m_BodyType = EBT_STATIC;
	
	PhysicalAppearance pApp;
	dfloat groundWidth = 2.0f; dfloat groundHeight = 0.02f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	// Create Boxes
	dfloat y = -0.85f;
	dfloat x = -1.0f;
	dint32 n = 20;
	dfloat dx = 0.0f;
	for(int j=0; j<20;j++ )
	{
		y += 0.2f;
		x = -2.0 + dx;
		dx += 0.1f;
		for( int i=0; i<n; i++ )
		{
			PhysicalBody* pBodyCapsule = pWorld->createPhysicalBody();
			//pBodyCapsule->m_BodyType = EBT_STATIC;
			x += 0.2f;
			pBodyCapsule->setPosition(Vector2f(x,y));
			//pBodyCapsule->setAngle(PI_4*0.9);
			dfloat boxWidth = 0.07f; dfloat boxHeight = 0.05f;
			boxWidth += ( (float)rand()/RAND_MAX - 0.5f )*0.0f;
			//if( i%2==0)
			pApp.m_CollisionAttributes.m_Shape = new Capsule(boxWidth,boxHeight);
			pBodyCapsule->createPhysicalShape(pApp);
		}
		n--;
	}
}

void demo7()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.8f));
	//pBodyGround->setAngle(M_PI_4/5);
	pBodyGround->m_BodyType = EBT_STATIC;
	
	PhysicalAppearance pApp;
	dfloat groundWidth = 2.0f; dfloat groundHeight = 0.02f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	// Create Capsule
	dfloat y = 0.5f;
	dfloat x = -0.0f;
	PhysicalBody* pBodyCapsule = pWorld->createPhysicalBody();
	//pBodyCapsule->m_BodyType = EBT_STATIC;
	pBodyCapsule->setPosition(Vector2f(x+0.1,y));
	//pBodyCapsule->setAngle(PI_4*0.9);
	dfloat capsuleRadius = 0.15f; dfloat capsuleHeight = 0.14079f;
	pApp.m_CollisionAttributes.m_Shape = new Capsule(capsuleRadius,capsuleHeight);
	pBodyCapsule->createPhysicalShape(pApp);
	
	// Create Circle
	PhysicalBody* pBodyCircle = pWorld->createPhysicalBody();
	//pBodyCapsule->m_BodyType = EBT_STATIC;
	pBodyCircle->setPosition(Vector2f(0.0f,0.0f));
	dfloat radius = 0.15f;
	pApp.m_CollisionAttributes.m_Shape = new Circle(radius/2);
	pBodyCircle->createPhysicalShape(pApp);
	
	// Create Box
	PhysicalBody* pBodyBox = pWorld->createPhysicalBody();
	//pBodyBox->m_BodyType = EBT_STATIC;
	pBodyBox->setPosition(Vector2f(0.0f,1.0f));
	dfloat boxWidth = 0.15f; dfloat boxHeight = 0.15f;
	Vector2f verticesBox[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox, 4);	
	pBodyBox->createPhysicalShape(pApp);
	
}

void demo8()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.8f));
	//pBodyGround->setAngle(M_PI_4/5);
	pBodyGround->m_BodyType = EBT_STATIC;
	
	PhysicalAppearance pApp;
	dfloat groundWidth = 1.5f; dfloat groundHeight = 0.02f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	// Create Capsule
	dfloat y = 0.5f;
	PhysicalBody* pBodyCompound = pWorld->createPhysicalBody();
	//pBodyCapsule->m_BodyType = EBT_STATIC;
	pBodyCompound->setPosition(Vector2f(0.0,y));
	//pBodyCapsule->setAngle(PI_4*0.9);
	dfloat capsuleRadius = 0.15f; dfloat capsuleHeight = 0.1f;
	pApp.m_CollisionAttributes.m_Shape = new Capsule(capsuleRadius,capsuleHeight);
	//pApp.m_PhysicalAttributes.m_Position = Vector2f(0.40f, 0.0f);
	//pApp.m_PhysicalAttributes.m_Angle = PI/2;
	pBodyCompound->createPhysicalShape(pApp);
	
	// Create Circle
	dfloat radius = 0.1f;
	pApp.m_CollisionAttributes.m_Shape = new Circle(radius);
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.0f, 0.9f);
	pBodyCompound->createPhysicalShape(pApp);
	
}

// Compound Shapes
void demo9()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.8f));
	//pBodyGround->setAngle(M_PI_4/5);
	pBodyGround->m_BodyType = EBT_STATIC;
	
	PhysicalAppearance pApp;
	dfloat groundWidth = 1.5f; dfloat groundHeight = 0.02f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	dfloat y = 0.0f;
	// Create D
	PhysicalBody* pBodyBoxD = pWorld->createPhysicalBody();
	//pBodyBox->m_BodyType = EBT_STATIC;
	pBodyBoxD->setPosition(Vector2f(-0.8f,y));
	//pBodyBox->setAngle(PI_4);
	dfloat boxWidth = 0.025f; dfloat boxHeight = 0.25f;
	Vector2f verticesBox[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.0f,0.0f);
	//pApp.m_PhysicalAttributes.m_Angle = PI/4;
	pBodyBoxD->createPhysicalShape(pApp);
	
	boxWidth = 0.025f; boxHeight = 0.1f;
	Vector2f verticesBox1[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox1, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.1f,0.2f);
	pApp.m_PhysicalAttributes.m_Angle = PI/2*0.9;
	pBodyBoxD->createPhysicalShape(pApp);
	
	boxWidth = 0.025f; boxHeight = 0.1f;
	Vector2f verticesBox2[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new Circle(0.05f);	
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox2, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.1f,-0.2f);
	pApp.m_PhysicalAttributes.m_Angle = -PI/2*0.9;
	pBodyBoxD->createPhysicalShape(pApp);
	
	boxWidth = 0.025f; boxHeight = 0.2f;
	Vector2f verticesBox3[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox3, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.2f,0.0f);
	pApp.m_PhysicalAttributes.m_Angle = 0.0f;
	pBodyBoxD->createPhysicalShape(pApp);
	
	
	// Create I
	PhysicalBody* pBodyBoxI = pWorld->createPhysicalBody();
	//pBodyBox->m_BodyType = EBT_STATIC;
	pBodyBoxI->setPosition(Vector2f(-0.3f,y));
	//pBodyBox->setAngle(PI_4);
	boxWidth = 0.025f; boxHeight = 0.25f;
	Vector2f verticesBox4[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox4, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.0f,0.0f);
	//pApp.m_PhysicalAttributes.m_Angle = PI/4;
	pBodyBoxI->createPhysicalShape(pApp);
	
	boxWidth = 0.025f; boxHeight = 0.1f;
	Vector2f verticesBox5[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox5, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.0f,0.25f);
	pApp.m_PhysicalAttributes.m_Angle = PI/2;
	pBodyBoxI->createPhysicalShape(pApp);
	
	boxWidth = 0.025f; boxHeight = 0.1f;
	Vector2f verticesBox6[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox6, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.0f,-0.25f);
	pApp.m_PhysicalAttributes.m_Angle = PI/2;
	pBodyBoxI->createPhysicalShape(pApp);
	
	
	// Create R
	PhysicalBody* pBodyBoxR = pWorld->createPhysicalBody();
	//pBodyBox->m_BodyType = EBT_STATIC;
	pBodyBoxR->setPosition(Vector2f(0.0f,y));
	//pBodyBox->setAngle(PI_4);
	boxWidth = 0.025f; boxHeight = 0.25f;
	Vector2f verticesBox7[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox7, 4);
	//pApp.m_CollisionAttributes.m_Shape = new Capsule(boxWidth, boxHeight);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.0f,0.0f);
	pApp.m_PhysicalAttributes.m_Angle = 0.0f;
	pBodyBoxR->createPhysicalShape(pApp);
	
	boxWidth = 0.03f; boxHeight = 0.1f;
	Vector2f verticesBox8[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox8, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.1f,0.25f);
	pApp.m_PhysicalAttributes.m_Angle = PI/2;
	pBodyBoxR->createPhysicalShape(pApp);
	
	boxWidth = 0.025f; boxHeight = 0.1f;
	Vector2f verticesBox9[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox9, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.2f,0.15f);
	pApp.m_PhysicalAttributes.m_Angle = 0.0f;
	pBodyBoxR->createPhysicalShape(pApp);
	
	boxWidth = 0.03f; boxHeight = 0.1f;
	Vector2f verticesBox10[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox10, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.1f,0.05f);
	pApp.m_PhysicalAttributes.m_Angle = PI/2;
	pBodyBoxR->createPhysicalShape(pApp);
	
	boxWidth = 0.03f; boxHeight = 0.17f;
	Vector2f verticesBox11[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox11, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.15f,-0.1f);
	pApp.m_PhysicalAttributes.m_Angle = PI/4;
	pBodyBoxR->createPhysicalShape(pApp);
	
	
	// Create A
	PhysicalBody* pBodyBoxA = pWorld->createPhysicalBody();
	//pBodyBox->m_BodyType = EBT_STATIC;
	pBodyBoxA->setPosition(Vector2f(0.4f,y));
	//pBodyBox->setAngle(PI_4);
	boxWidth = 0.025f;  boxHeight = 0.25f;
	Vector2f verticesBox12[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox12, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.0f,0.0f);
	pApp.m_PhysicalAttributes.m_Angle = 0.0f;
	pBodyBoxA->createPhysicalShape(pApp);
	
	boxWidth = 0.025f; boxHeight = 0.25f;
	Vector2f verticesBox13[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox13, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.2f,0.01f);
	pApp.m_PhysicalAttributes.m_Angle = PI/4;
	pBodyBoxA->createPhysicalShape(pApp);
	
	boxWidth = 0.025f; boxHeight = 0.1f;
	Vector2f verticesBox14[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox14, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.05f,0.0f);
	pApp.m_PhysicalAttributes.m_Angle = PI/2*1.5;
	pBodyBoxA->createPhysicalShape(pApp);
	
	// Create C
	PhysicalBody* pBodyBoxC = pWorld->createPhysicalBody();
	//pBodyBox->m_BodyType = EBT_STATIC;
	pBodyBoxC->setPosition(Vector2f(0.9f,y));
	//pBodyBox->setAngle(PI_4);
	boxWidth = 0.025f;  boxHeight = 0.25f;
	Vector2f verticesBox16[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox16, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.0f,0.0f);
	pApp.m_PhysicalAttributes.m_Angle = 0.0f;
	pBodyBoxC->createPhysicalShape(pApp);
	
	boxWidth = 0.03f; boxHeight = 0.1f;
	Vector2f verticesBox17[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox17, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.1f,0.22f);
	pApp.m_PhysicalAttributes.m_Angle = PI/2;
	pBodyBoxC->createPhysicalShape(pApp);
	
	boxWidth = 0.03f; boxHeight = 0.1f;
	Vector2f verticesBox18[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox18, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.1f,-0.22f);
	pApp.m_PhysicalAttributes.m_Angle = PI/2;
	pBodyBoxC->createPhysicalShape(pApp);
	
}

// Edge Chain - Circle
void demo10()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.8f));
	pBodyGround->setAngle(PI_4/115);
	pBodyGround->m_BodyType = EBT_STATIC;
	
	PhysicalAppearance pApp;
	dint32 edgeCount = 30;
	Vector2f vertices[edgeCount];
	dfloat chainLength = 3.0f;
	dfloat ex = -chainLength/2;
	dfloat ey = 0.0f;
	dfloat dx = chainLength/(edgeCount-1);
	for( dint32 e=0; e<edgeCount; e++ )
	{
		vertices[e] = Vector2f(ex,ey);
		ex += dx;
		ey = 0.05*sin(e);
	}
	
	pApp.m_CollisionAttributes.m_Shape = new EdgeChain(vertices, edgeCount);
	pBodyGround->createPhysicalShape(pApp);
	
	// Create Circle
	dfloat y = 0.3f;
	PhysicalBody* pBodyCompound = pWorld->createPhysicalBody();
	//pBodyCompound->m_BodyType = EBT_STATIC;
	pBodyCompound->setPosition(Vector2f(1.2,y));
	//pBodyCapsule->setAngle(PI_4*0.9);
	dfloat capsuleRadius = 0.15f;
	pApp.m_CollisionAttributes.m_Shape = new Circle(capsuleRadius);
	//pApp.m_PhysicalAttributes.m_Position = Vector2f(0.40f, 0.0f);
	//pApp.m_PhysicalAttributes.m_Angle = PI/2;
	pBodyCompound->createPhysicalShape(pApp);
}

// Edge Chain - Capsule
void demo11()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.8f));
	pBodyGround->setAngle(-PI_4/12);
	pBodyGround->m_BodyType = EBT_STATIC;
	
	PhysicalAppearance pApp;
	dint32 edgeCount = 30;
	Vector2f vertices[edgeCount];
	dfloat chainLength = 3.0f;
	dfloat ex = chainLength/2;
	dfloat ey = 0.0f;
	dfloat dx = chainLength/(edgeCount-1);
	
	vertices[0] = Vector2f(-ex,0.0f);
	vertices[1] = Vector2f(0.0f,0.8f);
	vertices[2] = Vector2f(ex,0.0f);

	for( dint32 e=0; e<edgeCount; e++ )
	{
		vertices[e] = Vector2f(ex,ey);
		ex -= dx;
		ey = -0.02*sin(e*1.39);
	}
	
	pApp.m_CollisionAttributes.m_Shape = new EdgeChain(vertices, edgeCount);
	pBodyGround->createPhysicalShape(pApp);
	
	// Create Capsule
	dfloat y = 0.4f;
	PhysicalBody* pBodyCompound = pWorld->createPhysicalBody();
	//pBodyCompound->m_BodyType = EBT_STATIC;
	pBodyCompound->setPosition(Vector2f(-1.2,y));
	//pBodyCompound->setAngle(PI_4*0.9);
	dfloat capsuleRadius = 0.15f;
	pApp.m_CollisionAttributes.m_Shape = new Capsule(capsuleRadius*1.0,1.0f*capsuleRadius);
	//pApp.m_PhysicalAttributes.m_Position = Vector2f(0.40f, 0.0f);
	//pApp.m_PhysicalAttributes.m_Angle = PI/2;
	pBodyCompound->createPhysicalShape(pApp);
}

// Capsule Polygon
void demo12()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.5f));
	pBodyGround->setAngle(PI_4);
	pBodyGround->m_BodyType = EBT_STATIC;
	
	PhysicalAppearance pApp;
	dfloat groundWidth = 0.4f; dfloat groundHeight = 0.4f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	// Create Capsule
	dfloat y = 0.4f;
	PhysicalBody* pBodyCompound = pWorld->createPhysicalBody();
	//pBodyCompound->m_BodyType = EBT_STATIC;
	pBodyCompound->setPosition(Vector2f(0.01,y));
	//pBodyCompound->setAngle(PI_4*0.9);
	dfloat capsuleRadius = 0.15f;
	pApp.m_CollisionAttributes.m_Shape = new Capsule(capsuleRadius,4.2f*capsuleRadius);
	//pApp.m_PhysicalAttributes.m_Position = Vector2f(0.40f, 0.0f);
	//pApp.m_PhysicalAttributes.m_Angle = PI/2;
	pBodyCompound->createPhysicalShape(pApp);
}


void initScene()
{
	pWorld = new PhysicalWorld();
	GLRenderer* glRenderer = new GLRenderer(pWorld);
	pWorld->setRenderer(glRenderer);
	
	demo11();
}

void changeSize(int w, int h) 
{
	windowWidth = w;
	windowHeight = h;
    // Prevent a divide by zero, when window is too short
    dfloat ratio;
    if(h == 0)
        h = 1;
	
    ratio = 1.0* w / h;
	
    // Reset the coordinate system before modifying
	
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
    // Set the viewport to be the entire window
	
    glViewport(0, 0, w, h);
	
    // Set the correct perspective.
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(-1.5*ratio, 1.5*ratio, -1.5, 1.5, -0.01, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluLookAt(0.0,0.0,5.0, 
//			  0.0,0.0,-1.0,
//			  0.0f,1.0f,0.0f);
	
	
}

void renderScene(void) 
{		
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	if(windowHeight == 0)
        windowHeight = 1;
	
    dfloat ratio = 1.0* windowWidth / windowHeight;
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();	
	glOrtho(-1.5*ratio, 1.5*ratio, -1.5, 1.5, -0.01, 100);
	pWorld->Step(dt);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	pWorld->draw();
	
    glutSwapBuffers();
}

static bool bWarmStart = true;

void keyProcessor(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27:
			exit(0);
			break;
			
		case 'w':
		case 'W':
			bWarmStart = !bWarmStart;
			pWorld->setWarmStart(bWarmStart);
			break;
	}
}
	
#ifndef WIN32
static void timerCallback (int value)
{
    timeval t1, t2;
    static double elapsedTime;
    double FPS = 0;
    
    static int frameCnt = 0;
    
    // start timer
    gettimeofday(&t1, NULL);
    
    // do something
    renderScene();
    frameCnt ++;
    
    // stop timer
    gettimeofday(&t2, NULL);
    
    // compute and print the elapsed time in millisec
    elapsedTime += (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    
    if( frameCnt >=500 )
    {
        frameCnt=0;
        FPS = 500 /(elapsedTime * 0.001);
        printf("\n FPS = %f", FPS);
        elapsedTime = 0;
    }
    
    /* Call back again after elapsedUSecs have passed */
    glutTimerFunc (0, timerCallback, 0);
}

#endif

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,10);
    glutInitWindowSize(windowWidth,windowHeight);
    glutCreateWindow("Physics Engine - DIRAC2D");
	glutKeyboardFunc(keyProcessor);
	glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);

	
#ifndef WIN32
	glutTimerFunc(0, timerCallback, 0);
#endif
	
	initScene();
	
    glutMainLoop();
	
	return 0;
}