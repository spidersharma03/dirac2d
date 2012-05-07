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
	Circle(dfloat radius);
	
	Circle(const Circle& other);
		
	virtual Vector2f getSupportPoint(Vector2f& d);

	virtual dbool isPointInside(Point2f& p);
	
	virtual void updateAABB(Matrix3f& xForm);
	
protected:
	
	virtual void findCentroid();
	
protected:
	dfloat m_Radius;
};

END_NAMESPACE_DIRAC2D

#endif