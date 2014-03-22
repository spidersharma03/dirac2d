/*
 *  ConvexHullGenerator.h
 *  Dirac2D
 *
 *  Created by Prashant on 31/08/12.
 *
 */

#ifndef _CONVEX_HULL_GENERATOR_H_
#define _CONVEX_HULL_GENERATOR_H_

#include "../../definitions.h" 
#include "../../maths/Vector2.h"

BEGIN_NAMESPACE_DIRAC2D

class ConvexPolygon;

/*
	This is the Interface for creating Convexhulls from a point data set. 
	Any class willing to implement the Convex Hull generation should subclass this.
 */

class ConvexHullGenerator
{
public:
	ConvexHullGenerator()
	{
	}
	
	virtual ConvexPolygon* createConvexHull( Vector2f* pointSet, dint32 numPoints) = 0; 
};

END_NAMESPACE_DIRAC2D

#endif

