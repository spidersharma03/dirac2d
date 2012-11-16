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
GLRenderer* glRenderer;
DTimer timer;

dfloat dt = 1.0f/2000.0f;

dint32 windowWidth   = 800;
dint32 windowHeight = 600;

DistanceConstraint *mouseJoint;

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
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	// Create Box
	PhysicalBody* pBodyBox = pWorld->createPhysicalBody();
	//pBodyBox->m_BodyType = EBT_STATIC;
	pBodyBox->setPosition(Vector2f(0.0f,0.1f));
	pBodyBox->setAngle(-PI_2);
	dfloat boxWidth = 0.051f; dfloat boxHeight = 0.051f;
	Vector2f verticesBox[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox, 4);
	pBodyBox->createPhysicalShape(pApp);
}

void demo2()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.8f));
	//pBodyGround->setAngle(-PI);
	pBodyGround->m_BodyType = EBT_STATIC;
	
	PhysicalAppearance pApp;
	dfloat groundWidth = 1.5f; dfloat groundHeight = 0.02f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	// Create Boxes
	dfloat y = -0.5f;
	for( int i=0; i<10; i++ )
	{
		PhysicalBody* pBodyBox = pWorld->createPhysicalBody();
		//pBodyBox->m_BodyType = EBT_STATIC;
		y += 0.2f;
		pBodyBox->setPosition(Vector2f(0.0f,y));
		dfloat boxWidth = 0.07f; dfloat boxHeight = 0.07f;
		Vector2f verticesBox[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
		for( dint32 i=0; i<4; i++ )
		{
			//verticesBox[i] += Vector2f(0.5f,0.0f);
		}
		pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox, 4);
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
	
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	// Create Boxes
	dfloat y = -0.8f;
	dfloat x = -1.0f;
	dint32 n = 40;
	dfloat dx = 0.0f;
	for(int j=0; j<40;j++ )
	{
		y += 0.1f;
		x = -2.0 + dx;
		dx += 0.05f;
		for( int i=0; i<n; i++ )
		{
			PhysicalBody* pBodyBox = pWorld->createPhysicalBody();
			//pBodyBox->m_BodyType = EBT_STATIC;
			x += 0.1f;
			pBodyBox->setPosition(Vector2f(x,y));
			//pBodyBox->setAngle(PI_4*0.9);
			dfloat boxWidth = 0.035f; dfloat boxHeight = 0.035f;
			Vector2f verticesBox[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
			
			pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox,4);
			pBodyBox->createPhysicalShape(pApp);
		}
		n--;
	}
}

class MyCallBack : public RayIntersectionCallBackClass
{
public:
	void rayIntersectionCallBack(dint32 overlapNodeID, void* userData)
	{
		BroadPhaseNode* pNode = (BroadPhaseNode*)userData;
		m_pBody = pNode->m_PhysicalShape->m_ParentBody;
		numCalls++;
	}

	int numCalls;
	PhysicalBody* m_pBody;
};

MyCallBack callBack;

