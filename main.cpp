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
	//pBodyBox->setAngle(PI_4*0.99);
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
	dint32 n = 10;
	dfloat dx = 0.0f;
	for(int j=0; j<10;j++ )
	{
		y += 0.2f;
		x = -1.0 + dx;
		dx += 0.1f;
		for( int i=0; i<n; i++ )
		{
			//if( i== 0 && j == 0 )continue;
			PhysicalBody* pBodyBox = pWorld->createPhysicalBody();
			//pBodyBox->m_BodyType = EBT_STATIC;
			x += 0.2f;
			pBodyBox->setPosition(Vector2f(x,y));
			//pBodyBox->setAngle(M_PI_4*0.9);
			dfloat boxWidth = 0.07f; dfloat boxHeight = 0.07f;
			Vector2f verticesBox[4] = { Vector2f(boxWidth, boxHeight), Vector2f(-boxWidth, boxHeight), Vector2f(-boxWidth, -boxHeight), Vector2f(boxWidth, -boxHeight) };
			pApp.m_CollisionAttributes.m_Shape = new RegularPolygon(verticesBox, 4);
			pBodyBox->createPhysicalShape(pApp);
		}
		n -= 1;
	}
}


void initScene()
{
	pWorld = new PhysicalWorld();
	GLRenderer* glRenderer = new GLRenderer(pWorld);
	pWorld->setRenderer(glRenderer);
	
	demo1();
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
	glOrtho(-1*ratio, 1*ratio, -1, 1, -0.01, 100);
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
	glOrtho(-1*ratio, 1*ratio, -1, 1, -0.01, 100);
	pWorld->Step(dt);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	pWorld->draw();
	
    glutSwapBuffers();
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
	glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);

	
#ifndef WIN32
	glutTimerFunc(0, timerCallback, 0);
#endif
	
	//MemoryAllocator<Vector2f> memAllocator(10);
//	
//	Vector2f *v1 = new (memAllocator.Allocate()) Vector2f(1,0);
//	Vector2f *v2 = new (memAllocator.Allocate()) Vector2f(2,0);
//	//memAllocator.Free(v2);
//	Vector2f *v3 = new (memAllocator.Allocate()) Vector2f(3,0);
//	memAllocator.Free(v3);
//	Vector2f *v4 = new (memAllocator.Allocate()) Vector2f(4,0);
//	memAllocator.Free(v4);
	//Vector2f *v3 = new (memAllocator.Allocate()) Vector2f(3,0);
	//memAllocator.Free(v2);
	//Vector2f *v4 = new (memAllocator.Allocate()) Vector2f(4,0);

	//memAllocator.Free(v3);
	//memAllocator.Free(v4);
	//Vector2f* temp = v3;

	//Vector2f* v = memAllocator[0];
//	v = memAllocator[1];
//	v = memAllocator[2];
//	v = memAllocator[3];
//	
//	
//	v = memAllocator[0];
//	v = memAllocator[1];
//	v = memAllocator[2];
	
	initScene();
	
    glutMainLoop();
	
	return 0;
}