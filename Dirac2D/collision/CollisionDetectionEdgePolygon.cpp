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
	return true;
}
// Find Whether Edge/Circle intersect. Also find the Contact Points if the Edge/Circle intersect.
dbool intersectEdgeCircle( Edge* edge, Matrix3f& xform1, Circle* circle, Matrix3f& xform2, ContactManifold* contactManifold)
{
	// Transform the Circle into Edge's Frame
	Vector2f c = xform2 * circle->m_Centroid;
	c *= xform1;
	
	// Find Closest Distance between Edge and Circle.
	Vector2f outPoint;
	dfloat u = 0.0f;
	findBaryCentricCoordinate( edge->m_Vertex1, edge->m_Vertex2, c, u );
	
	// Centre of the Circle on the Left of the Edge
	if( u < 0.0f )
	{
		outPoint = edge->m_Vertex1;		
		dfloat D2 = c.distanceSquared(outPoint);
		if( D2 > circle->m_Radius * circle->m_Radius )
		{
			return false;
		}
		if( edge->m_bHasPrev )
		{
			// If this Edge is having a Previous Vertex, then find if the Circle Centre Lies within the Prev Edge.
			Vector2f d = edge->m_Vertex1 - edge->m_PrevVertex;
			dfloat lambda = (c - edge->m_Vertex1 ).dot(d);
			// Return if the Circle Centre lies within the Prev Edge
			if( lambda < 0.0f )
				return false;
		}
	}
	// Centre of the Circle on the Right of the Edge
	else if( u > 1.0f )
	{
		outPoint = edge->m_Vertex2;		
		dfloat D2 = c.distanceSquared(outPoint);
		if( D2 > circle->m_Radius * circle->m_Radius )
		{
			return false;
		}
		if( edge->m_bHasNext )
		{
			// If this Edge is having a Next Vertex, then find if the Circle Centre Lies within the Next Edge.
			Vector2f d = edge->m_Vertex2 - edge->m_NextVertex;
			dfloat lambda = (c - edge->m_Vertex2 ).dot(d);
			if( lambda < 0.0f )
				return false;
		}
	}
	// Centre of the Circle inside the Edge.
	else 
	{
		outPoint = edge->m_Vertex1 + (edge->m_Vertex2 - edge->m_Vertex1 ) * u;
	}

	
	dfloat D2 = c.distanceSquared(outPoint);
	
	if( D2 > circle->m_Radius * circle->m_Radius )
	{
		return false;
	}
	contactManifold->m_NumContacts = 1;
	contactManifold->m_ContactNormal = outPoint - c; // Contact Normal points from Shape2(Circle) to Shape1(Edge)
	dfloat d = contactManifold->m_ContactNormal.length();
	contactManifold->m_ContactNormal /= d;
	
	c += contactManifold->m_ContactNormal * circle->m_Radius;
	contactManifold->m_ContactPoints[0].m_Point = ( c + outPoint ) * 0.5f;
	contactManifold->m_ContactPoints[0].m_Depth = -0.5f * ( circle->m_Radius - d );
	
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
	dfloat u1, u2;
	findBaryCentricCoordinate(edge->m_Vertex1, edge->m_Vertex2, p0, u1);
	findBaryCentricCoordinate(edge->m_Vertex1, edge->m_Vertex2, p1, u2);	
	
	// Accept/Reject Points
	contactManifold->m_NumContacts = 0;
	
	dbool bRes = false;
	Vector2f v1, v2;
	dfloat D2;
	
	// First Closest Point is on the Left of the Edge
	if( u1 < 0.0f )
	{
		outPoint1 = edge->m_Vertex1;
		findClosestPoint(p0, p1, outPoint1, v1);
		D2 = v1.distanceSquared(outPoint1);
		if( D2 < capsuleRadius * capsuleRadius )
		{
			if( edge->m_bHasPrev )
			{
				// If this Edge is having a Previous Vertex, then find if the First Closest Point on Capsule Lies within the Prev Edge.
				Vector2f d = edge->m_Vertex1 - edge->m_PrevVertex;
				dfloat lambda = (v1 - edge->m_Vertex1 ).dot(d);
				// Return if the First Closest Point on Capsule Lies within the Prev Edge
				if( lambda > 0.0f )
					bRes = true;
			}
			else 
				bRes = true;
		}
	}
	// First Closest Point is on the Right of the Edge
	else if( u1 > 1.0f )
	{
		outPoint1 = edge->m_Vertex2;
		findClosestPoint(p0, p1, outPoint1, v1);
		D2 = v1.distanceSquared(outPoint1);
		if( D2 < capsuleRadius * capsuleRadius )
		{
			if( edge->m_bHasNext )
			{
				// If this Edge is having a Next Vertex, then find if the First Closest Point Lies within the Next Edge.
				Vector2f d = edge->m_Vertex2 - edge->m_NextVertex;
				dfloat lambda = (v1 - edge->m_Vertex2 ).dot(d);
				// Return if the First Closest Point on Capsule Lies within the Next Edge
				if( lambda > 0.0f )
					bRes = true;
			}
			else 
				bRes = true; 
		}
	}
	// First Closest Point is in between the Edge
	else 
	{
		outPoint1 = edge->m_Vertex1 + (edge->m_Vertex2 - edge->m_Vertex1 ) * u1;
		findClosestPoint(p0, p1, outPoint1, v1);
		D2 = v1.distanceSquared(outPoint1);
		if( D2 < capsuleRadius * capsuleRadius )
			bRes = true;
	}
	
	if( bRes )
	{
		contactManifold->m_ContactNormal = outPoint1 - v1;
		contactManifold->m_ContactNormal.normalize();
		
		v1 += contactManifold->m_ContactNormal * capsuleRadius;
		
		contactManifold->m_ContactPoints[contactManifold->m_NumContacts].m_Point = (v1+outPoint1)*0.5f;
		contactManifold->m_ContactPoints[contactManifold->m_NumContacts].m_Depth = -0.5f*( capsuleRadius - sqrt(D2) );
		contactManifold->m_NumContacts++;
	}
	
	bRes = false;
	
	// Second Closest Point is on the Left of the Edge
	if( u2 < 0.0f )
	{
		outPoint2 = edge->m_Vertex1;
		findClosestPoint(p0, p1, outPoint2, v2);
		dfloat D2 = v2.distanceSquared(outPoint2);
		if( D2 < capsuleRadius * capsuleRadius )
		{
			if( edge->m_bHasPrev )
			{
				// If this Edge is having a Previous Vertex, then find if the Second Closest Point Lies within the Previous Edge.
				Vector2f d = edge->m_Vertex1 - edge->m_PrevVertex;
				dfloat lambda = (v1 - edge->m_Vertex1 ).dot(d);
				// Return if the Second Closest Point on Capsule Lies within the Prev Edge
				if( lambda > 0.0f )
					bRes = true;
			}
			else 
				bRes = true;
		}	
	}
	// Second Closest Point is on the Right of the Edge
	else if( u2 > 1.0f )
	{
		outPoint2 = edge->m_Vertex2;
		findClosestPoint(p0, p1, outPoint2, v2);
		dfloat D2 = v2.distanceSquared(outPoint2);
		if( D2 < capsuleRadius * capsuleRadius )
		{
			if( edge->m_bHasNext )
			{
				// If this Edge is having a Next Vertex, then find if the Second Closest Point Lies within the Next Edge.
				Vector2f d = edge->m_Vertex2 - edge->m_NextVertex;
				dfloat lambda = (v1 - edge->m_Vertex2 ).dot(d);
				// Return if the Second Closest Point on Capsule Lies within the Next Edge
				if( lambda > 0.0f )
					bRes = true;
			}
			else 
				bRes = true;
		}
	}
	// Second Closest Point is in between the Edge
	else 
	{
		outPoint2 = edge->m_Vertex1 + (edge->m_Vertex2 - edge->m_Vertex1 ) * u2;
		findClosestPoint(p0, p1, outPoint2, v2);
		dfloat D2 = v2.distanceSquared(outPoint2);
		if( D2 < capsuleRadius * capsuleRadius )
			bRes = true;
	}
	
	if( bRes )
	{
		contactManifold->m_ContactNormal = outPoint2 - v2;
		contactManifold->m_ContactNormal.normalize();
		
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