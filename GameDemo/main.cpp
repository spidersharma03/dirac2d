#include <iostream>

#ifndef WIN32 
#include <sys/time.h>
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif

#include "../Dirac2D/Dirac2D.h"

USE_NAMESPACE_DIRAC2D

#include "FirstGame.h"

void initScene();
void changeSize(int w, int h);
void renderScene(void);

PhysicalWorld* pWorld;
GLRenderer* glRenderer;
DTimer timer;
FirstGame* pGame;

dfloat dt = 1.0f/600.0f;

dint32 windowWidth   = 800;
dint32 windowHeight = 600;

DistanceConstraint *mouseJoint;


// Wheel Constraint
void demo23()
{
	// Create Ground Body
	PhysicalBody* pBodyGround = pWorld->createPhysicalBody();
    pBodyGround->setAngle(PI_4/10);
	pBodyGround->setPosition(Vector2f(1.0f,-0.7f));
	pBodyGround->m_BodyType = EBT_STATIC;
	dfloat groundWidth = 1.2f;  dfloat groundHeight = 0.02f;
	//Vector2f vertices[4] = { Vector2f(groundWidth, groundHeight), Vector2f(-groundWidth, groundHeight), Vector2f(-groundWidth, -groundHeight), Vector2f(groundWidth, -groundHeight) };
	PhysicalAppearance pApp;
    pApp.m_PhysicalAttributes.m_Friction = 1.0f;
	
	dint32 edgeCount = 30;
	Vector2f vertices[30];
	dfloat chainLength = 3.0f;
	dfloat ex = chainLength/2;
	dfloat ey = 0.0f;
	dfloat dx = 2*chainLength/(edgeCount-1);
	
	vertices[0] = Vector2f(-ex,0.0f);
	vertices[1] = Vector2f(0.0f,0.8f);
	vertices[2] = Vector2f(ex,0.0f);
	
	for( dint32 e=0; e<edgeCount; e++ )
	{
		vertices[e] = Vector2f(ex,ey);
		ex -= dx;
		ey = -0.03*sin(e*1.39);
	}
	
	pApp.m_CollisionAttributes.m_Shape = new EdgeChain(vertices, edgeCount);
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
		pApp.m_CollisionAttributes.m_Shape = new Capsule(0.03f, 0.6f);
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
		lc1->m_Cfm = 4.0f;
		
		WheelConstraint* lc2 = (WheelConstraint*)pWorld->createConstraint(ECT_WHEEL);
		lc2->m_PhysicalBody1 = pBox1;
		lc2->m_PhysicalBody2 = circle2;
		lc2->m_Anchor = Vector2f(-0.3f,-0.2f);
		lc2->m_LocalAxis = Vector2f(-1.0f,-1.0f);
		lc2->initialize();
		lc2->m_Erp = 50.0f;
		lc2->m_Cfm = 4.0f;
		
        MotorConstraint* mc = (MotorConstraint*)pWorld->createConstraint(ECT_MOTOR);
        mc->m_PhysicalBody1 = circle2;
		mc->m_MaxTorque = 10.0f;
		mc->m_Speed = 20.0f;
		mc->initialize();
	}
	// Create Circle Chain
}


void initScene()
{
	pWorld = new PhysicalWorld();
	glRenderer = new GLRenderer(pWorld);
	pWorld->setRenderer(glRenderer);
	demo23();
    
	mouseJoint = (DistanceConstraint*)pWorld->createConstraint(ECT_DISTANCE);
	mouseJoint->m_Erp = 2.0f;
	mouseJoint->m_Cfm = 1.0f;
    
    pGame = new FirstGame();
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
	pWorld->Step(dt);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	pWorld->draw();
	
	pGame->gameLoop();
	
	pGame->render();
	
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
    glutCreateWindow("GAME DEMO - DIRAC");
	glutKeyboardFunc(keyProcessor);
	glutMouseFunc (MouseButton);
	glutMotionFunc (MouseMotion);
	glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutFullScreen();
	
#ifndef WIN32
	//glutTimerFunc(0, timerCallback, 0);
#endif
	
	initScene();
	
    glutMainLoop();
	
	return 0;
}