void demo3_()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.8f));
	//pBodyGround->setAngle(M_PI_4/5);
	pBodyGround->m_BodyType = EBT_STATIC;
	
	PhysicalAppearance pApp;
	dfloat groundWidth = 3.0f; dfloat groundHeight = 0.02f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	// Create Boxes
	dfloat y = -0.8f;
	dfloat x = -1.0f;
	dint32 n = 3;
	dfloat dx = 0.0f;
												
	for(int j=0; j<3;j++ )
	{
		y += 0.1f;
		x = -1.0 + dx;
		dx += 0.1f;
		for( int i=0; i<n; i++ )
		{
			PhysicalBody* pBodyBox = pWorld->createPhysicalBody();
			//pBodyBox->m_BodyType = EBT_STATIC;
			x += 0.1f;
			pBodyBox->setPosition(Vector2f(x,y));
			//pBodyBox->setAngle(PI_4*0.9);
			dfloat boxWidth = 0.037f; dfloat boxHeight = 0.037f;
			Vector2f verticesBox[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
			
			pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox,4);
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
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
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
		pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox, 4);
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
		pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
		pBodyGround1->createPhysicalShape(pApp);
		
		PhysicalBody* pBodyGround2 = pWorld->createPhysicalBody();
		pBodyGround2->setPosition(Vector2f(0.0f,-0.0f));
		pBodyGround2->m_BodyType = EBT_STATIC;
		groundWidth = 1.50f;
		Vector2f vertices2[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
		pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices2, 4);
		pBodyGround2->createPhysicalShape(pApp);
		
		PhysicalBody* pBodyGround3 = pWorld->createPhysicalBody();
		pBodyGround3->setPosition(Vector2f(0.0f,-1.0f));
		pBodyGround3->m_BodyType = EBT_STATIC;
		groundWidth = 2.50f;
		Vector2f vertices3[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
		pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices3, 4);
		pBodyGround3->createPhysicalShape(pApp);
		
		PhysicalBody* pBodyGround4 = pWorld->createPhysicalBody();
		pBodyGround4->setPosition(Vector2f(-1.6f,-0.3f));
		pBodyGround4->setAngle(-PI_4);
		pBodyGround4->m_BodyType = EBT_STATIC;
		groundWidth = 0.20f;
		Vector2f vertices4[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
		pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices4, 4);
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
				pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox, 4);
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
				pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox, 4);
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
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
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
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
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
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox, 4);	
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
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
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
	pBodyGround->setPosition(Vector2f(0.0f,-1.4f));
	//pBodyGround->setAngle(M_PI_4/5);
	pBodyGround->m_BodyType = EBT_STATIC;
	
	PhysicalAppearance pApp;
	dfloat groundWidth = 2.5f; dfloat groundHeight = 0.02f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	dfloat y = 0.0f;
	// Create D
	PhysicalBody* pBodyBoxD = pWorld->createPhysicalBody();
	//pBodyBox->m_BodyType = EBT_STATIC;
	pBodyBoxD->setPosition(Vector2f(-0.8f,y));
	//pBodyBox->setAngle(PI_4);
	dfloat boxWidth = 0.025f; dfloat boxHeight = 0.25f;
	Vector2f verticesBox[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.0f,0.0f);
	//pApp.m_PhysicalAttributes.m_Angle = PI/4;
	pBodyBoxD->createPhysicalShape(pApp);
	
	boxWidth = 0.025f; boxHeight = 0.1f;
	Vector2f verticesBox1[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox1, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.1f,0.2f);
	pApp.m_PhysicalAttributes.m_Angle = PI/2*0.9;
	pBodyBoxD->createPhysicalShape(pApp);
	
	boxWidth = 0.025f; boxHeight = 0.1f;
	Vector2f verticesBox2[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new Circle(0.05f);	
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox2, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.1f,-0.2f);
	pApp.m_PhysicalAttributes.m_Angle = -PI/2*0.9;
	pBodyBoxD->createPhysicalShape(pApp);
	
	boxWidth = 0.025f; boxHeight = 0.2f;
	Vector2f verticesBox3[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox3, 4);	
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
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox4, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.0f,0.0f);
	//pApp.m_PhysicalAttributes.m_Angle = PI/4;
	pBodyBoxI->createPhysicalShape(pApp);
	
	boxWidth = 0.025f; boxHeight = 0.1f;
	Vector2f verticesBox5[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox5, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.0f,0.25f);
	pApp.m_PhysicalAttributes.m_Angle = PI/2;
	pBodyBoxI->createPhysicalShape(pApp);
	
	boxWidth = 0.025f; boxHeight = 0.1f;
	Vector2f verticesBox6[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox6, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.0f,-0.25f);
	pApp.m_PhysicalAttributes.m_Angle = PI/2;
	pBodyBoxI->createPhysicalShape(pApp);
	
	
	// Create R
	PhysicalBody* pBodyBoxR = pWorld->createPhysicalBody();
	//pBodyBoxR->m_BodyType = EBT_STATIC;
	pBodyBoxR->setPosition(Vector2f(0.0f,y));
	//pBodyBox->setAngle(PI_4);
	boxWidth = 0.025f; boxHeight = 0.25f;
	Vector2f verticesBox7[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	for( dint32 i=0; i<4; i++ )
	{
		verticesBox7[i] += Vector2f(0, 0);
	}
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox7, 4);
	//pApp.m_CollisionAttributes.m_Shape = new Capsule(boxWidth, boxHeight);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.0f,0.0f);
	pApp.m_PhysicalAttributes.m_Angle = 0.0f;
	pBodyBoxR->createPhysicalShape(pApp);
	
	boxWidth = 0.03f; boxHeight = 0.1f;
	Vector2f verticesBox8[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	for( dint32 i=0; i<4; i++ )
	{
		verticesBox8[i] += Vector2f(0, 0);
	}
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox8, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.1f,0.25f);
	pApp.m_PhysicalAttributes.m_Angle = PI/2;
	pBodyBoxR->createPhysicalShape(pApp);
	
	boxWidth = 0.025f; boxHeight = 0.1f;
	Vector2f verticesBox9[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	for( dint32 i=0; i<4; i++ )
	{
		verticesBox9[i] += Vector2f(0, 0);
	}
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox9, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.2f,0.15f);
	pApp.m_PhysicalAttributes.m_Angle = 0.0f;
	pBodyBoxR->createPhysicalShape(pApp);
	
	boxWidth = 0.03f; boxHeight = 0.1f;
	Vector2f verticesBox10[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	for( dint32 i=0; i<4; i++ )
	{
		verticesBox10[i] += Vector2f(0, 0);
	}
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox10, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.1f,0.05f);
	pApp.m_PhysicalAttributes.m_Angle = PI/2;
	pBodyBoxR->createPhysicalShape(pApp);
	
	boxWidth = 0.03f; boxHeight = 0.17f;
	Vector2f verticesBox11[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	for( dint32 i=0; i<4; i++ )
	{
		verticesBox11[i] += Vector2f(0, 0);
	}
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox11, 4);	
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
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox12, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.0f,0.0f);
	pApp.m_PhysicalAttributes.m_Angle = 0.0f;
	pBodyBoxA->createPhysicalShape(pApp);
	
	boxWidth = 0.025f; boxHeight = 0.25f;
	Vector2f verticesBox13[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox13, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.2f,0.01f);
	pApp.m_PhysicalAttributes.m_Angle = PI/4;
	pBodyBoxA->createPhysicalShape(pApp);
	
	boxWidth = 0.025f; boxHeight = 0.1f;
	Vector2f verticesBox14[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox14, 4);	
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
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox16, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.0f,0.0f);
	pApp.m_PhysicalAttributes.m_Angle = 0.0f;
	pBodyBoxC->createPhysicalShape(pApp);
	
	boxWidth = 0.03f; boxHeight = 0.1f;
	Vector2f verticesBox17[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox17, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.1f,0.22f);
	pApp.m_PhysicalAttributes.m_Angle = PI/2;
	pBodyBoxC->createPhysicalShape(pApp);
	
	boxWidth = 0.03f; boxHeight = 0.1f;
	Vector2f verticesBox18[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox18, 4);	
	pApp.m_PhysicalAttributes.m_Position = Vector2f(0.1f,-0.22f);
	pApp.m_PhysicalAttributes.m_Angle = PI/2;
	pBodyBoxC->createPhysicalShape(pApp);
	
	DistanceConstraint* dc1 = (DistanceConstraint*)pWorld->createConstraint(ECT_DISTANCE);
	dc1->m_PhysicalBody1 = pBodyBoxD;
	dc1->m_PhysicalBody2 = pBodyBoxI;
	dc1->m_Anchor1 = Vector2f(0.0f,0.0f);
	dc1->m_Anchor2 = Vector2f(0.0f,0.0f);
	dc1->initialize();
	
	DistanceConstraint* dc2 = (DistanceConstraint*)pWorld->createConstraint(ECT_DISTANCE);
	dc2->m_PhysicalBody1 = pBodyBoxI;
	dc2->m_PhysicalBody2 = pBodyBoxR;
	dc2->m_Anchor1 = Vector2f(0.0f,0.0f);
	dc2->m_Anchor2 = Vector2f(0.0f,0.0f);
	dc2->initialize();
	
	DistanceConstraint* dc3 = (DistanceConstraint*)pWorld->createConstraint(ECT_DISTANCE);
	dc3->m_PhysicalBody1 = pBodyBoxR;
	dc3->m_PhysicalBody2 = pBodyBoxA;
	dc3->m_Anchor1 = Vector2f(0.0f,0.0f);
	dc3->m_Anchor2 = Vector2f(0.0f,0.0f);
	dc3->initialize();
	
	DistanceConstraint* dc4 = (DistanceConstraint*)pWorld->createConstraint(ECT_DISTANCE);
	dc4->m_PhysicalBody1 = pBodyBoxA;
	dc4->m_PhysicalBody2 = pBodyBoxC;
	dc4->m_Anchor1 = Vector2f(0.0f,0.0f);
	dc4->m_Anchor2 = Vector2f(0.0f,0.0f);
	dc4->initialize();
	
	//PhysicalBody* pBodyBoxD2 = pBodyBoxD->clone();
	//pBodyBoxD2->setPosition(Vector2f(1.2f,0.0f));
	//pBodyBoxD2->addToPhysicalWorld(pWorld);
	
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
	Vector2f vertices[30];
	dfloat chainLength = 3.0f;
	dfloat ex = -chainLength/2;
	dfloat ey = 0.0f;
	dfloat dx = chainLength/(edgeCount-1);
	for( dint32 e=0; e<edgeCount; e++ )
	{
		vertices[e] = Vector2f(ex,ey);
		ex += dx;
		ey = 0.05*sinf(e);
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
	Vector2f vertices[30];
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
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
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

// Capsule Capsule
void demo13()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.5f));
	pBodyGround->setAngle(PI_4);
	pBodyGround->m_BodyType = EBT_STATIC;
	
	PhysicalAppearance pApp;
	//Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	pApp.m_CollisionAttributes.m_Shape = new Capsule(0.1, 0.2);
	pBodyGround->createPhysicalShape(pApp);
	
	// Create Capsule
	dfloat y = 0.4f;
	PhysicalBody* pBodyCompound = pWorld->createPhysicalBody();
	//pBodyCompound->m_BodyType = EBT_STATIC;
	pBodyCompound->setPosition(Vector2f(0.01,y));
	//pBodyCompound->setAngle(PI_4*0.9);
	dfloat capsuleRadius = 0.15f;
	pApp.m_CollisionAttributes.m_Shape = new Capsule(capsuleRadius*0.5,14*capsuleRadius);
	//pApp.m_PhysicalAttributes.m_Position = Vector2f(0.40f, 0.0f);
	//pApp.m_PhysicalAttributes.m_Angle = PI/2;
	pBodyCompound->createPhysicalShape(pApp);
}

// EdgeChain Polygon
void demo14()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.5f));
	//pBodyGround->setAngle(PI_4/112);
	pBodyGround->m_BodyType = EBT_STATIC;
	
	PhysicalAppearance pApp;
	dfloat groundWidth = 2.0f; dfloat groundHeight = 0.0f;
	dint32 vertexCount = 3;
	Vector2f vertices[3];
	dfloat dx = groundWidth/(vertexCount-1);
	dfloat ex = -groundWidth/2,ey = 0.0f;
	
	for( dint32 v=0; v<vertexCount-1; v++ )
	{
		vertices[v].x = ex;
		vertices[v].y = ey;
		ex += dx;
		vertices[v+1].x = ex;
		vertices[v+1].y = ey;
	}
	
	pApp.m_CollisionAttributes.m_Shape = new EdgeChain(vertices, vertexCount);
	pBodyGround->createPhysicalShape(pApp);
	
	// Create Capsule
	dfloat y = 0.4f;
	PhysicalBody* pBodyCompound = pWorld->createPhysicalBody();
	//pBodyCompound->m_BodyType = EBT_STATIC;
	pBodyCompound->setPosition(Vector2f(0.3,y));
	//pBodyCompound->setAngle(-PI_4*0.999);
	groundWidth = 0.2f;  groundHeight = 0.2f;
	Vector2f vertices1[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices1,4);
	//pApp.m_PhysicalAttributes.m_Position = Vector2f(0.40f, 0.0f);
	//pApp.m_PhysicalAttributes.m_Angle = PI/2;
	pBodyCompound->createPhysicalShape(pApp);
}


