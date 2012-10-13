/*
 *  BSpline.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 04/10/12.
 *
 */

#include "BSpline.h"
#include "GLRenderer.h"
#include "Vector2.h"

USE_NAMESPACE_DIRAC2D

GLRenderer* pglRenderer = new GLRenderer(0);

void findOpenKnots(int nControlPoints, int nOrder, float* knotVector)
{
	for( int i=0; i<nOrder; i++ )
		knotVector[i] = 0.0f;
	for( int i=nOrder+nControlPoints-1; i>nOrder+nControlPoints-4; i-- )
		knotVector[i] = nControlPoints - nOrder + 1;
	int n = 1;
	for( int i=nOrder; i<nOrder+nControlPoints-3; i++ )
		knotVector[i] = n++;	
}


void splineBasis(int nControlPoints, int nOrder, float t, float* knotVector, float* basisVector)
{
	float N_i1[40];  // First Order basis functions
	
	
	// initialize the first order basis functions
	for (int i=0; i<nControlPoints + nOrder - 1; i++) 
	{
		if( t >= knotVector[i] && t < knotVector[i+1] )
			N_i1[i] = 1.0f;
		else
			N_i1[i] = 0.0f;
	}
	//if(t == (float)knotVector[nControlPoints + nOrder])
//	{
//		N_i1[nControlPoints] = 1.0f;
//	}
	int N = nControlPoints + nOrder - 1;
	// Calculate higher order basis functions
	for( int o=1; o<nOrder; o++ )
	{
		for( int i=0; i<N-1; i++ )
		{
			float a, b;
			if( N_i1[i] != 0.0f )
			{
				a = ( t - knotVector[i] )/( knotVector[i+nOrder-1] - knotVector[i] ) * N_i1[i];
			}
			else {
				a = 0.0f;
			}
			if( N_i1[i+1] != 0.0f )
			{
				b = ( knotVector[i+nOrder] - t)/(knotVector[i+nOrder] - knotVector[i+1] ) *  N_i1[i+1];
			}
			else {
				b = 0.0f;
			}

			N_i1[i] = a + b;
		}
		N = N - 1;
	}	
	for (int i=0; i<nControlPoints; i++ )
	{
		basisVector[i] = N_i1[i];
	}
}

void BSpline(float* controlPoints, int nControlPoints, float* curvePoints, int nCurvePoints, int nOrder)
{
	float x[30];     // Knot Vector
	float basis[30];     // Knot Vector

	findOpenKnots(nControlPoints, nOrder, x);
	float t = 0.0f;
	float tStep = (float)(nControlPoints - nOrder + 1)/(nCurvePoints-1);
	for( int i=0; i<2*nCurvePoints; i+=2 )
	{
		splineBasis( nControlPoints, nOrder, t, x, basis);
		
		float s1 = 0.0f;
		float s2 = 0.0f;

		for (int j=0; j<2*nControlPoints; j+=2) 
		{
			s1 += basis[j/2] * controlPoints[j];
			s2 += basis[j/2] * controlPoints[j+1];
		}
		curvePoints[i]   = s1;
		curvePoints[i+1] = s2;
		t += tStep;
	}
}

void drawBSpline(float* curvePoints, int nCurvePoints)
{
	pglRenderer->setColor(0, 255, 0);
	for( int i=0; i<2*nCurvePoints-2; i+=2 )
	{
		Vector2f p0(curvePoints[i], curvePoints[i+1]);
		Vector2f p1(curvePoints[i+2], curvePoints[i+3]);
		pglRenderer->drawLine(p0, p1);
	}
}