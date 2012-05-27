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


// Clip Against Edge Plane
static void clip1(Vector2f& refEdge, Vector2f& p, ContactManifold* contactManifold, dint32 refIndex, dfloat capsuleRadius)
{
	Vector2f& cp0 = contactManifold->m_ContactPoints[0].m_Point;
	Vector2f& cp1 = contactManifold->m_ContactPoints[1].m_Point;
	
	Vector2f o1 = cp0 - p;
	dfloat u1 = refEdge.dot(o1);
	o1 = cp1 - p;
	dfloat u2 = refEdge.dot(o1);
	
	dfloat lambda = u1/(u1-u2);
	
	if( u1 + capsuleRadius < 0.0f ) 
	{
		cp0.x = cp0.x + lambda*(cp1.x - cp0.x); 
		cp0.y = cp0.y + lambda*(cp1.y - cp0.y); 
		contactManifold->m_ContactPoints[0].m_ID.edgeIndex.m_Index1 = refIndex;
		contactManifold->m_ContactPoints[0].m_ID.edgeIndex.m_Type1  = ECT_EDGE;
	}
	if( u2 + capsuleRadius < 0.0f ) 
	{
		cp1.x = cp0.x + lambda*(cp1.x - cp0.x); 
		cp1.y = cp0.y + lambda*(cp1.y - cp0.y); 
		contactManifold->m_ContactPoints[1].m_ID.edgeIndex.m_Index1 = refIndex;
		contactManifold->m_ContactPoints[1].m_ID.edgeIndex.m_Type1  = ECT_EDGE;
	}
}

// Finds the Closest points of two line segments.
static void findClosestPoints(Vector2f& p0, Vector2f& p1, Vector2f& p2, Vector2f& p3, Vector2f& outPoint0, Vector2f& outPoint1)
{
	dfloat dx1 = p1.x - p0.x;
	dfloat dx2 = p3.x - p2.x;
	dfloat dy1 = p1.y - p0.y;
	dfloat dy2 = p3.y - p2.y;
	
	dfloat A = dx1 * dx1 + dx2 * dx2;
	dfloat B = - dx1 * dx2 - dy1 * dy2;
	dfloat C = p2.x * dx1 + p2.y * dy1 - p0.x * dx1 - p0.y * dy1;
	dfloat D = B;
	dfloat E = dy1 * dy1 + dy2 * dy2;
	dfloat F = p0.x * dx2 + p0.y * dy2 - p2.x * dx2 - p2.y * dy2;
	
	dfloat Denom = D * B - A * E;
	dfloat u1 = 0.0f;
	dfloat u2 = 0.0f;
	if( fabs(Denom) > EPSILON )
	{
		Denom = 1.0f/Denom;
		u2 = ( D * C - A * F ) * Denom;
		u1 = ( B * F - E * C ) * Denom;
	}
	outPoint0 = p0 + p1 * u1;
	outPoint1 = p2 + p3 * u2;
}

// Finds the Closest point on the Line from a Point.
static inline void findClosestPoint(Vector2f& p0, Vector2f& p1, Vector2f& point, Vector2f& outPoint)
{
	dfloat dx = p1.x - p0.x;
	dfloat dy = p1.y - p0.y;
	dfloat Denom = dx*dx + dy*dy;
	
	dfloat u = 0.0f;
	
	if( fabs(Denom) > EPSILON )
	{
		u = (dx * point.x + dy * point.y - dx * p0.x - dy * p0.y)/Denom;
	}
	if( u < 0.0f ) u = 0.0f;
	if( u > 1.0f ) u = 1.0f;
	
	outPoint = p0 + (p1-p0) * u;
}

// Finds the Closest point on the Line from a Point.
//static void findClosestPoint(Vector2f& p0, Vector2f& p1, Vector2f& point, Vector2f& outPoint)
//{
//	Vector2f d = p1-p0;
//	dfloat dot = (point-p0).dot(d);
//	if( dot < 0.0f )
//	{
//		outPoint = p0;
//		return;
//	}
//	d = -d;
//	dot = (point-p1).dot(d);
//	if( dot < 0.0f )
//	{
//		outPoint = p1;
//		return;
//	}
//	Vector2f n(d.y, -d.x);
//	n.normalize();
//	dot = (point-p0).dot(n);
//	outPoint = point + n * -dot;
//}



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
