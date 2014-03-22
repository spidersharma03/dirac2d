/*
 *  BSpline.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 04/10/12.
 *
 */

#include "maths/Curves/BSpline.h"
#include "maths/Vector2.h"

USE_NAMESPACE_DIRAC2D

void findOpenKnots(int nControlPoints, int nOrder, float* knotVector)
{
	for( int i=0; i<nOrder; i++ )
		knotVector[i] = 0.0f;
	for( int i=nOrder+nControlPoints-1; i>nOrder+nControlPoints-4; i-- )
		knotVector[i] = (float)(nControlPoints - nOrder + 1);
	float n = 1.0f;
	for( int i=nOrder; i<nOrder+nControlPoints-3; i++ )
    {
		knotVector[i] = n;	
        n += 1.0f;
    }
}

void findPeriodicKnots(int nControlPoints, int nOrder, float* knotVector)
{
    float n = 0.0f;
	for( int i=0; i<nControlPoints + nOrder; i++ )
    {
		knotVector[i] = n;	
        n += 1.0f;
    }
}


void splineBasis(int nControlPoints, int nOrder, float t, float* knotVector, float* basisVector)
{
	float N_i1[100];  // First Order basis functions

	// initialize the first order basis functions
	for (int i=0; i<nControlPoints + nOrder - 1; i++) 
	{
		if( t >= knotVector[i] && t < knotVector[i+1] )
			N_i1[i] = 1.0f;
		else
			N_i1[i] = 0.0f;
	}
	
	int N = nControlPoints + nOrder - 1;
	// Calculate higher order basis functions
	for( int o=1; o<nOrder; o++ )
	{
		for( int i=0; i<N-1; i++ )
		{
			float a, b;
			if( N_i1[i] != 0.0f )
			{
				a = ( t - knotVector[i] ) * N_i1[i]/( knotVector[i+o] - knotVector[i] );
			}
			else {
				a = 0.0f;
			}
			if( N_i1[i+1] != 0.0f )
			{
				b = ( knotVector[i+o+1] - t) * N_i1[i+1]/(knotVector[i+o+1] - knotVector[i+1] );
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

void BSpline(float* controlPoints, int nControlPoints, float* curvePoints, int nCurvePoints, float* knotVector, int nOrder)
{
	float basis[100]; 

	float t = 0.0f;
	float tStep = (float)(nControlPoints - nOrder + 1)/(nCurvePoints-1);
	for( int i=0; i<2*nCurvePoints; i+=2 )
	{
		splineBasis( nControlPoints, nOrder, t, knotVector, basis);
		
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
