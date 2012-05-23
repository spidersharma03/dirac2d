/*
 *  CollisionDetectionCapsulePolygon.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 23/05/12.
 *
 */

#include "CollisionDetectionCapsulePolygon.h"

BEGIN_NAMESPACE_DIRAC2D


// Find Whether two Capsules intersect.
dbool intersectCapsules( Capsule* capsule1, Matrix3f& xform1, Capsule* capsule2, Matrix3f& xform2)
{
	return 0;
}

// Find Whether two Capsules intersect.Also find the Contact Points if the Capsules intersect.
dbool intersectCapsules( Capsule* capsule1, Matrix3f& xform1, Capsule* capsule2, Matrix3f& xform2, ContactManifold* contactManifold)
{
	// Transform Capsule 2 to Capsule1's frame
	Vector2f c1 = xform2 * capsule2->m_Centroid;
	c1 *= xform1;
	Vector2f axis(1.0f,0.0f);
	xform2.transformAsVector(axis);
	Vector2f p0, p1, p2, p3;
	
	return 0;
}


// Find Whether Capsule/Circle intersect. Also find the Contact Points if the Capsule/Circle intersect.
dbool intersectCapsuleCircle( Circle* circle, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2)
{
	return 0;
}

// Find Whether two Circles intersect. Also find the Contact Points if the Circles intersect.
dbool intersectCapsuleCircle( Circle* circle, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2, ContactManifold* contactManifold)
{
	// Transform Circle to Capsule's frame
	Vector2f c = xform1 * circle->m_Centroid;
	c *= xform2;
	dfloat capsuleHeight = capsule->m_Height;
	Vector2f p0( -capsuleHeight*0.5f, 0.0f ), p1( capsuleHeight*0.5f, 0.0f );
	Vector2f outPoint;
	findClosestPoint(p0, p1, c, outPoint);
	
	dfloat r2 = capsule->m_Radius + circle->m_Radius;
	r2 *= r2;
	dfloat d2 = Vector2f::distanceSquared(c, outPoint);
	if( d2 > r2 )
	{
		return false;
	}
	
	
	return true;
}

// Find Whether two Capsule/ConvexPolygons intersect.
dbool intersectCapsulePolygon( RegularPolygon* poly, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2)
{
	return 0;
}

// Find Whether two Capsule/ConvexPolygons intersect.
dbool intersectCapsulePolygon( RegularPolygon* poly, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2, ContactManifold* contactManifold)
{
	return 0;
}

END_NAMESPACE_DIRAC2D
