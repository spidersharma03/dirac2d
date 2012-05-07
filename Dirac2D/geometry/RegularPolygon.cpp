/*
 *  RegularPolygon.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 03/05/12.
 *
 */


#include "RegularPolygon.h"

BEGIN_NAMESPACE_DIRAC2D


RegularPolygon::RegularPolygon()
{
}
//
////Box(const Box& other);
//


Vector2f RegularPolygon::getSupportPoint(Vector2f& d)
{
	return Vector2f();
}

dbool RegularPolygon::isPointInside(Point2f& p)
{
	return 0;
}

void RegularPolygon::updateAABB(Matrix3f& xForm)
{
}

void RegularPolygon::findCentroid()
{
}

END_NAMESPACE_DIRAC2D