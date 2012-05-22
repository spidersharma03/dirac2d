/*
 *  CollisionDetectionCirclePolygon.h
 *  Dirac2D
 *
 *  Created by Prashant on 22/05/12.
 *
 */
#include <math.h>
#include "../common.h"
#include "../definitions.h"
#include "../Settings.h"
#include "../maths/MathUtil.h"
#include "../geometry/CollisionShape.h"
#include "../geometry/RegularPolygon.h"
#include "../geometry/Circle.h"
#include "../dynamics/contacts/Contact.h"

#ifndef _COLLISION_DETECTION_POLYGON_CIRCLE_H_
#define _COLLISION_DETECTION_POLYGON_CIRCLE_H_

BEGIN_NAMESPACE_DIRAC2D


// Find Whether two Circles intersect.
dbool intersectCircles( Circle* circle1, Matrix3f& xform1, Circle* circle2, Matrix3f& xform2);

// Find Whether two Circles intersect. Also find the Contact Points if the Circles intersect.
dbool intersectCircles( Circle* circle1, Matrix3f& xform1, Circle* circle2, Matrix3f& xform2, ContactManifold* contactManifold);


// Find Whether two Circle/ConvexPolygons intersect.
dbool intersectCirclePolygon( Circle* circle, Matrix3f& xform1, RegularPolygon* poly, Matrix3f& xform2);

// Find Whether two Circle/ConvexPolygons intersect. Also find the Contact Points if the Polygons intersect.
dbool intersectCirclePolygon( Circle* circle, Matrix3f& xform1, RegularPolygon* poly, Matrix3f& xform2, ContactManifold* contactManifold);


END_NAMESPACE_DIRAC2D

#endif