// EdgeChain Polygon
void demo15()
{
    PhysicalBody* pBodyCircle = pWorld->createPhysicalBody();
	pBodyCircle->setPosition(Vector2f(0.0f,1.5f));
	PhysicalAppearance pAppCircle;
    pAppCircle.m_CollisionAttributes.m_Shape = new Circle(0.23f);
	pBodyCircle->createPhysicalShape(pAppCircle);
    
	dfloat y = 0.85f;
	dfloat bodyX = -1.0f;
	dfloat bodyDx = 0.45f;
	// CREATE D
	PhysicalBody* pBodyD = pWorld->createPhysicalBody();
	pBodyD->setPosition(Vector2f(bodyX,y));
	PhysicalAppearance pApp;
	dfloat groundWidth = 2.0f; dfloat groundHeight = 0.0f;
	dint32 vertexCount = 15;
	Vector2f verticesD[15];
	
	dfloat Height = 0.4f;
	
	verticesD[0].x = 0.0f;
	verticesD[0].y = Height/2;
	verticesD[1].x = 0.0f;
	verticesD[1].y = -Height/2;
	
	dfloat angle = 0.0f;
	dfloat dAngle = PI/11;
	dfloat ex = 0.0f,ey = -Height/2;
	for( dint32 v=2; v<vertexCount-1; v++ )
	{
		ex = Height/2 * sin(angle);
		ey = Height/2 * cos(angle);
		verticesD[v].x = ex;
		verticesD[v].y = ey;
		angle += dAngle;
	}
	pApp.m_CollisionAttributes.m_Shape = new EdgeChain(verticesD, vertexCount);
	pBodyD->createPhysicalShape(pApp);
	
//	DistanceConstraint* dcD = (DistanceConstraint*)pWorld->createConstraint(ECT_DISTANCE);
//	dcD->m_PhysicalBody1 = pBodyD;
//	dcD->m_Anchor2 = Vector2f(bodyX,y+0.5);
//	dcD->initialize();
	
	// CREATE I
	dfloat WIDTH = 0.25f;
	bodyX += bodyDx;
	PhysicalBody* pBodyI = pWorld->createPhysicalBody();
	pBodyI->setPosition(Vector2f(bodyX,y));
	vertexCount = 13;
	Vector2f verticesI[13];
		
	verticesI[0].x = -WIDTH/4; verticesI[0].y = Height/2 - Height/4;
	verticesI[1].x = -WIDTH/4; verticesI[1].y = -Height/2 + Height/4;
	verticesI[2].x = -WIDTH/2; verticesI[2].y = -Height/2 + Height/4;
	verticesI[3].x = -WIDTH/2; verticesI[3].y = -Height/2;
	verticesI[4].x =  WIDTH/2; verticesI[4].y = -Height/2;
	verticesI[5].x =  WIDTH/2; verticesI[5].y = -Height/2 + Height/4;
	verticesI[6].x =  WIDTH/4; verticesI[6].y = -Height/2 + Height/4;
	verticesI[7].x =  WIDTH/4; verticesI[7].y = Height/2 - Height/4;
	verticesI[8].x =  WIDTH/2; verticesI[8].y = Height/2 - Height/4;
	verticesI[9].x =  WIDTH/2; verticesI[9].y = Height/2;
	verticesI[10].x = -WIDTH/2; verticesI[10].y = Height/2;
	verticesI[11].x = -WIDTH/2; verticesI[11].y = Height/2 - Height/4;
	verticesI[12].x = -WIDTH/4; verticesI[12].y = Height/2 - Height/4;

	
	pApp.m_CollisionAttributes.m_Shape = new EdgeChain(verticesI, vertexCount);
	pBodyI->createPhysicalShape(pApp);
	
//	DistanceConstraint* dcI = (DistanceConstraint*)pWorld->createConstraint(ECT_DISTANCE);
//	dcI->m_PhysicalBody1 = pBodyI;
//	dcI->m_Anchor2 = Vector2f(bodyX-0.1,y+0.5);
//	dcI->initialize();
	
	// CREATE R
	PhysicalBody* pBodyR = pWorld->createPhysicalBody();
	//pBodyR->m_BodyType = EBT_STATIC;
	bodyX += bodyDx;
	pBodyR->setPosition(Vector2f(bodyX,y));
	vertexCount = 14;
	Vector2f verticesR[14];
	
	verticesR[0].x = 0.0f; verticesR[0].y = Height/4;

	angle = 0.0f;
	dAngle = PI/8;
	//angle += dAngle;
	for( dint32 v=1; v<8; v++ )
	{
		ex = 2*Height/4 * sin(angle);
		ey = Height/4 * cos(angle);
		verticesR[v].x = ex;
		verticesR[v].y = ey;
		angle += dAngle;
	}
	
	verticesR[8].x = ex + WIDTH/2; verticesR[8].y = -Height/2-Height/4;
	verticesR[9].x = ex + WIDTH/2 - WIDTH/4; verticesR[9].y = -Height/2-Height/4;
	verticesR[10].x = ex + WIDTH/2 - WIDTH/2 - WIDTH/4; verticesR[10].y = -Height/4;
	verticesR[11].x = ex + WIDTH/2 - WIDTH/2 - WIDTH/4; verticesR[11].y = -Height/2-Height/4;
	verticesR[12].x = ex + WIDTH/2 - WIDTH/2 - WIDTH/2; verticesR[12].y = -Height/2-Height/4;
	verticesR[13].x = 0.0f; verticesR[13].y = Height/4;

	pApp.m_CollisionAttributes.m_Shape = new EdgeChain(verticesR, vertexCount);
	pBodyR->createPhysicalShape(pApp);

//	DistanceConstraint* dcR = (DistanceConstraint*)pWorld->createConstraint(ECT_DISTANCE);
//	dcR->m_PhysicalBody1 = pBodyR;
//	dcR->m_Anchor2 = Vector2f(bodyX-0.1,y+0.5);
//	dcR->initialize();
	
	// CREATE A
	PhysicalBody* pBodyA = pWorld->createPhysicalBody();
	//pBodyA->m_BodyType = EBT_STATIC;
	bodyX += bodyDx;
	pBodyA->setPosition(Vector2f(bodyX,y));
	vertexCount = 7;
	Vector2f verticesA[7];
	
	verticesA[0].x = 0.0f;                verticesA[0].y = Height/2;
	verticesA[1].x = WIDTH/1.5;           verticesA[1].y = -Height/2;
	verticesA[2].x = WIDTH/1.5 - WIDTH/4; verticesA[2].y = -Height/2;
	
	verticesA[3].x = 0.0f; verticesA[3].y = 0.0f;
	verticesA[4].x = -verticesA[2].x; verticesA[4].y = verticesA[2].y;
	verticesA[5].x = -verticesA[1].x; verticesA[5].y = verticesA[1].y;
	verticesA[6].x = 0.0f; verticesA[6].y = Height/2;
	
	pApp.m_CollisionAttributes.m_Shape = new EdgeChain(verticesA, vertexCount);
	pBodyA->createPhysicalShape(pApp);
	
//	DistanceConstraint* dcA = (DistanceConstraint*)pWorld->createConstraint(ECT_DISTANCE);
//	dcA->m_PhysicalBody1 = pBodyA;
//	dcA->m_Anchor2 = Vector2f(bodyX-0.1,y+0.5);
//	dcA->initialize();
	
	// CREATE C
	PhysicalBody* pBodyC = pWorld->createPhysicalBody();
	//pBodyC->m_BodyType = EBT_STATIC;
	bodyX += bodyDx;
	pBodyC->setPosition(Vector2f(bodyX,y));
	vertexCount = 18;
	Vector2f verticesC[18];
	
	angle = 0.0f;
	dAngle = PI/7;
	//angle += dAngle;
	for( dint32 v=0; v<8; v++ )
	{
		ex = 1.5*( Height/2 - Height/8 ) * sin(angle);
		ey = ( Height/2 - Height/8 ) * cos(angle);
		verticesC[v].x = ex;
		verticesC[v].y = ey;
		angle -= dAngle;
	}
	verticesC[8].x = ex; verticesC[8].y = ey - Height/8;
	dAngle = PI/7;
	angle += dAngle;
	for( dint32 v=9; v<17; v++ )
	{
		ex = 1.5*Height/2 * sin(angle);
		ey = Height/2 * cos(angle);
		verticesC[v].x = ex;
		verticesC[v].y = ey;
		angle += dAngle;
	}
	
	verticesC[17].x = ex; verticesC[17].y = ey - Height/8;

	pApp.m_CollisionAttributes.m_Shape = new EdgeChain(verticesC, vertexCount);
	pBodyC->createPhysicalShape(pApp);
	
	DistanceConstraint* dcC = (DistanceConstraint*)pWorld->createConstraint(ECT_DISTANCE);
	dcC->m_PhysicalBody1 = pBodyC;
	dcC->m_Anchor1 = Vector2f(0.0f,0.5f);
	dcC->m_Anchor2 = Vector2f(bodyX-0.1,y+0.5);
	dcC->initialize();
	
	// CREATE 2
	PhysicalBody* pBody2 = pWorld->createPhysicalBody();
	//pBody2->m_BodyType = EBT_STATIC;
	bodyX += bodyDx*0.3;
	pBody2->setPosition(Vector2f(bodyX,y));
	pBody2->setAngle(PI_4*0.5f);

	vertexCount = 21;
	Vector2f vertices2[21];
	angle = 0.0f;
	dAngle = PI/7;
	for( dint32 v=0; v<8; v++ )
	{
		ex = 1.2*( Height/2 - Height/8 ) * sin(angle);
		ey = ( Height/2 - Height/8 ) * cos(angle);
		vertices2[v].x = ex;
		vertices2[v].y = ey;
		angle += dAngle;
	}
	vertices2[8].x = ex; vertices2[8].y = ey - Height/4;
	vertices2[9].x = ex+WIDTH; vertices2[9].y = ey - Height/4;
	vertices2[10].x = ex+WIDTH; vertices2[10].y = ey - Height/4 + Height/8;
	vertices2[11].x = ex+WIDTH/8; vertices2[11].y = ey - Height/4 + Height/8;

	dfloat R = vertices2[11].length();
	angle = PI;
	dAngle = PI/7;
	for( dint32 v=12; v<20; v++ )
	{
		ex = 1.2*R * sin(angle);
		ey = R * cos(angle);
		vertices2[v].x = ex;
		vertices2[v].y = ey;
		angle -= dAngle;
	}
	vertices2[20].x = vertices2[19].x; vertices2[20].y = vertices2[19].y - Height/8;

	pApp.m_CollisionAttributes.m_Shape = new EdgeChain(vertices2, vertexCount);
	pBody2->createPhysicalShape(pApp);
	
//	DistanceConstraint* dc2 = (DistanceConstraint*)pWorld->createConstraint(ECT_DISTANCE);
//	dc2->m_PhysicalBody1 = pBody2;
//	dc2->m_Anchor1 = Vector2f(0.0f,0.5f);
//	dc2->m_Anchor2 = Vector2f(bodyX-0.1,y+0.5);
//	dc2->initialize();
	
	// CREATE CLONE OF D
	bodyX += bodyDx*0.9;
	PhysicalBody* pBodyD2 = pBodyD->clone();
	pBodyD2->setPosition(Vector2f(bodyX,y));
	pBodyD2->addToPhysicalWorld(pWorld);
	
//	DistanceConstraint* dcD2 = (DistanceConstraint*)pWorld->createConstraint(ECT_DISTANCE);
//	dcD2->m_PhysicalBody1 = pBodyD2;
//	dcD2->m_Anchor1 = Vector2f(0.0f,0.5f);
//	dcD2->m_Anchor2 = Vector2f(bodyX-0.1,y+0.5);
//	dcD2->initialize();
	
	// Create Capsule
	y = -0.4f;
	PhysicalBody* pBodyCompound = pWorld->createPhysicalBody();
	pBodyCompound->m_BodyType = EBT_STATIC;
	pBodyCompound->setPosition(Vector2f(0.3,y));
	//pBodyCompound->setAngle(-PI_4*0.999);
	groundWidth = 2.0f;  groundHeight = 0.02f;
	Vector2f vertices1[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices1,4);
	//pApp.m_PhysicalAttributes.m_Position = Vector2f(0.40f, 0.0f);
	//pApp.m_PhysicalAttributes.m_Angle = PI/2;
	pBodyCompound->createPhysicalShape(pApp);
	

	
	//DistanceConstraint* dc = (DistanceConstraint*)pWorld->createConstraint(ECT_DISTANCE);
//	dc->m_PhysicalBody1 = pBodyA;
//	dc->m_PhysicalBody2 = pBodyR;
//	//dc->m_Anchor1 = Vector2f(0.0f,0.0f);
//	dc->initialize();
}


