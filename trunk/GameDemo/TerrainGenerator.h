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
#include "../Dirac2D/DTimer.h"
#include <vector>

using namespace std;
USE_NAMESPACE_DIRAC2D

class FirstGame;

BEGIN_NAMESPACE_DIRAC2D
class PhysicalBody;
END_NAMESPACE_DIRAC2D

#define MAX_TERRAIN_POINTS_ON_SCREEN 6
// B Spline Based Terrain Generator.

typedef Vector2f (*SampleFunction)(float);
enum SAMPLE_FUNCTION_TYPE { ESFT_LINEAR, ESFT_SIN_WAVE, ESFT_SQUARE_WAVE, ESFT_SAW_TOOTH };

class TerrainSampleFunction
{
  public:    
    virtual Vector2f operator()( float t ) = 0;

    SAMPLE_FUNCTION_TYPE getSampleFunctionType()
    {
        return m_SampleFunctionType;
    }
protected:
    SAMPLE_FUNCTION_TYPE m_SampleFunctionType;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class LinearSampleFunction : public TerrainSampleFunction
{
public:
    LinearSampleFunction( float slope = 0.0f ) : m_Slope(slope)
    {
        m_SampleFunctionType = ESFT_LINEAR;
    }
public:
    Vector2f operator()( float t )
    {
        float x = cos(m_Slope);
        float y = sin(m_Slope);
        return Vector2f( x*t, y*t);
    }
        
private:
    float m_Slope;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SinSampleFunction : public TerrainSampleFunction
{
public:
    SinSampleFunction( float amplitude = 1.0f ) : m_Amplitude(amplitude)
    {
        m_SampleFunctionType = ESFT_SIN_WAVE;
    }
public:
    Vector2f operator()( float t )
    {
        float x = t;
        float y = m_Amplitude * sin(0.6f*x);
        return Vector2f( x, y);
    }
   
private:
    float m_Amplitude, m_Period;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SquareSampleFunction : public TerrainSampleFunction
{
public:
    SquareSampleFunction( float amplitude = 1.0f, float period = 120.0f ) : m_Amplitude(amplitude), m_Period(period)
    {
        m_SampleFunctionType = ESFT_SQUARE_WAVE;
        m_Amplitude = m_Amplitude < 0.0f ? 0.0f : m_Amplitude;
        m_Period = m_Period < 50.0f ? 50.0f : m_Period;
    }
        
public:
    Vector2f operator()( float t )
    {
        float x = t;
        float y = fmod(x, m_Period);
        
        if( 2.0f*y  < m_Period )
            y = m_Amplitude/2;
        else
        {
            y = -m_Amplitude/2;
        }
        return Vector2f(x,y);
    }
    
private:
    float m_Amplitude, m_Period;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SawToothSampleFunction : public TerrainSampleFunction
{
public:
    SawToothSampleFunction( float amplitude = 1.0f, float period = 15.0f ) : m_Amplitude(amplitude), m_Period(period)
    {
        m_SampleFunctionType = ESFT_SAW_TOOTH;
        m_Period = m_Period < 10.0f ? 10.0f : m_Period;
        m_Amplitude = m_Amplitude < 0.0f ? 0.0f  : m_Amplitude;
    }
public:
    Vector2f operator()( float t )
    {
        float x = t;
        float A = m_Amplitude/(2.0f*m_Period);
        float y = A * fmod(x, m_Period) - A;
        return Vector2f(x,y);
    }
    
private:
    float m_Amplitude, m_Period;
};

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
	
	float* getCurvePointsArray()
	{
		return curvePoints;
	}
    
    int getNumTerrainPoints()
    {
        return nCurvePoints;
    }
	
	SAMPLE_FUNCTION_TYPE getSampleFunctionType()
	{
		return m_pCurrentSampler->getSampleFunctionType();
	}
	
    /* Get the Average Y Level of the terrain at a given x position. this is used for auto zoom. */
    float getAvgLevel(float x);

protected:
    
	
	void initializeTerrainBody();
    
    /* Sample and generate control points of the Terrain  */
    void generateSamplePoints();
    
    /* Generate Points from the  control points for the Terrain */
    void generateBSplineCurvePoints();
    
	
	/* Change Shape of the Terrain */
	void changeTerrainShape();
		
    
protected:
    
    FirstGame* m_pGame;
    
    vector<Vector2f> m_vecTerrainPoints;
    
    float m_TerrainSlope;
    float m_MaxTerrainHeight;
    float m_AvgLevel; // Average Y Level of the Terrain
    
    int m_NumTerrainControlPoints;
    int m_SmoothNess;
    
    float curvePoints[1000];
    Vector2f m_TerrainPoints[500];
    int m_NumTerrainPoints;
    PhysicalBody* m_pTerrainBody;
    
    //SampleFunction m_SampleFunction;
	
	DTimer m_Timer;
	double m_StartTime;
	
	double m_TerrainSwitchTime;
	Vector2f m_LastPoint; // 
    Vector2f m_LastSamplePoint;
    
    vector<Vector2f> m_vecAvgSamplePoints;

	int nCurvePoints;
	//SAMPLE_FUNCTION_TYPE m_SampleFunctionType;
    vector<TerrainSampleFunction*> m_vecTerrainSampleFunctions;
    TerrainSampleFunction* m_pCurrentSampler;
};

#endif
