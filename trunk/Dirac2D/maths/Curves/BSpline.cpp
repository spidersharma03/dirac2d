/*
 *  BSpline.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 04/10/12.
 *
 */

#include "BSpline.h"

void findOpenKnots(int nControlPoints, int nOrder, float* knotVector)
{
}


void splineBasis(int nControlPoints, int nOrder, float t, float* basisVector)
{
	float x[10];     // Knot Vector
	float N_i1[10];  // First Order basis functions
	
	findOpenKnots(nControlPoints, nOrder, x);
	
	for (int i=0; i<nControlPoints; i++) 
	{
		if( t >= x[i+1] || t < x[i] )
			N_i1[i] = 0.0f;
		else
			N_i1[i] = 1.0f;
	}
	
	for( int o=1; o<nOrder; o++ )
	{
		for( int i=0; i<nControlPoints; i++ )
		{
			float sum = ( t - x[i] )/( x[i+nOrder-1] ) * N_i1[i] + ( x[i+nOrder] - t)/(x[i+nOrder] - x[i+1] ) *  N_i1[i+1];
			basisVector[i] += sum;
			N_i1[i] = sum;
		}
	}
	
}