// Distance Constraint
void demo16()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-1.5f));
	pBodyGround->setAngle(PI_4/112);
	pBodyGround->m_BodyType = EBT_STATIC;
	dfloat groundWidth = 1.2f;  dfloat groundHeight = 0.02f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	PhysicalAppearance pApp;
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	// Create Circle Chain
	dfloat y = 1.1f;
	dint32 n = 15;
	dfloat radius = 0.035f;
	PhysicalBody* pBodyCircle = pWorld->createPhysicalBody();
	pBodyCircle->setPosition(Vector2f(0.0,y));
	pApp.m_CollisionAttributes.m_Shape = new Circle(radius);
	pBodyCircle->createPhysicalShape(pApp);
	
	dfloat erp = 10.0f; dfloat cfm = 3.0f;
	DistanceConstraint* dc = (DistanceConstraint*)pWorld->createConstraint(ECT_DISTANCE);
	dc->m_Erp = erp;
	dc->m_Cfm = cfm;
	dc->m_PhysicalBody1 = pBodyCircle;
	dc->m_Anchor1 = Vector2f(0.0f,0.0f);
	dc->m_Anchor2 = Vector2f(0.0f,y+0.1f);
	dc->initialize();
	PhysicalBody* pPrevCircle = pBodyCircle;
	dfloat dy = radius*3;
	dfloat boxHeight = 2*radius; dfloat boxWidth = 2*radius;
	Vector2f verticesBox[4] = {Vector2f(boxWidth/2, boxHeight/2), Vector2f(-boxWidth/2, boxHeight/2), Vector2f(-boxWidth/2, -boxHeight/2), Vector2f(boxWidth/2, -boxHeight/2) };
	for( dint32 i=1; i<n; i++ )
	{
		y -= dy;
		PhysicalBody* pBodyCircle = pWorld->createPhysicalBody();
		pBodyCircle->m_LinearDamping = 0.5f;
		pBodyCircle->m_AngularDamping = 0.5f;
		pBodyCircle->setPosition(Vector2f(0.0,y));
		pApp.m_CollisionAttributes.m_Shape = new Circle(radius);//,radius*0.5f);
		pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox,  4);
		pBodyCircle->createPhysicalShape(pApp);
		
		
		DistanceConstraint* dc = (DistanceConstraint*)pWorld->createConstraint(ECT_DISTANCE);
		dc->m_Erp = erp;
		dc->m_Cfm = cfm;
		dc->m_PhysicalBody1 = pBodyCircle;
		dc->m_PhysicalBody2 = pPrevCircle;
		dc->m_Anchor1 = Vector2f(0.0f,radius/2);
		dc->m_Anchor2 = Vector2f(0.0f,-radius/2);
		dc->initialize();
		
		pPrevCircle = pBodyCircle;
	}	
}

//Regular Polygons
void demo17()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.8f));
	//pBodyGround->setAngle(M_PI_4/5);
	pBodyGround->m_BodyType = EBT_STATIC;
	
	PhysicalAppearance pApp;
	dfloat groundWidth = 2.0f; dfloat groundHeight = 0.02f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	// Create Polygons
	dfloat y = -0.5f;
	for( int i=0; i<10; i++ )
	{
		PhysicalBody* pBodyBox = pWorld->createPhysicalBody();
		//pBodyBox->m_BodyType = EBT_STATIC;
		y += 0.2f;
		pBodyBox->setPosition(Vector2f(0.0f,y));
		dfloat radius = 0.1f;
		pApp.m_CollisionAttributes.m_Shape = ConvexPolygon::createRegularPolygon(5, radius);
		pBodyBox->createPhysicalShape(pApp);
	}
}

