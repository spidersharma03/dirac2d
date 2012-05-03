/*
 *  Capsule.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 03/05/12.
 *
 */

#include "Capsule.h"

BEGIN_NAMESPACE_DIRAC2D


Capsule::Capsule()
{
}
//
////Box(const Box& other);
//

dfloat Capsule::getArea()
{
	return 0.0f;
}

Vector2f Capsule::getSupportPoint(Vector2f& d)
{
	return Vector2f();
}

dbool Capsule::isPointInside(Point2f& p)
{
	return 0;
}

void Capsule::updateAABB(Matrix3f& xForm)
{
}

void Capsule::findCentroid()
{
}

END_NAMESPACE_DIRAC2D
