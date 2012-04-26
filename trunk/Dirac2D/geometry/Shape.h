/*
 *  Shape.h
 *  Dirac2D
 *
 *  Created by Prashant on 24/04/12.
 *
 */

#include "../definitions.h"
#include "../Settings.h"
#include "../maths/MathUtil.h"

#ifndef _SHAPE_H_
#define _SHAPE_H_

BEGIN_NAMESPACE_DIRAC2D

class Shape
{
	enum SHAPE_TYPE { EST_CIRCLE, EST_BOX, EST_REGULARPOLY, EST_CAPSULE };
public:
	Shape();
	
	Shape(const Shape& other);
	
	virtual dfloat getArea();
	
	// For GJK/EPA 
	virtual Vector2f getSupportPoint(Vector2f& d);
	
	Vector2f m_Centroid;

protected:
	
	virtual void findCentroid();
	
protected:
	
	dfloat m_Area;
	SHAPE_TYPE m_ShapeType;
};

END_NAMESPACE_DIRAC2D

#endif