// Dynamic Tree Test.
DynamicTree* dynamicTree = 0;

void demo18()
{	
	dint32 numBoxes = 15;
	
	dynamicTree = new DynamicTree(64);
	// Create Boxes
	for( dint32 i=0; i< numBoxes; i++ )
	{
		PhysicalBody* pBox = pWorld->createPhysicalBody();
		//Vector2f loc
		dfloat x = RANDOM_NUMBER( -1.0f, 1.0f );
		dfloat y = RANDOM_NUMBER( -1.0f, 1.0f );
		
		pBox->setPosition(Vector2f(x,y));
		pBox->m_BodyType = EBT_STATIC;
		
		PhysicalAppearance pApp;
		dfloat boxWidth = 0.06f; dfloat boxHeight = 0.06f;
		Vector2f vertices[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
		pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
		pBox->createPhysicalShape(pApp);
		
		dynamicTree->createProxy(pBox->m_AABB, 0);
	}
	dynamicTree->removeProxy(13);
	dynamicTree->removeProxy(17);
	dynamicTree->removeProxy(15);
	dynamicTree->removeProxy(19);
	dynamicTree->removeProxy(21);

}


// Hinge/Revolute Constraint
void demo19()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.8f));
	pBodyGround->m_BodyType = EBT_STATIC;
	dfloat groundWidth = 1.2f;  dfloat groundHeight = 0.02f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	PhysicalAppearance pApp;
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	for( dint32 i=0; i< 1; i++ )
	{
		PhysicalBody* pBox1 = pWorld->createPhysicalBody();
		PhysicalBody* pBox2 = pWorld->createPhysicalBody();
		//pBox2->setAngle(PI_4*2);
		//pBox2->m_BodyType = EBT_STATIC;
		
		pBox1->setPosition(Vector2f(0.0f,0.0f));		
		pBox2->setPosition(Vector2f(0.3f,0.0f));		

		PhysicalAppearance pApp;
		dfloat boxWidth = 0.06f; dfloat boxHeight = 0.2f;
		Vector2f vertices[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
		pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
		pBox1->createPhysicalShape(pApp);
		//pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
		pApp.m_CollisionAttributes.m_Shape = new Circle(0.1f);

		pBox2->createPhysicalShape(pApp);
		
		HingeConstraint* hc = (HingeConstraint*)pWorld->createConstraint(ECT_HINGE);
		//hc->m_Erp = 0.0f;
		hc->m_PhysicalBody1 = pBox1;
		hc->m_PhysicalBody2 = pBox2;
		hc->m_Anchor = Vector2f(0.0f,0.4f);
		hc->initialize();
	}
	// Create Circle Chain
}


// Weld Constraint
void demo20()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.8f));
	pBodyGround->m_BodyType = EBT_STATIC;
	dfloat groundWidth = 1.2f;  dfloat groundHeight = 0.02f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	PhysicalAppearance pApp;
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	for( dint32 i=0; i< 1; i++ )
	{
		PhysicalBody* pBox1 = pWorld->createPhysicalBody();
		PhysicalBody* pBox2 = pWorld->createPhysicalBody();
		//pBox2->setAngle(PI_4*2);
		//pBox2->m_BodyType = EBT_STATIC;
		
		pBox1->setPosition(Vector2f(-0.1f,0.0f));	
		//pBox1->m_BodyType = EBT_STATIC;
		//pBox1->setAngle(-1*PI_4);
		pBox2->setPosition(Vector2f(0.5f,0.0f));		
		//pBox2->setAngle(1*PI_4);
		
		PhysicalAppearance pApp;
		dfloat boxWidth = 0.03f; dfloat boxHeight = 0.52f;
		Vector2f vertices[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
		pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
		pBox1->createPhysicalShape(pApp);
		pApp.m_CollisionAttributes.m_Shape = new Capsule(0.1f,0.1f);
		pBox2->createPhysicalShape(pApp);
		
		WeldConstraint* wc = (WeldConstraint*)pWorld->createConstraint(ECT_WELD);
		wc->m_PhysicalBody1 = pBox1;
		wc->m_PhysicalBody2 = pBox2;
		wc->m_Anchor = Vector2f(0.1f,0.0f);
		wc->initialize();
	}
	// Create Circle Chain
}

PhysicalBody* stick = 0;
PhysicalBody* limb2 = 0;

void demo21()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.8f));
	pBodyGround->m_BodyType = EBT_STATIC;
	dfloat groundWidth = 1.5f;  dfloat groundHeight = 0.02f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	PhysicalAppearance pApp;
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	{
		// Create Balancing Stick
		stick = pWorld->createPhysicalBody();
		//pBox2->setAngle(PI_4*2);
		//pBox2->m_BodyType = EBT_STATIC;
		
		stick->setPosition(Vector2f(0.9f,0.1f));		
		stick->setAngle(PI_4/10);
		PhysicalAppearance pApp;
		dfloat boxWidth = 0.01f; dfloat boxHeight = 0.525f;
		Vector2f vertices[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
		pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
		pApp.m_PhysicalAttributes.m_Friction = 0.8f;
		stick->createPhysicalShape(pApp);
		
		limb2 = pWorld->createPhysicalBody();
		limb2->m_BodyType = EBT_KINEMATIC;
		limb2->setPosition(Vector2f(0.5f,-0.45f));
		limb2->setAngle(PI_2);
		boxWidth = 0.02f;  boxHeight = 0.5f;
		Vector2f vertices2[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
		pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices2, 4);
		limb2->createPhysicalShape(pApp);
		
		//DistanceConstraint* dc = (DistanceConstraint*)pWorld->createConstraint(ECT_DISTANCE);
//		dc->m_PhysicalBody2 = stick;
//		dc->m_PhysicalBody1 = limb2;
//		dc->m_Erp = 10.0f;
//		dc->m_Anchor2 = Vector2f(0.0f,-0.525f);
//		dc->m_Anchor1 = Vector2f(0.025f,-0.45f);
//		dc->initialize();
	}
	// Create Circle Chain
}

// Catenary Constraint
void demo22()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.8f));
	pBodyGround->m_BodyType = EBT_STATIC;
	dfloat groundWidth = 1.2f;  dfloat groundHeight = 0.02f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	PhysicalAppearance pApp;
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
	//pBodyGround->createPhysicalShape(pApp);
	
	for( dint32 i=0; i< 1; i++ )
	{
		PhysicalBody* pBox = pWorld->createPhysicalBody();
		pBox->setAngle(PI_4);
		//pBox2->m_BodyType = EBT_STATIC;
		
		pBox->setPosition(Vector2f(0.0f,-0.02f));	
		pBox->m_LinearDamping = 0.1f;
		pBox->m_AngularDamping = 0.5f;
		//pBox2->setAngle(1*PI_4);
		
		PhysicalAppearance pApp;
		
		dfloat boxWidth = 0.05f; dfloat boxHeight = 0.05f;
		Vector2f vertices[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
		pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
		pApp.m_CollisionAttributes.m_Shape = new Capsule(0.1f,0.1f);
		pBox->createPhysicalShape(pApp);
		
		CatenaryConstraintFixedRotation* cc = (CatenaryConstraintFixedRotation*)pWorld->createConstraint(ECT_CATENARY_FIXED_ROTATION);
		cc->m_PhysicalBody1 = pBox;
		cc->m_FixedPoint1 = Vector2f(-0.9f,0.1f);
		cc->m_FixedPoint2 = Vector2f(0.9f,0.0f);
		cc->m_FixedLength = 1.9030005f;
		cc->m_Anchor = Vector2f(0.0f,0.1f);
		cc->initialize();
	}
	// Create Circle Chain
}

