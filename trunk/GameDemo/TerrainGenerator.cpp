//
//  TerrainGenerator.cpp
//  Dirac2D
//
//  Created by Prashant Sharma on 28/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "TerrainGenerator.h"
#include "FirstGame.h"
#include "Camera.h"
#include "ObjectFactory.h"
#include "../Dirac2D/Dirac2D.h"
#include "SimpleVehicle.h"

#include <algorithm>


Vector2f sawToothSample(float t )
{
    float x = t;
    float y = 0.4f * fmod(x, 10.0f) - 1.5f;
    return Vector2f(x,y);
}

Vector2f squareWaveSample( float t )
{
    float x = t;
	float A = 122.0f;
    float y = fmod(x, A);

    if( 2.0f*y  < A )
        y = 0.0f;
    else
    {
        y = -2.25f;
    }
    return Vector2f(x,y);
}

Vector2f linearSample( float t )
{
    float slope = PI_4/1112;
    float x = cos(slope);
    float y = sin(slope);
    return Vector2f( x*t, y*t);
}

Vector2f sinWaveSample( float t )
{
	float x = t;
	float y = 1.0f*sin(0.6f*x);
	return Vector2f( x, y);
}

float knots[130];
float controlPoints[200];

int numSampleFunctions = 4;
SampleFunction funcArray[] = {linearSample, squareWaveSample, sinWaveSample, sawToothSample};
//SampleFunction funcArray[] = {squareWaveSample, linearSample, sinWaveSample, sinWaveSample};

TerrainGenerator::TerrainGenerator(FirstGame* pGame)
{
    m_pGame = pGame;
    m_vecTerrainPoints.reserve(3*MAX_TERRAIN_POINTS_ON_SCREEN);
    m_TerrainSlope = 0.0f;
    m_MaxTerrainHeight = 0.5f;
    m_SmoothNess = 3;
    
	m_SampleFunction = linearSample;

    m_vecTerrainPoints.push_back(Vector2f(-0.9f,0.0f));
    
    nCurvePoints = 120;

	generateSamplePoints();
	generateBSplineCurvePoints();
	
	m_pTerrainBody = m_pGame->getPhysicalWorld()->createPhysicalBody();
    
    //m_pTerrainBody->setAngle(PI_4/10);
	m_pTerrainBody->setPosition(Vector2f(0.0f,-0.7f));
	m_pTerrainBody->m_BodyType = EBT_STATIC;
	

    PhysicalAppearance pApp;
    pApp.m_PhysicalAttributes.m_Friction = 5.0f;
    
	Vector2f vertices[300];
	
	for( int i=0; i<2*nCurvePoints-4; i+=2)
	{
		vertices[i/2] = Vector2f(curvePoints[i], curvePoints[i+1]);
	}
		
	pApp.m_CollisionAttributes.m_Shape = new EdgeChain(vertices, (2*nCurvePoints-4)/2);
	m_pTerrainBody->createPhysicalShape(pApp);
	
	m_StartTime = m_Timer.getCurrentTime();
	
	m_TerrainSwitchTime = 10000.0;
	
}

void TerrainGenerator::update(float dt)
{
	changeTerrainShape();
	
    // Sample and generate control points of the Terrain 
    generateSamplePoints();
    
    
    generateBSplineCurvePoints();

    //
	initializeTerrainBody();
}

void TerrainGenerator::initializeTerrainBody()
{
	//return;	
    // 1. delete old terrain body.    
	m_pGame->getPhysicalWorld()->deletePhysicalBody(m_pTerrainBody);
    // 2. create new terrain body using the new points.   
	PhysicalAppearance pApp;
    pApp.m_PhysicalAttributes.m_Friction = 1.0f;
    pApp.m_PhysicalAttributes.m_Restitution = 0.5f;

	Vector2f vertices[300];
	
	for( int i=0; i<2*nCurvePoints-4; i+=2)
	{
		vertices[i/2] = Vector2f(curvePoints[i], curvePoints[i+1]);
	}
	
	m_pTerrainBody = m_pGame->getPhysicalWorld()->createPhysicalBody();
	m_pTerrainBody->setPosition(Vector2f(0.0f,-0.7f));

    //m_pTerrainBody->setAngle(PI_4/10);
	//m_pTerrainBody->setPosition(Vector2f(1.0f,-0.7f));
	m_pTerrainBody->m_BodyType = EBT_STATIC;
	
	pApp.m_CollisionAttributes.m_Shape = new EdgeChain(vertices, (2*nCurvePoints-6)/2);
	
	PhysicalShape* pShape = m_pTerrainBody->createPhysicalShape(pApp);
	pShape->m_CollisionFilter.m_CollisionBit = EOCB_TERRAIN;
}

