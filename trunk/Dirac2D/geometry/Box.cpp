/*
 *  Box.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 03/05/12.
 *
 */
#include "Box.h"

BEGIN_NAMESPACE_DIRAC2D

		
Box::Box()
{
}
//
////Box(const Box& other);
//

dfloat Box::getArea()
{
	return 0.0f;
}

Vector2f Box::getSupportPoint(Vector2f& d)
{
	return Vector2f();
}

dbool Box::isPointInside(Point2f& p)
{
	return 0;
}

void Box::updateAABB(Matrix3f& xForm)
{
}

void Box::findCentroid()
{
}

END_NAMESPACE_DIRAC2D