// Wheel Constraint
void demo23()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.8f));
	pBodyGround->m_BodyType = EBT_STATIC;
	dfloat groundWidth = 1.2f;  dfloat groundHeight = 0.02f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	PhysicalAppearance pApp;
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	for( dint32 i=0; i< 1; i++ )
	{
		PhysicalBody* pBox1 = pWorld->createPhysicalBody();
		PhysicalBody* circle1 = pWorld->createPhysicalBody();
		PhysicalBody* circle2 = pWorld->createPhysicalBody();

		//pBox1->setAngle(PI_4);
		//pBox1->m_BodyType = EBT_STATIC;
		
		pBox1->setPosition(Vector2f(0.0f,0.0f));		
		circle1->setPosition(Vector2f(0.3f,-0.2f));		
		circle2->setPosition(Vector2f(-0.3f,-0.2f));		
		
		PhysicalAppearance pApp;
		dfloat boxWidth = 0.3f; dfloat boxHeight = 0.04f;
		//Vector2f vertices[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
		//pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
		pApp.m_CollisionAttributes.m_Shape = new Capsule(0.1f, 0.3f);
		pBox1->createPhysicalShape(pApp);
		pApp.m_CollisionAttributes.m_Shape = new Circle(0.1f);
		
		circle1->createPhysicalShape(pApp);
		
		pApp.m_CollisionAttributes.m_Shape = new Circle(0.1f);

		circle2->createPhysicalShape(pApp);
		
		WheelConstraint* lc1 = (WheelConstraint*)pWorld->createConstraint(ECT_WHEEL);
		lc1->m_PhysicalBody1 = pBox1;
		lc1->m_PhysicalBody2 = circle1;
		lc1->m_Anchor = Vector2f(0.3f,-0.2f);
		lc1->m_LocalAxis = Vector2f(1.0f,-1.0f);
		lc1->initialize();
		lc1->m_Erp = 50.0f;
		lc1->m_Cfm = 10.0f;
		
		WheelConstraint* lc2 = (WheelConstraint*)pWorld->createConstraint(ECT_WHEEL);
		lc2->m_PhysicalBody1 = pBox1;
		lc2->m_PhysicalBody2 = circle2;
		lc2->m_Anchor = Vector2f(-0.3f,-0.2f);
		lc2->m_LocalAxis = Vector2f(-1.0f,-1.0f);
		lc2->initialize();
		lc2->m_Erp = 50.0f;
		lc2->m_Cfm = 10.0f;
		
	}
	// Create Circle Chain
}

// Line Constraint
void demo24()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.8f));
	pBodyGround->m_BodyType = EBT_STATIC;
	dfloat groundWidth = 1.2f;  dfloat groundHeight = 0.02f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	PhysicalAppearance pApp;
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	for( dint32 i=0; i< 1; i++ )
	{
		PhysicalBody* pBox1 = pWorld->createPhysicalBody();
		PhysicalBody* circle1 = pWorld->createPhysicalBody();
		PhysicalBody* circle2 = pWorld->createPhysicalBody();
		
		//pBox1->setAngle(PI_4);
		//pBox1->m_BodyType = EBT_STATIC;
		
		pBox1->setPosition(Vector2f(0.0f,0.0f));		
		circle1->setPosition(Vector2f(0.2f,0.0f));		
		circle2->setPosition(Vector2f(-0.2f,0.0f));		
		
		PhysicalAppearance pApp;
		dfloat boxWidth = 0.06f; dfloat boxHeight = 0.06f;
		Vector2f vertices[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
		pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
		pBox1->createPhysicalShape(pApp);
		pApp.m_CollisionAttributes.m_Shape = new Circle(0.05f);
		
		circle1->createPhysicalShape(pApp);
		
		pApp.m_CollisionAttributes.m_Shape = new Circle(0.05f);

		//pApp.m_CollisionAttributes.m_Shape = new Circle(0.1f);
		
		circle2->createPhysicalShape(pApp);
		
		LineConstraint* lc1 = (LineConstraint*)pWorld->createConstraint(ECT_LINE);
		lc1->m_PhysicalBody1 = pBox1;
		lc1->m_PhysicalBody2 = circle1;
		lc1->m_Anchor = Vector2f(0.2f,0.0f);
		lc1->m_LocalAxis = Vector2f(1.0f,0.0f);
		lc1->m_LowerLimit = 0.1f;
		lc1->m_UpperLimit = 0.5f;
		lc1->m_Cfm = 100.0f;
		lc1->m_Erp = 100.0f;
		lc1->initialize();
		
		LineConstraint* lc2 = (LineConstraint*)pWorld->createConstraint(ECT_LINE);
		lc2->m_PhysicalBody1 = pBox1;
		lc2->m_PhysicalBody2 = circle2;
		lc2->m_Anchor = Vector2f(-0.2f,0.0f);
		lc2->m_LocalAxis = Vector2f(-1.0f,0.0f);
		lc2->m_LowerLimit = 0.1f;
		lc2->m_UpperLimit = 0.5f;
		lc2->m_Cfm = 100.0f;
		lc2->m_Erp = 100.0f;
		lc2->initialize();
		
		DistanceConstraint* dc = (DistanceConstraint*)pWorld->createConstraint(ECT_DISTANCE);
		dc->m_PhysicalBody1 = pBox1;
		dc->m_Erp = 100.0f;
		dc->initialize();
		
		MotorConstraint* mc = (MotorConstraint*)pWorld->createConstraint(ECT_MOTOR);
		mc->m_PhysicalBody1 = pBox1;
		mc->m_MaxTorque = 10.0f;
		mc->m_Speed = 0.50f;
		mc->initialize();
		//LineConstraint* lc2 = (LineConstraint*)pWorld->createConstraint(ECT_LINE);
//		lc2->m_PhysicalBody1 = pBox1;
//		lc2->m_PhysicalBody2 = circle2;
//		lc2->m_Anchor = Vector2f(0.2f,-0.2f);
//		lc2->m_LocalAxis = Vector2f(1.0f,0.0f);
//		lc2->initialize();
		
	}
	// Create Circle Chain
}


// Prismatic Constraint
void demo25()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.8f));
	pBodyGround->m_BodyType = EBT_STATIC;
	dfloat groundWidth = 1.2f;  dfloat groundHeight = 0.02f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	PhysicalAppearance pApp;
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	for( dint32 i=0; i< 1; i++ )
	{
		PhysicalBody* pBox1 = pWorld->createPhysicalBody();
		PhysicalBody* circle1 = pWorld->createPhysicalBody();
		//PhysicalBody* circle2 = pWorld->createPhysicalBody();
		
		//pBox1->setAngle(PI_4);
		pBox1->m_BodyType = EBT_STATIC;
		
		pBox1->setPosition(Vector2f(0.0f,0.0f));		
		circle1->setPosition(Vector2f(0.4f,0.0f));		
		//circle2->setPosition(Vector2f(0.2f,-0.2f));		
		
		PhysicalAppearance pApp;
		dfloat boxWidth = 0.06f; dfloat boxHeight = 0.06f;
		Vector2f vertices[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
		pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
		pBox1->createPhysicalShape(pApp);
		pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
		
		circle1->createPhysicalShape(pApp);
		
		//pApp.m_CollisionAttributes.m_Shape = new Circle(0.1f);
		
		//circle2->createPhysicalShape(pApp);
		
		if(1)
		{
		PrismaticConstraint* pc1 = (PrismaticConstraint*)pWorld->createConstraint(ECT_PRISMATIC);
		pc1->m_PhysicalBody1 = pBox1;
		pc1->m_PhysicalBody2 = circle1;
		pc1->m_Anchor = Vector2f(0.4f,0.20f);
		pc1->m_LocalAxis = Vector2f(1.0f,1.0f);
		pc1->m_LowerLimit = -0.1f;
		pc1->m_UpperLimit = 0.5f;		
		pc1->initialize();
		}
		else {
			LineConstraint* pc1 = (LineConstraint*)pWorld->createConstraint(ECT_LINE);
			pc1->m_PhysicalBody1 = pBox1;
			pc1->m_PhysicalBody2 = circle1;
			pc1->m_Anchor = Vector2f(0.4f,0.40f);
			pc1->m_LocalAxis = Vector2f(1.0f,1.0f);
			pc1->m_LowerLimit = -0.1f;
			pc1->m_UpperLimit = 0.5f;		
			pc1->initialize();
		}

		
		
				//LineConstraint* lc2 = (LineConstraint*)pWorld->createConstraint(ECT_LINE);
		//		lc2->m_PhysicalBody1 = pBox1;
		//		lc2->m_PhysicalBody2 = circle2;
		//		lc2->m_Anchor = Vector2f(0.2f,-0.2f);
		//		lc2->m_LocalAxis = Vector2f(1.0f,0.0f);
		//		lc2->initialize();
		
	}
	// Create Circle Chain
}


