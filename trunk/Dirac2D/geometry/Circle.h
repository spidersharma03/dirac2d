/*
 *  Circle.h
 *  Dirac2D
 *
 *  Created by Prashant on 24/04/12.
 *
 */

#ifndef _CIRCLE_H_
#define _CIRCLE_H_

#include "CollisionShape.h"

BEGIN_NAMESPACE_DIRAC2D

class Circle : public CollisionShape
{
public:
	Circle();
	
	Circle(const Circle& other);
	
	virtual dfloat getArea();
	
	virtual Vector2f getSupportPoint(Vector2f& d);

protected:
	
	virtual void findCentroid();
	
protected:
	dfloat m_Radius;
};

END_NAMESPACE_DIRAC2D

#endif