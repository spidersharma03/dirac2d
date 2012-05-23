/*
 *  CollisionDetectionCapsulePolygon.h
 *  Dirac2D
 *
 *  Created by Prashant on 23/05/12.
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
#include "../geometry/Capsule.h"
#include "../dynamics/contacts/Contact.h"

#ifndef _COLLISION_DETECTION_CAPSULE_POLYGON_H_
#define _COLLISION_DETECTION_CAPSULE_POLYGON_H_

BEGIN_NAMESPACE_DIRAC2D

// Finds the Closest points of two line segments.
static void findClosestPoints(Vector2f& p0, Vector2f& p1, Vector2f& p2, Vector2f& p3, Vector2f& outPoint0, Vector2f& outPoint1)
{
	dfloat A = p1.x*p1.x + p1.y*p1.y;
	dfloat B = -( p1.x * p3.x + p1.y * p3.y );
	dfloat C = p1.x * p2.x + p1.y * p2.y - p0.x * p1.x - p0.y * p1.y;
	dfloat D = B;
	dfloat E = p3.x * p3.x + p3.y * p3.y;
	dfloat F = p0.x * p3.x + p0.y * p3.y - p2.x * p3.x - p2.y * p3.y;
	dfloat Denom = D * B - A * E;
	dfloat u1 = 0.0f;
	dfloat u2 = 0.0f;
	if( fabs(Denom) > EPSILON )
	{
		Denom = 1.0f/Denom;
		u1 = ( D * C - A * F ) * Denom;
		u2 = ( E * C - B * F ) * Denom;
	}
	outPoint0 = p0 + p1 * u1;
	outPoint1 = p2 + p3 * u2;
}

// Finds the Closest point on the Line from a Point.
static void findClosestPoint(Vector2f& p0, Vector2f& p1, Vector2f& point, Vector2f& outPoint)
{
	Vector2f d = p1-p0;
	dfloat dot = (point-p0).dot(d);
	if( dot < 0.0f )
	{
		outPoint = p0;
		return;
	}
	d = -d;
	dot = (point-p1).dot(d);
	if( dot < 0.0f )
	{
		outPoint = p1;
		return;
	}
	Vector2f n(d.y, -d.x);
	n.normalize();
	dot = (point-p0).dot(n);
	outPoint = point + n * -dot;
}

// Find Whether two Capsules intersect.
dbool intersectCapsules( Capsule* capsule1, Matrix3f& xform1, Capsule* capsule2, Matrix3f& xform2);

// Find Whether two Capsules intersect.Also find the Contact Points if the Circles intersect.
dbool intersectCapsules( Capsule* capsule1, Matrix3f& xform1, Capsule* capsule2, Matrix3f& xform2, ContactManifold* contactManifold);



// Find Whether Capsule/Circle intersect. Also find the Contact Points if the Circles intersect.
dbool intersectCapsuleCircle( Circle* circle, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2);

// Find Whether two Circles intersect. Also find the Contact Points if the Circles intersect.
dbool intersectCapsuleCircle( Circle* circle, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2, ContactManifold* contactManifold);


// Find Whether two Capsule/ConvexPolygons intersect.
dbool intersectCapsulePolygon( RegularPolygon* poly, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2);

// Find Whether two Capsule/ConvexPolygons intersect.
dbool intersectCapsulePolygon( RegularPolygon* poly, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2, ContactManifold* contactManifold);

END_NAMESPACE_DIRAC2D

#endif
