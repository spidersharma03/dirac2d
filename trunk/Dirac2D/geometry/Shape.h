/*
 *  Shape.h
 *  Dirac2D
 *
 *  Created by Prashant on 24/04/12.
 *
 */

#include "../../definitions.h"
#include "../../Settings.h"
#include "../../maths/MathUtil.h"

#ifndef _SHAPE_H_
#define _SHAPE_H_

BEGIN_NAMESPACE_DIRAC2D

class Shape
{
	enum SHAPE_TYPE { EST_CIRCLE, EST_REGULARPOLY, EST_CAPSULE};
public:
	Shape();
	
	Shape(const Shape& other);
	
	virtual dfloat getArea();
	
	
protected:
	
	virtual void findCentroid();
	
protected:
	
	dfloat m_Area;
	Vector2f m_Centroid;
	SHAPE_TYPE m_ShapeType;
};

END_NAMESPACE_DIRAC2D

#endif