/*
 *  GrahamScanConvexHullGenerator.h
 *  Dirac2D
 *
 *  Created by Prashant on 31/08/12.
 *
 */

#ifndef _GRAHAM_SCAN_CONVEX_HULL_GENERATOR_H_
#define _GRAHAM_SCAN_CONVEX_HULL_GENERATOR_H_

#include "ConvexHullGenerator.h" 

BEGIN_NAMESPACE_DIRAC2D


class GrahamScanConvexHullGenerator : public ConvexHullGenerator
{
public:
	GrahamScanConvexHullGenerator()
	{
	}
	
	virtual ConvexPolygon* createConvexHull( Vector2f* pointSet, dint32 numPoints); 
};

END_NAMESPACE_DIRAC2D

#endif