void TerrainGenerator::render()
{
	return;
	glPushMatrix();   
	for( int i=0; i<2*nCurvePoints-4; i+=2 )
	{
		glBegin(GL_LINES);
        glVertex2f(curvePoints[i], curvePoints[i+1]);
        glVertex2f(curvePoints[i+2], curvePoints[i+3]);
		glEnd();
	}
	glPopMatrix();
}


bool SortPredicate(const Vector2f& p1, const Vector2f& p2)
{
    return p1.x < p2.x;
}

void TerrainGenerator::generateBSplineCurvePoints()
{
    int c = 0;
	int size = m_vecTerrainPoints.size();
    for( int i=0; i<size; i++ )
    {
        Vector2f& p = m_vecTerrainPoints[i];
	
        controlPoints[c] =  p.x;
        controlPoints[c+1] =  p.y;
        c += 2;
    }
    
	findOpenKnots(m_vecTerrainPoints.size(), m_SmoothNess, knots);
    
    BSpline(controlPoints, m_vecTerrainPoints.size(), curvePoints, nCurvePoints, knots, m_SmoothNess);
}

static float t = 0.0f;

void TerrainGenerator::generateSamplePoints()
{
    Camera* pCamera = m_pGame->getCamera();
    float swh = pCamera->getScreenWidth() * 0.5f;

    float R = ( swh + swh * 2.0f ); 

	// Remove points which are left behind by more than 2 screens.
    for( int i=0; i<m_vecTerrainPoints.size(); i++ )
    {
        Vector2f& p = m_vecTerrainPoints[i];
        bool bRes =  (pCamera->getPosition().x - p.x) > R;
        if( bRes )
        {
            m_vecTerrainPoints.erase(m_vecTerrainPoints.begin() + i);
        }
    }
    
   
    float delta = swh * 2.0f * 3.0f/(3*MAX_TERRAIN_POINTS_ON_SCREEN);

    float low  = -m_MaxTerrainHeight * 0.5f;
    float high = m_MaxTerrainHeight * 0.5f;

	// Add points at a time.
    t += delta;
    while (true)
    {
        Vector2f p =  m_LastPoint + m_SampleFunction(t);
		
        if( fabs(pCamera->getPosition().x - p.x) < R )
        {
            t += delta;
            p.y += RANDOM_NUMBER(low, high);
            m_vecTerrainPoints.push_back(p);
        }
        else
        {
            t -= delta;
            break;
        }
    //printf("Num TerrainPoints = %d\n", m_vecTerrainPoints.size());
    }
	// Sort the Points    
    std::sort(m_vecTerrainPoints.begin(), m_vecTerrainPoints.end(), SortPredicate);
}

int f = 0;

void TerrainGenerator::changeTerrainShape()
{
	double currentTime = m_Timer.getCurrentTime();
	if( currentTime - m_StartTime > m_TerrainSwitchTime )
	{
		printf("Terrain Shape Changed\n");
		m_SampleFunctionType = (SAMPLE_FUNCTION_TYPE)f;
		m_StartTime = currentTime;
		f++;
        if( f == 3 )
        {
            //m_pGame->getVehicle()->setMotorSpeed(m_pGame->getVehicle()->getMotorSpeed() + 100);
        }
		f = f > 3 ? 0 : f;
		m_SampleFunction = funcArray[f];
		m_LastPoint = m_vecTerrainPoints[m_vecTerrainPoints.size()-1];
		// Reset the time, so that the new terrain points will start from the last point.
		t = 0.0f;
	}
}

void TerrainGenerator::initializeTerrainPoints(const Vector2f& p)
{
    Camera* pCamera = m_pGame->getCamera();
    float swh = pCamera->getScreenWidth() * 0.5f;
    float s = -( swh + swh * 2.0f ); 
    float delta = swh * 2.0f * 3.0f/(3*MAX_TERRAIN_POINTS_ON_SCREEN);
    
    for( int i=0; i<3*MAX_TERRAIN_POINTS_ON_SCREEN; i++ )
    {
        m_vecTerrainPoints.push_back(Vector2f(s, pCamera->getPosition().y));
        s += delta;
    }
    m_NumTerrainPoints = 3*MAX_TERRAIN_POINTS_ON_SCREEN;
}
