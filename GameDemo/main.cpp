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
#include "TerrainGenerator.h"
#include "Camera.h"
#include "SimpleVehicle.h"


void initScene();
void changeSize(int w, int h);
void renderScene(void);

//PhysicalWorld* pWorld;
GLRenderer* glRenderer;
DTimer timer;
FirstGame* pGame;

dfloat dt = 1.0f/1400.0f;

dint32 windowWidth   = 800;
dint32 windowHeight = 600;

DistanceConstraint *mouseJoint;


void initScene()
{
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
	
    pGame->getCamera()->setAspectRatio(ratio);
    // Reset the coordinate system before modifying
	
    // Set the viewport to be the entire window
	
    glViewport(0, 0, w, h);
}


void renderScene(void) 
{	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if(windowHeight == 0)
        windowHeight = 1;
		
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
	pGame->gameLoop();
	
	pGame->render();
	
    //pWorld->draw();

	glutSwapBuffers();
	
	//timer.tick();
}

//static bool bWarmStart = true;
int smoothNess = 2;

void keyProcessor(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'd':
		case 'D':
			pGame->getVehicle()->applyTorqueImpulse(-3.5f);

			break;
			
		case 'a':
		case 'A':
			pGame->getVehicle()->applyTorqueImpulse(3.5f);

			break;
		case 32:
			//pGame->getVehicle()->applyImpulse(Vector2f(0.0f,-300.0f));
			break;
		case 27:
			exit(0);
			break;
		
		case 'w':
		case 'W':
            smoothNess--;
            smoothNess = smoothNess < 2 ? 2 : smoothNess;
			//bWarmStart = !bWarmStart;
            pGame->getTerrainGenerator()->setSmoothNess(smoothNess);
			//pWorld->setWarmStart(bWarmStart);
			break;
        case 's':
		case 'S':
            smoothNess++;
			//bWarmStart = !bWarmStart;
            pGame->getTerrainGenerator()->setSmoothNess(smoothNess);
			//pWorld->setWarmStart(bWarmStart);
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
	if ( 0 )//state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
    {
		PhysicalBody* pBody = 0;//pWorld->pickBodyFromScreenCoordinates(p);
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
	if( 0 )//state == GLUT_UP && button == GLUT_LEFT_BUTTON )
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
    //glutFullScreen();
	
#ifndef WIN32
	//glutTimerFunc(0, timerCallback, 0);
#endif
	
	initScene();
	
    glutMainLoop();
	
	return 0;
}