// Pulley Constraint
void demo26()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.8f));
	pBodyGround->m_BodyType = EBT_STATIC;
	dfloat groundWidth = 1.2f;  dfloat groundHeight = 0.02f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	PhysicalAppearance pApp;
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	for( dint32 i=0; i< 1; i++ )
	{
		PhysicalBody* pBox1 = pWorld->createPhysicalBody();
		PhysicalBody* circle1 = pWorld->createPhysicalBody();
		//PhysicalBody* circle2 = pWorld->createPhysicalBody();
		
		//pBox1->setAngle(PI_4);
		//pBox1->m_BodyType = EBT_STATIC;
		
		pBox1->setPosition(Vector2f(0.0f,0.0f));		
		circle1->setPosition(Vector2f(0.4f,0.0f));		
		//circle2->setPosition(Vector2f(0.2f,-0.2f));		
		
		PhysicalAppearance pApp;
		dfloat boxWidth = 0.06f; dfloat boxHeight = 0.06f;
		Vector2f vertices[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
		pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
		pBox1->createPhysicalShape(pApp);
		pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
		//pApp.m_CollisionAttributes.m_Shape = new Circle(0.1f);

		circle1->createPhysicalShape(pApp);
		
		//pApp.m_CollisionAttributes.m_Shape = new Circle(0.1f);
		
		//circle2->createPhysicalShape(pApp);
		
		if(1)
		{
			PulleyConstraint* pc1 = (PulleyConstraint*)pWorld->createConstraint(ECT_PULLEY);
			pc1->m_PhysicalBody1 = pBox1;
			pc1->m_PhysicalBody2 = circle1;
			pc1->m_PulleyRatio = 2.0f;
			pc1->m_Anchor1 = Vector2f(0.0f,0.1f);
			pc1->m_Anchor2 = Vector2f(0.0f,0.1f);
			pc1->m_FixedPoint1 = Vector2f(0.0f, 0.5f);
			pc1->m_FixedPoint2 = Vector2f(0.4f, 0.5f);
			//pc1->m_Anchor1 = Vector2f(0.4f,0.20f);
			pc1->initialize();
		}
		
		//LineConstraint* lc2 = (LineConstraint*)pWorld->createConstraint(ECT_LINE);
		//		lc2->m_PhysicalBody1 = pBox1;
		//		lc2->m_PhysicalBody2 = circle2;
		//		lc2->m_Anchor = Vector2f(0.2f,-0.2f);
		//		lc2->m_LocalAxis = Vector2f(1.0f,0.0f);
		//		lc2->initialize();
		
	}
	// Create Circle Chain
}


// MinMax Constraint
void demo27()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-1.0f));
	//pBodyGround->setAngle(PI_4/112);
	pBodyGround->m_BodyType = EBT_STATIC;
	dfloat groundWidth = 1.2f;  dfloat groundHeight = 0.02f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	PhysicalAppearance pApp;
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
	pBodyGround->createPhysicalShape(pApp);
	
	// Create Circle Chain
	dfloat y = 0.1f;
	dint32 n = 1;
	
	dfloat radius = 0.07f;
	PhysicalBody* pBodyCircle1 = pWorld->createPhysicalBody();
	pBodyCircle1->setPosition(Vector2f(0.0,y));
	pApp.m_CollisionAttributes.m_Shape = new Circle(radius);
	pBodyCircle1->createPhysicalShape(pApp);
	
	PhysicalBody* pBodyCircle2 = pWorld->createPhysicalBody();
	pBodyCircle2->setPosition(Vector2f(0.0,y-0.5f));
	pApp.m_CollisionAttributes.m_Shape = new Circle(radius);
	pBodyCircle2->createPhysicalShape(pApp);
	
	dfloat erp = 10.0f; dfloat cfm = 1.0f;
	
	MinMaxConstraint* mc = (MinMaxConstraint*)pWorld->createConstraint(ECT_MIN_MAX);

	mc->m_Erp = erp;
	mc->m_Cfm = cfm;
	mc->m_PhysicalBody1 = pBodyCircle1;
	mc->m_PhysicalBody2 = pBodyCircle2;
	mc->m_Anchor1 = Vector2f(0.0f,0.0f);
	mc->m_Anchor2 = Vector2f(0.0f,0.0f);
	mc->m_LowerLimit = 0.3f;
	mc->m_UpperLimit = 0.5f;
	mc->initialize();
	
	//PhysicalBody* pPrevCircle = pBodyCircle;
//	dfloat dy = radius*3;
//	dfloat boxHeight = 2*radius; dfloat boxWidth = 2*radius;
//	Vector2f verticesBox[4] = {Vector2f(boxWidth/2, boxHeight/2), Vector2f(-boxWidth/2, boxHeight/2), Vector2f(-boxWidth/2, -boxHeight/2), Vector2f(boxWidth/2, -boxHeight/2) };
//	
}

void demo28()
{
	int nPoints = 8;
	int nCurvePoints = 260;
	float controlPoints[8*2] = {0.0f,0.0f,  0.0f,0.4f,  0.5f,0.5f, 0.8f,0.2f, 0.5f,-0.8f, 0.2f,-0.9f, 0.4f,-0.3f,  -0.3f,-0.2f};
	float curvePoints[nCurvePoints*2];
	float knots[130], basis[30];
	int nOrder = 4;
	findOpenKnots(nPoints, nOrder, knots);
	

    knots[0] = 0.0f;    
    knots[1] = 0.0f;
    knots[2] = 0.0f;
    knots[3] = 1.0f;
    knots[4] = 1.0f;
    knots[5] = 2.0f;
    knots[6] = 3.0f;
    
	float t = 0.0f;
	glPushMatrix();
	glTranslated(-1.0, 0, 0);
    glScaled(0.5, 1.0, 1);
	for( int i=0; i<1000; i++ )
	{
		splineBasis( nPoints, nOrder, t, knots, basis);
		glBegin(GL_POINTS);
		for( int j=0; j<nPoints; j++ )
			glVertex2f(t, basis[j]);
		glEnd();
		t += 0.01f;
	}
	glPopMatrix();
	BSpline(controlPoints, nPoints, curvePoints, nCurvePoints, knots, nOrder);
	
	glPushMatrix();
	glTranslated(0, -0.5, 0);
	glPointSize(3.0f);
	for( int i=0; i<2*nPoints; i+=2 )
	{
		glBegin(GL_POINTS);
		glVertex2f(controlPoints[i], controlPoints[i+1]);
		glEnd();
	}
	glPointSize(1.0f);

	for( int i=0; i<2*nCurvePoints; i+=2 )
	{
		glBegin(GL_POINTS);
			glVertex2f(curvePoints[i], curvePoints[i+1]);
		glEnd();
	}
	glPopMatrix();
	
}

void demo29()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
	pBodyGround->setPosition(Vector2f(0.0f,-0.8f));
	//pBodyGround->setAngle(M_PI_4/5);
	pBodyGround->m_BodyType = EBT_STATIC;
	
	PhysicalAppearance pApp;
	dfloat groundWidth = 1.0f; dfloat groundHeight = 0.02f;
	Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(vertices, 4);
	Vector2f vertex1(-1.5f, -0.2f);
	Vector2f vertex2(1.5f, -0.2f);
	pApp.m_CollisionAttributes.m_Shape = new Edge(vertex1, vertex2);
	pBodyGround->createPhysicalShape(pApp);
	
	// Create Box1
	PhysicalBody* pBodyBox1 = pWorld->createPhysicalBody();
	//pBodyBox->m_BodyType = EBT_STATIC;
	pBodyBox1->setPosition(Vector2f(0.0f,0.1f));
	pBodyBox1->setAngle(-PI_2);
	dfloat boxWidth = 0.051f; dfloat boxHeight = 0.051f;
	Vector2f verticesBox[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
	//pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox, 4);
	pApp.m_CollisionAttributes.m_Shape = new Circle(0.1);
	pBodyBox1->createPhysicalShape(pApp);
	
	// Create Box2
	PhysicalBody* pBodyBox2 = pWorld->createPhysicalBody();
	pBodyBox2->m_LinearDamping = 2.0f;
	//pBodyBox->m_BodyType = EBT_STATIC;
	pBodyBox2->setPosition(Vector2f(0.0f,0.35f));
	pBodyBox2->setAngle(-PI_2);
	pApp.m_CollisionAttributes.m_Shape = new ConvexPolygon(verticesBox, 4);
	PhysicalShape* pShape = pBodyBox2->createPhysicalShape(pApp);
	pShape->setSensor(true);
}

