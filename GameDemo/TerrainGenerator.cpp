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


float knots[130];
float controlPoints[200];

int numSampleFunctions = 4;

bool SortPredicate(const Vector2f& p1, const Vector2f& p2)
{
    return p1.x < p2.x;
}

TerrainGenerator::TerrainGenerator(FirstGame* pGame)
{
    m_pGame = pGame;
    m_vecTerrainPoints.reserve(3*MAX_TERRAIN_POINTS_ON_SCREEN);
    m_TerrainSlope = 0.0f;
    m_MaxTerrainHeight = 0.5f;
    m_AvgLevel = 0.0f;
    m_SmoothNess = 3;
    
    m_vecTerrainSampleFunctions.push_back(new LinearSampleFunction(-PI_4/20));
    m_vecTerrainSampleFunctions.push_back(new SinSampleFunction());
    m_vecTerrainSampleFunctions.push_back(new SquareSampleFunction(2.0f));
    m_vecTerrainSampleFunctions.push_back(new LinearSampleFunction(-PI_4/2));
    m_vecTerrainSampleFunctions.push_back(new SawToothSampleFunction(8.0f));
    
    m_pCurrentSampler = m_vecTerrainSampleFunctions[0];
    
    m_vecTerrainPoints.push_back(Vector2f(-0.9f,0.0f));
    
    nCurvePoints = 120;

    m_vecAvgSamplePoints.push_back(Vector2f());
    m_vecAvgSamplePoints.push_back(Vector2f(50.0f,0.0f));
    
	generateSamplePoints();
    
    // Sort the Points    
    std::sort(m_vecAvgSamplePoints.begin(), m_vecAvgSamplePoints.end(), SortPredicate);
    
	generateBSplineCurvePoints();
	
	m_pTerrainBody = m_pGame->getPhysicalWorld()->createPhysicalBody();
    m_pTerrainBody->m_BodyType = EBT_STATIC;
	

    PhysicalAppearance pApp;
    pApp.m_PhysicalAttributes.m_Friction = 5.0f;
    
	Vector2f vertices[300];
	
	for( int i=0; i<2*nCurvePoints-4; i+=2)
	{
		vertices[i/2] = Vector2f(curvePoints[i], curvePoints[i+1]);
	}
    
	EdgeChainInfo eInfo(vertices, (2*nCurvePoints-4)/2);
    pApp.m_CollisionAttributes.m_CollisionShapeInfo = &eInfo;
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

    // Remove sample points from the vector which can't make the range which contain the vehicle.
    for( int i=0; i<(int)m_vecAvgSamplePoints.size()-1; i++ )
    {
        Vector2f& p0 = m_vecAvgSamplePoints[i];
        Vector2f& p1 = m_vecAvgSamplePoints[i+1];
        float vehiclex = m_pGame->getVehicle()->getPosition().x;
        
        bool bRes =  vehiclex > p0.x && vehiclex > p1.x;
        if( bRes )
        {
            i = 0;
            m_vecAvgSamplePoints.erase(m_vecAvgSamplePoints.begin() + i);
        } 
    }
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
        Vector2f samplePoint = (*m_pCurrentSampler)(t);
        Vector2f p =  m_LastSamplePoint + samplePoint;
		
        if( fabs(pCamera->getPosition().x - p.x) < R )
        {
            t += delta;
            
            if( m_pCurrentSampler->getSampleFunctionType() == ESFT_LINEAR )
            {
                Vector2f point = m_LastSamplePoint + samplePoint;
                m_vecAvgSamplePoints.push_back(point);
            }
            else
            {
                Vector2f point = m_LastSamplePoint + Vector2f(samplePoint.x, 0.0f);
                m_vecAvgSamplePoints.push_back(point);
            }
            
            p.y += RANDOM_NUMBER(low, high);
            m_vecTerrainPoints.push_back(p);
        }
        else
        {
            t -= delta;
            break;
        }
    }
	// Sort the Points    
    std::sort(m_vecTerrainPoints.begin(), m_vecTerrainPoints.end(), SortPredicate);
    std::sort(m_vecAvgSamplePoints.begin(), m_vecAvgSamplePoints.end(), SortPredicate);
}

float TerrainGenerator::getAvgLevel(float x)
{
    int index = -1;
    for( int i=0; i<m_vecAvgSamplePoints.size()-1; i++ )
    {
        if( x >= m_vecAvgSamplePoints[i].x && x < m_vecAvgSamplePoints[i+1].x )
        {
            index = i;
            break;
        }
    }
    
    //dAssert(index != -1);
    
    float m = (m_vecAvgSamplePoints[index+1].y - m_vecAvgSamplePoints[index].y)
    /(m_vecAvgSamplePoints[index+1].x - m_vecAvgSamplePoints[index].x);
    float c = m_vecAvgSamplePoints[index].y - m * m_vecAvgSamplePoints[index].x;
        
    m_pGame->getRenderer()->setColor(0, 255, 0);
    m_pGame->getRenderer()->setPointSize(6.0f);
    m_pGame->getRenderer()->drawPoint(Vector2f(x,m*x+c));
    m_pGame->getRenderer()->setPointSize(1.0f);
    
    return m * x + c;
}

int f = 1;

void TerrainGenerator::changeTerrainShape()
{
	double currentTime = m_Timer.getCurrentTime();
	if( currentTime - m_StartTime > m_TerrainSwitchTime )
	{
		printf("Terrain Shape Changed\n");
		m_StartTime = currentTime;
        m_pCurrentSampler = m_vecTerrainSampleFunctions[f];
        
        f++;
		f = f > m_vecTerrainSampleFunctions.size()-1 ? 0 : f;
        
		m_LastPoint = m_vecTerrainPoints[m_vecTerrainPoints.size() - 1];
        m_LastSamplePoint = m_vecAvgSamplePoints[m_vecAvgSamplePoints.size() - 1];
        
        //printf(" Last Sample Point = %f   %f\n", m_LastSamplePoint.x, m_LastSamplePoint.y);
        // Reset the time, so that the new terrain points will start from the last point.
		t = 0.0f;
	}
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
	m_pTerrainBody->m_BodyType = EBT_STATIC;
	
    EdgeChainInfo eInfo(vertices, (2*nCurvePoints-6)/2);
    pApp.m_CollisionAttributes.m_CollisionShapeInfo = &eInfo;	
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

