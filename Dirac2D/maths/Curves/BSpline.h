/*
 *  BSpline.h
 *  Dirac2D
 *
 *  Created by Prashant on 04/10/12.
 *
 */

#ifndef _B_SPLINE_H_
#define _B_SPLINE_H_


// Number of ControlPolygon vertices -- nControlPoints
// Order  of the BSpline Curve       -- nOrder

// Number of Knots in the Knot vector -- nKnots = nOrder + nControlPoints
// Parameter range in case of Open BSpline curve = ( 0 , nControlPoints - nOrder + 1 )

/*
 in  -- nControlPoints, nOrder
 out -- knotVector containing knots for open BSpline Curves.
 */
void findOpenKnots(int nControlPoints, int nOrder, float* knotVector);


void findPeriodicKnots(int nControlPoints, int nOrder, float* knotVector);


/*
 in  -- nControlPoints, nOrder, parameter value t at which basis functions are needed
 out -- basisVector containing basis functions values at given t for all nControlPoints.
 */
void splineBasis(int nControlPoints, int nOrder, float t, float* knotVector, float* basisVector);


void BSpline(float* controlPoints, int nControlPoints, float* curvePoints, int nCurvePoints, float* knotVector, int nOrder);


#endif
