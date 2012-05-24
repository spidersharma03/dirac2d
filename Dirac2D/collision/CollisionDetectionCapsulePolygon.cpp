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
	// Transform Capsule to Poly's frame
	Vector2f c = xform2 * capsule->m_Centroid;
	c *= xform1;
	dfloat capsuleHeight = capsule->m_Height;
	dfloat capsuleRadius = capsule->m_Radius;
	Vector2f axis(1.0f, 0.0f);
	Vector2f p0, p1;
    xform2.transformAsVector(axis);
	axis = xform1.getRotationMatrixTransposed() * axis;
	
	p0 = c + axis * capsuleHeight*0.5f;
	p1 = c - axis * capsuleHeight*0.5f;
	
	
	dint32 numVertices = poly->getNumVertices();
	Vector2f* vertices = poly->getVertices();
	
	Vector2f closestPointOnCapsuleAxis;
	Vector2f closestPointOnPolyEdge;
	
	dbool bRes = false;
	for( dint32 v=0; v<numVertices; v++ )
	{
		dint32 i = (v==numVertices-1) ? 0 : v+1; 
		
		//findClosestPoint(vertices[v], vertices[i], p0, closestPointOnPolyEdge);
		findClosestPoints(vertices[v], vertices[i], p0, p1, closestPointOnPolyEdge, closestPointOnCapsuleAxis);
		dfloat d2 = closestPointOnPolyEdge.distanceSquared(closestPointOnCapsuleAxis);
		if( d2 > capsuleRadius*capsuleRadius )
			continue;
		bRes = true;	
		contactManifold->m_NumContacts = 1;
		contactManifold->m_ContactNormal = closestPointOnPolyEdge - closestPointOnCapsuleAxis;
		contactManifold->m_ContactNormal.normalize();
		
		Vector2f closestPointOnCapsule = closestPointOnCapsuleAxis + contactManifold->m_ContactNormal * capsuleRadius;
		
		contactManifold->m_ContactPoints[0].m_Point = (closestPointOnCapsule + closestPointOnPolyEdge) * 0.5f;
		xform1.transformAsPoint(contactManifold->m_ContactPoints[0].m_Point);
		
		dfloat d = (closestPointOnPolyEdge - closestPointOnCapsuleAxis).dot(contactManifold->m_ContactNormal);
		contactManifold->m_ContactPoints[0].m_Depth = -(capsuleRadius - d);
		
		xform1.transformAsVector(contactManifold->m_ContactNormal);

		dAssert(contactManifold->m_ContactPoints[0].m_Depth < 0.0f);
	}
	return bRes;
}

END_NAMESPACE_DIRAC2D
