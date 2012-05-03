/*
 *  Circle.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 03/05/12.
 *
 */


#include "Circle.h"

BEGIN_NAMESPACE_DIRAC2D


Circle::Circle()
{
}
//
////Box(const Box& other);
//

dfloat Circle::getArea()
{
	return 0.0f;
}

Vector2f Circle::getSupportPoint(Vector2f& d)
{
	return Vector2f();
}

dbool Circle::isPointInside(Point2f& p)
{
	return 0;
}

void Circle::updateAABB(Matrix3f& xForm)
{
}

void Circle::findCentroid()
{
}

END_NAMESPACE_DIRAC2D