DynamicTreeBroadPhaseAlgorithm* pAlgo = 0;

void renderDynamicTree(DynamicTreeNode* pNode)
{
	return;
	if( pNode->isLeaf() )
	{
		glRenderer->setColor(255, 255, 0);
		glRenderer->setLineWidth(1.0f);
	}
	else
	{
		glRenderer->setColor(255, 255, 255);
		glRenderer->setLineWidth(2.0f);
	}

	glRenderer->drawAABB(pNode->m_AABB);

	if( pNode->isLeaf() )
		return;

	renderDynamicTree( pAlgo->getDynamicTree()->getNode(pNode->m_Child1) );
	renderDynamicTree( pAlgo->getDynamicTree()->getNode(pNode->m_Child2) );
}

void initScene()
{
	pWorld = new PhysicalWorld();
	glRenderer = new GLRenderer(pWorld);
	pWorld->setRenderer(glRenderer);
	pAlgo = (DynamicTreeBroadPhaseAlgorithm*)pWorld->getBroadPhaseAlgorithm();
	demo29();

	mouseJoint = (DistanceConstraint*)pWorld->createConstraint(ECT_DISTANCE);
	mouseJoint->m_Erp = 2.0f;
	mouseJoint->m_Cfm = 1.0f;
}

void changeSize(int w, int h) 
{
	windowWidth  = w;
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


//static dfloat time = 0.0f;
//time += dt;
//// Find Anchor Point Position
//Vector2f ap(0.025f,-0.45f);
//limb2->m_Transform.transformAsPoint(ap);
//dfloat angle = stick->m_Angle;
//Vector2f c = stick->m_Position;
//dfloat r = c.distance(ap);
//dfloat R = r * sin(angle);
//dfloat random = RANDOM_NUMBER(60.0f, 100.0f);
//dfloat tau = dt*random;
//Vector2f A (-R/(tau*tau), 0.0f );
//{
//	if( fabs(R) < 0.1f ) 
//		limb2->m_Velocity.x += A.x * dt;
//		else
//			limb2->m_Velocity.x = 0.0f;
//			}

void renderScene(void) 
{	
#ifndef WIN32
	timeval t1, t2;
    static double elapsedTime;
    double FPS = 0;
    
    static int frameCnt = 0;
    
    // start timer
    gettimeofday(&t1, NULL);
#endif
	
		
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if(windowHeight == 0)
        windowHeight = 1;
	
    dfloat ratio = 1.0* windowWidth / windowHeight;
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();	
	glOrtho(-1.5*ratio, 1.5*ratio, -1.5, 1.5, -0.01, 100);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
	pWorld->Step(dt);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	pWorld->draw();
	
	DynamicTreeBroadPhaseAlgorithm* pAlgo = (DynamicTreeBroadPhaseAlgorithm*)pWorld->getBroadPhaseAlgorithm();
	
	if( pAlgo->getAlgorithmType() == EAT_DYNAMIC_TREE )
	{
		//printf("Height=%d\n", pAlgo->getDynamicTree()->getRootNode()->m_Height);
		if( pAlgo->getDynamicTree() )
			renderDynamicTree(pAlgo->getDynamicTree()->getRootNode() );
	}
	
    //demo28();
	//RaySegment2f raySeg;//(Vector2f(), Vector2f());
//	raySeg.m_End = Vector2f(0.0f,-1.0f);
//	raySeg.m_Start = Vector2f(0.4f,1.6f);
//
//	callBack.numCalls = 0;
//	pWorld->intersectRaySegment(raySeg,&callBack);
//	
//	glPushMatrix();
//	Matrix3f xForm =  callBack.m_pBody->m_Transform;
//	Matrix3f Identity;
//	pWorld->getRenderer()->setTransform(Identity);
//	pWorld->getRenderer()->setTransform(xForm);
//	pWorld->getRenderer()->setColor(0, 255, 255);
//	pWorld->getRenderer()->drawShape(callBack.m_pBody->getPhysicalShapeList()->m_CollisionShape);
//	glPopMatrix();
//
//	//printf("Num Ray Intersections = %d\n", callBack.numCalls);
//
//	glPushMatrix();
//	glBegin(GL_LINES);
//	  glVertex2f(raySeg.m_Start.x, raySeg.m_Start.y); 
//	  glVertex2f(raySeg.m_End.x, raySeg.m_End.y); 
//	glEnd();
//	glPopMatrix();

	glutSwapBuffers();
	
#ifndef WIN32
	frameCnt ++;
    
    // stop timer
    gettimeofday(&t2, NULL);
    
    // compute and print the elapsed time in millisec
    elapsedTime += (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    
    if( frameCnt >=1000 )
    {
        frameCnt=0;
        FPS = 1000 /(elapsedTime * 0.001);
        printf("\n FPS = %f", FPS);
        elapsedTime = 0;
    }
#else
	timer.tick();
#endif
}

static bool bWarmStart = true;

void keyProcessor(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27:
			exit(0);
			break;
		case 127:
		{
            //PhysicalBody* pBody =  pWorld->getPhysicalBodyList();
			//pWorld->deletePhysicalBody(pBody);	
		}
            break;
		case 'w':
		case 'W':
			bWarmStart = !bWarmStart;
			pWorld->setWarmStart(bWarmStart);
			break;
	}
}

int _button = -1;
dbool bPicked = false;

void MouseButton(int button, int state, int x, int y)
{	
	_button = button;
	
	if(windowHeight == 0)
        windowHeight = 1;
	y = windowHeight - y;
    dfloat ratio = 1.0* windowWidth / windowHeight;
	
	dfloat px = 2.0f * (dfloat)x/windowWidth - 1;
	dfloat py = 2.0f * (dfloat)y/windowHeight - 1;
	px *= 1.5f*ratio;
	py *= 1.5f;
	
	Vector2f p(px,py);
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
    {
		PhysicalBody* pBody = pWorld->pickBodyFromScreenCoordinates(p);
		if( pBody )
		{
			bPicked = true;
			Vector2f posLocal = pBody->getLocalPoint(p);
			mouseJoint->m_PhysicalBody1 = pBody;
			mouseJoint->m_Anchor1 = posLocal;
			mouseJoint->m_Anchor2 = p;
			mouseJoint->initialize();
		}
		else 
		{
			bPicked = false;
			mouseJoint->m_PhysicalBody1 = 0;
		}

    }
	if( state == GLUT_UP && button == GLUT_LEFT_BUTTON )
	{
		bPicked = false;
		mouseJoint->m_PhysicalBody1 = 0;
		mouseJoint->m_Anchor1 = Vector2f();
		mouseJoint->m_Anchor2 = Vector2f();
	}
}

void MouseMotion(int x, int y)
{
	if( !bPicked )
		return;
	
	if(windowHeight == 0)
        windowHeight = 1;
	y = windowHeight - y;
    dfloat ratio = 1.0* windowWidth / windowHeight;
	
	dfloat px = 2.0f * (dfloat)x/windowWidth - 1;
	dfloat py = 2.0f * (dfloat)y/windowHeight - 1;
	px *= 1.5f*ratio;
	py *= 1.5f;
	
	Vector2f p(px,py);
	if (_button == GLUT_LEFT_BUTTON)
    {
		mouseJoint->m_Anchor2 = p;
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
	glutMouseFunc (MouseButton);
	glutMotionFunc (MouseMotion);
	glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);
	
#ifndef WIN32
	//glutTimerFunc(0, timerCallback, 0);
#endif
	
	initScene();
	
    glutMainLoop();
	
	return 0;
}