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
#include "../Dirac2D/Dirac2D.h"

#include <algorithm>


Vector2f sawToothSample(float t )
{
    float x = t;
    float y = 0.5f * fmod(x, 6.0f) - 2.0f;
    return Vector2f(x,y);
}

Vector2f squareWaveSample( float t )
{
    float x = t;
    float y = fmod(x, 12.0f);

    if( y  < 6.0f )
        y = 1.0f;
    else
    {
        y = -1.0f;
    }
    return Vector2f(x,y);
}

Vector2f linearSample( float t )
{
    float slope = M_PI_4/4;
    float x = cos(slope);
    float y = sin(slope);
    return Vector2f( x*t, y*t);
}

TerrainGenerator::TerrainGenerator(FirstGame* pGame)
{
    m_pGame = pGame;
    m_vecTerrainPoints.reserve(3*MAX_TERRAIN_POINTS_ON_SCREEN);
    m_TerrainSlope = 0.0f;
    m_MaxTerrainHeight = 0.5f;
    m_SmoothNess = 2;
    
    m_vecTerrainPoints.push_back(Vector2f(0.0f,0.0f));
    
    m_pTerrainBody = m_pGame->getPhysicalWorld()->createPhysicalBody();
    
    m_pTerrainBody->setAngle(PI_4/10);
	m_pTerrainBody->setPosition(Vector2f(1.0f,-0.7f));
	m_pTerrainBody->m_BodyType = EBT_STATIC;
    
    PhysicalAppearance pApp;
    pApp.m_PhysicalAttributes.m_Friction = 1.0f;
    
    m_SampleFunction = squareWaveSample;
}

void TerrainGenerator::update(float dt)
{
    // Sample and generate control points of the Terrain 
    generateSamplePoints();
    
    
    generateBSplineCurvePoints();

    if( 0 )
    {
    // Create new Physical Body for the terrain
        
    // 1. delete old terrain body.    
    //m_pGame->getPhysicalWorld()->deletePhysicalBody(m_pTerrainBody);
    // 2. create new terrain body using the new points.     
    }
}
float knots[130], basis[30];
float controlPoints[200];
int nCurvePoints = 120;


void TerrainGenerator::render()
{
	glPushMatrix();   
	for( int i=0; i<2*nCurvePoints-4; i+=2 )
	{
		glBegin(GL_LINES);
        glVertex2f(curvePoints[i], curvePoints[i+1]);
        glVertex2f(curvePoints[i+2], curvePoints[i+3]);
		glEnd();
	}
	glPopMatrix();
    
//    glPushMatrix();
//    glBegin(GL_LINES);
//    for (int i=0; i<m_vecTerrainPoints.size()-1; i++) 
//    {
//        Vector2f& p1 = m_vecTerrainPoints[i];
//        Vector2f& p2 = m_vecTerrainPoints[i+1];
//        glVertex2f(p1.x, p1.y);
//        glVertex2f(p2.x, p2.y);
//        //glColor3b(255, 255, 0);
//    }
//    glEnd();
//    glPopMatrix();
}


bool SortPredicate(const Vector2f& p1, const Vector2f& p2)
{
    return p1.x < p2.x;
}

void TerrainGenerator::generateBSplineCurvePoints()
{
    int c = 0;
    for( int i=0; i<m_vecTerrainPoints.size(); i++ )
    {
        Vector2f& p = m_vecTerrainPoints[i];
        controlPoints[c] =  p.x;
        controlPoints[c+1] =  p.y;
        c += 2;    
    }
    
	findOpenKnots(m_vecTerrainPoints.size(), m_SmoothNess, knots);
    
    BSpline(controlPoints, m_vecTerrainPoints.size(), curvePoints, nCurvePoints, knots, m_SmoothNess);
}

void TerrainGenerator::generateSamplePoints()
{
    Camera* pCamera = m_pGame->getCamera();
    float swh = pCamera->getScreenWidth() * 0.5f;

    float R = ( swh + swh * 2.0f ); 

    for( int i=0; i<m_vecTerrainPoints.size(); i++ )
    {
        Vector2f& p = m_vecTerrainPoints[i];
        bool bRes =  fabs(pCamera->getPosition().x - p.x) > R;
        if( bRes )
        {
            m_vecTerrainPoints.erase(m_vecTerrainPoints.begin() + i);
        }
    }
    
   
    float delta = swh * 2.0f * 3.0f/(3*MAX_TERRAIN_POINTS_ON_SCREEN);
    // Sample new points on the line, passing through the last point and having given slope.
    float low  = -m_MaxTerrainHeight * 0.5f;
    float high = m_MaxTerrainHeight * 0.5f;

    static float t = 0.0f;
    t += delta;

    while (true)
    {
        Vector2f p =   m_SampleFunction(t);

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
        // Sort the Points    
    //printf("Num TerrainPoints = %d\n", m_vecTerrainPoints.size());
    }
    std::sort(m_vecTerrainPoints.begin(), m_vecTerrainPoints.end(), SortPredicate);
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