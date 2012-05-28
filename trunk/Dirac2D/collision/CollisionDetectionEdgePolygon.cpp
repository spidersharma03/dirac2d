/*
 *  CollisionDtectionEdge.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 28/05/12.
 *
 */

#include "CollisionDetectionEdgePolygon.h"


BEGIN_NAMESPACE_DIRAC2D

// Find Whether Edge/ConvexPolygons intersect.
dbool intersectEdgePolygon( Edge* edge, Matrix3f& xform1, RegularPolygon* poly, Matrix3f& xform2)
{
	return false;
}

// Find Whether Edge/ConvexPolygons intersect. Also find the Contact Points if the Edge/ConvexPolygons intersect.
dbool intersectEdgePolygon( Edge* edge, Matrix3f& xform1, RegularPolygon* poly, Matrix3f& xform2, ContactManifold* contactManifold)
{
	return false;
}


// Find Whether Edge/Circle intersect.
dbool intersectEdgeCircle( Edge* edge, Matrix3f& xform1, Circle* circle, Matrix3f& xform2)
{
	return false;
}
// Find Whether Edge/Circle intersect. Also find the Contact Points if the Edge/Circle intersect.
dbool intersectEdgeCircle( Edge* edge, Matrix3f& xform1, Circle* circle, Matrix3f& xform2, ContactManifold* contactManifold)
{
	// Transform the Circle into Edge's Frame
	Vector2f c = xform2 * circle->m_Centroid;
	c *= xform1;
	
	
	// Find Closest Distance between Edge and Circle.
	Vector2f outPoint;
	findClosestPoint(edge->m_Vertex1, edge->m_Vertex2, c, outPoint);
	dfloat D2 = c.distanceSquared(outPoint);
	
	if( D2 > circle->m_Radius * circle->m_Radius )
	{
		return false;
	}
	
	contactManifold->m_NumContacts = 1;
	contactManifold->m_ContactNormal = outPoint - c; // Contact Normal points from Shape2(Circle) to Shape1(Edge)
	contactManifold->m_ContactNormal.normalize();

	c += contactManifold->m_ContactNormal * circle->m_Radius;
	contactManifold->m_ContactPoints[0].m_Point = ( c + outPoint ) * 0.5f;
	contactManifold->m_ContactPoints[0].m_Depth = -0.5f * ( circle->m_Radius - sqrt(D2) );
	
	// Transform to World Space
	xform1.transformAsPoint(contactManifold->m_ContactPoints[0].m_Point);
	xform1.transformAsVector(contactManifold->m_ContactNormal);
	
	return true;
}


// Find Whether Edge/Capsule intersect.
dbool intersectEdgeCapsule( Edge* edge, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2)
{
	return false;
}

// Find Whether Edge/Capsule intersect. Also find the Contact Points if the Edge/Circle intersect.
dbool intersectEdgeCapsule( Edge* edge, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2, ContactManifold* contactManifold)
{
	// Transform Capsule to Edge's frame
	Vector2f c = xform2 * capsule->m_Centroid;
	c *= xform1;
	dfloat capsuleHeight = capsule->m_Height;
	dfloat capsuleRadius = capsule->m_Radius;
	
	Vector2f axis(1.0f, 0.0f);
	xform2.transformAsVector(axis);
	axis = xform1.getRotationMatrixTransposed() * axis;
	
	Vector2f p0, p1; // Capsule
	p0 = c + axis * ( capsuleHeight*0.5f );
	p1 = c - axis * ( capsuleHeight*0.5f );
	
	Vector2f outPoint1, outPoint2;
	// Find Closest Points on Edge.
	findClosestPoint(edge->m_Vertex1, edge->m_Vertex2, p0, outPoint1);
	findClosestPoint(edge->m_Vertex1, edge->m_Vertex2, p1, outPoint2);
	
	// Accept/Reject Points
	contactManifold->m_NumContacts = 0;
	
	Vector2f v1, v2;
	findClosestPoint(p0, p1, outPoint1, v1);
	dfloat D1 = outPoint1.distanceSquared(v1);
	findClosestPoint(p0, p1, outPoint2, v2);
	dfloat D2 = outPoint2.distanceSquared(v2);
	
	dfloat R = capsuleRadius;
	R *= R; 
	
	// Contact Normal Points from Shape2(Capsule) to Shape1(Edge)
	if( D1 < D2 )
	{
		contactManifold->m_ContactNormal = outPoint1 - v1;
	}
	else 
	{
		contactManifold->m_ContactNormal = outPoint2 - v2;
	}
	contactManifold->m_ContactNormal.normalize();
	
	
	if( D1 < R )
	{
		v1 += contactManifold->m_ContactNormal * capsuleRadius;
		
		contactManifold->m_ContactPoints[contactManifold->m_NumContacts].m_Point = (v1+outPoint1)*0.5f;
		contactManifold->m_ContactPoints[contactManifold->m_NumContacts].m_Depth = -0.5f*( capsuleRadius - sqrt(D1) );
		contactManifold->m_NumContacts++;
	}
	
	if( D2 < R )
	{
		v2 += contactManifold->m_ContactNormal * capsuleRadius;
		
		contactManifold->m_ContactPoints[contactManifold->m_NumContacts].m_Point = (v2+outPoint2)*0.5f;
		contactManifold->m_ContactPoints[contactManifold->m_NumContacts].m_Depth = -0.5f*( capsuleRadius - sqrt(D2) );
		contactManifold->m_NumContacts++;
	}
	
	ContactPoint* contactPoints = contactManifold->m_ContactPoints;
	
	xform1.transformAsPoint(contactPoints[0].m_Point);
	xform1.transformAsPoint(contactPoints[1].m_Point);
	xform1.transformAsVector(contactManifold->m_ContactNormal);
	
	return contactManifold->m_NumContacts > 0;
}

END_NAMESPACE_DIRAC2D