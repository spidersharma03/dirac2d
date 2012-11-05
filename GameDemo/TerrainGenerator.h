//
//  TerrainGenerator.h
//  Dirac2D
//
//  Created by Prashant Sharma on 28/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include "IUpdatable.h"
#include "IRenderable.h"

#ifndef _TERRAIN_GENERATOR_H_
#define _TERRAIN_GENERATOR_H_

#include "../Dirac2D/maths/MathUtil.h"
#include <vector>

using namespace std;
USE_NAMESPACE_DIRAC2D

class FirstGame;

BEGIN_NAMESPACE_DIRAC2D
class PhysicalBody;
END_NAMESPACE_DIRAC2D

#define MAX_TERRAIN_POINTS_ON_SCREEN 10
// B Spline Based Terrain Generator.

typedef Vector2f (*SampleFunction)(float);

class TerrainGenerator : public IUpdatable, IRenderable
{
public:
    TerrainGenerator(FirstGame* pGame);
    
    virtual void update(float dt);
    
    virtual void render();
    
    void setTerrainSlope( const float terrainSlope)
    {
        m_TerrainSlope = terrainSlope;
    }
    
    void setSmoothNess( int smoothness )
    {
        if( smoothness < 2 )smoothness = 2;
        if( smoothness > m_vecTerrainPoints.size()/3-1 ) smoothness = m_vecTerrainPoints.size()/3-1;
        m_SmoothNess = smoothness;
    }

protected:
    
    // Initialize some points around the given input point
    void initializeTerrainPoints(const Vector2f& p);
    
    /* Sample and generate control points of the Terrain  */
    void generateSamplePoints();
    
    /* Generate Points from the  control points for the Terrain */
    void generateBSplineCurvePoints();
    
protected:
    
    FirstGame* m_pGame;
    
    vector<Vector2f> m_vecTerrainPoints;
    
    float m_TerrainSlope;
    float m_MaxTerrainHeight;
    
    int m_NumTerrainControlPoints;
    int m_SmoothNess;
    
    float curvePoints[1000];
    Vector2f m_TerrainPoints[500];
    int m_NumTerrainPoints;
    PhysicalBody* m_pTerrainBody;
    
    SampleFunction m_SampleFunction;
};

#endif
