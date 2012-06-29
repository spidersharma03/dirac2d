/*
 *  CollisionDtectionEdge.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 28/05/12.
 *
 */

#include "CollisionDetectionEdgePolygon.h"
#include "PolygonHelper.h"

#include <stdio.h>

BEGIN_NAMESPACE_DIRAC2D

// Find Whether Edges intersect.
dbool intersectEdgeEdge( Edge* edge1, Matrix3f& xform1, Edge* edge2, Matrix3f& xform2)
{
	return false;
}

// Find Whether Edges intersect. Also find the Contact Points if the Edges intersect.
dbool intersectEdgeEdge( Edge* edge1, Matrix3f& xform1, Edge* edge2, Matrix3f& xform2, ContactManifold* contactManifold)
{
	Vector2f e1_v1 = xform1 * edge1->m_Vertex1;
	Vector2f e1_v2 = xform1 * edge1->m_Vertex2;
	Vector2f e2_v1 = xform2 * edge2->m_Vertex1;
	Vector2f e2_v2 = xform2 * edge2->m_Vertex2;

	Vector2f outPoint1, outPoint2; // Closest Points on Edge 1
	findClosestPoint(e1_v1, e1_v2, e2_v1, outPoint1);
	findClosestPoint(e1_v1, e1_v2, e2_v2, outPoint2);

	Vector2f v1, v2; // Closest Points on Edge 2
	findClosestPoint(e2_v1, e2_v2, outPoint1, v1);
	findClosestPoint(e2_v1, e2_v2, outPoint2, v2);
	
	dfloat D1 = v1.distanceSquared(outPoint1);
	dfloat D2 = v2.distanceSquared(outPoint2);
	
	contactManifold->m_NumContacts = 0;

	// Contact Normal Points from Shape2( Edge2 ) to Shape1( Edge1 )
	if( D1 < EDGE_THICKNESS * EDGE_THICKNESS )
	{
		contactManifold->m_ContactNormal = outPoint1 - v1;
		contactManifold->m_ContactNormal.normalize();
		v1 += contactManifold->m_ContactNormal * EDGE_THICKNESS;
		outPoint1 += contactManifold->m_ContactNormal * -EDGE_THICKNESS;

		contactManifold->m_ContactPoints[contactManifold->m_NumContacts].m_Point = (v1 + outPoint1 ) * 0.5f;
		contactManifold->m_ContactPoints[contactManifold->m_NumContacts].m_Depth = 0.5f * (sqrt(D1) - EDGE_THICKNESS);
		contactManifold->m_NumContacts++;
	}
	if( D2 < EDGE_THICKNESS * EDGE_THICKNESS )
	{
		contactManifold->m_ContactNormal = outPoint2 - v2; 
		contactManifold->m_ContactNormal.normalize();
		v2 += contactManifold->m_ContactNormal * EDGE_THICKNESS;
		outPoint2 += contactManifold->m_ContactNormal * -EDGE_THICKNESS;

		contactManifold->m_ContactPoints[contactManifold->m_NumContacts].m_Point = (v2 + outPoint2 ) * 0.5f;
		contactManifold->m_ContactPoints[contactManifold->m_NumContacts].m_Depth = 0.5f * (sqrt(D2) - EDGE_THICKNESS);
		contactManifold->m_NumContacts++;
	}
	return contactManifold->m_NumContacts > 0;
}

// Find Whether Edge/ConvexPolygons intersect.
dbool intersectEdgePolygon( Edge* edge, Matrix3f& xform1, ConvexPolygon* poly, Matrix3f& xform2)
{
	return false;
}

// Find Whether Edge/ConvexPolygons intersect. Also find the Contact Points if the Edge/ConvexPolygons intersect.
dbool intersectEdgePolygon( Edge* edge, Matrix3f& xform1, ConvexPolygon* poly, Matrix3f& xform2, ContactManifold* contactManifold)
{
	// Transform Edge to Poly's Frame.
	Vector2f v1 = xform1 * edge->m_Vertex1;
	v1 *= xform2;
	Vector2f v2 = xform1 * edge->m_Vertex2;
	v2 *= xform2;

	dint32 numVertices = poly->getNumVertices();
	Vector2f* polyVertices = poly->getVertices();
	Vector2f* polyNormals  = poly->getNormals();
	
	Vector2f edgeDir = v2 - v1;
	edgeDir.normalize();
	Vector2f edgeNormal(-edgeDir.y, edgeDir.x);

	// Find Seperating Axis from poly's seperating axis(normals) list
	dfloat minDistance = 100000.0f;
	Vector2f collisionNormal;
		
	for( dint32 i=0; i<numVertices; i++ )
	{
		Vector2f& normal = polyNormals[i];
		dfloat min1 = 100000.0f, min2 = 100000.0f, max1 = -100000.0f, max2 = -100000.0f; 
		dint32 minIndex = 0, maxIndex = 0;
		
				
		PROJECT_POLYGON( poly, numVertices, normal, minIndex , maxIndex );
		Vector2f p1 = polyVertices[minIndex];
		Vector2f p2 = polyVertices[maxIndex];
		min1 = p1.dot(normal);
		max1 = p2.dot(normal);
		
		min2 = v1.dot(normal);
		max2 = v2.dot(normal);
		if( min2 > max2 )
			SWAP(min2, max2);
		
		dfloat distance; //Calculate the distance between the two intervals
		distance = min1 < min2 ? min2-max1 : min1-max2;
		
		if( distance > 0.0f ) //If the intervals don't overlap, return, since there is no collision
			return false;
		
		if( fabs( distance ) < minDistance )
		{
			/*if( edge->m_bHasPrev )
			{
				Vector2f prevEdge = ( edge->m_Vertex1 - edge->m_PrevVertex);
				xform1.transformAsVector(prevEdge);
				prevEdge = xform2.getRotationMatrixTransposed() * prevEdge;
				prevEdge.normalize();
				Vector2f prevNormal( -prevEdge.y, prevEdge.x);
				if( fabs(normal.dot(prevNormal)) < 0.9f )
					continue;
			}
			if( edge->m_bHasNext )
			{
				Vector2f nextEdge = ( edge->m_Vertex2 - edge->m_NextVertex);
				xform1.transformAsVector(nextEdge);
				nextEdge = xform2.getRotationMatrixTransposed() * nextEdge;
				nextEdge.normalize();
				Vector2f nextNormal( -nextEdge.y, nextEdge.x);
				if( fabs(normal.dot(nextNormal)) < 0.9f )
					continue;
			}*/
			minDistance = fabs( distance );
			collisionNormal = normal; //Save collision information for later
		}
	}
	// Find Seperating Axis from Edge's Normal
	dfloat min1 = 100000.0f, min2 = 100000.0f, max1 = -100000.0f, max2 = -100000.0f; 
	dint32 minIndex = 0; dint32 maxIndex = 0;
	PROJECT_POLYGON( poly, numVertices, edgeNormal, minIndex , maxIndex );
	Vector2f p1 = polyVertices[minIndex];
	Vector2f p2 = polyVertices[maxIndex];
	min1 = p1.dot(edgeNormal);
	max1 = p2.dot(edgeNormal);
	
	min2 = max2 = v1.dot(edgeNormal);
	
	dfloat distance; //Calculate the distance between the two intervals
	distance = min1 < min2 ? min2-max1 : min1-max2;
	
	// Check for Containment of Projection intervals
	if( (min2 > min1 && min2 < max1) || (min2 > max1 && min2 < min1) )
	{
		dfloat d1 = fabs(min1 - min2);
		dfloat d2 = fabs(max1 - min2);
		distance = d1 < d2 ? -d1 : -d2;
	}
	
	if( distance > 0.0f ) //If the intervals don't overlap, return, since there is no collision
		return false;
	
	dfloat absDist = fabs( distance );
	if( absDist < minDistance )
	{
		minDistance = fabs( distance );
		collisionNormal = edgeNormal; 
	}
	
	dint32 vertexIndex1;
	dint32 vertexIndex2;
	
	// Make sure that the Collision Normal points from Shape2(Polygon) to Shape1(Edge)
	Vector2f c1c2 = v1 - poly->m_Centroid;

	if( c1c2.dot(collisionNormal) < 0.0f )
		collisionNormal = -collisionNormal;
	
	findCandidateEdge(poly, collisionNormal, vertexIndex1, vertexIndex2);
	
	Vector2f refEdge = polyVertices[vertexIndex2] - polyVertices[vertexIndex1];
	refEdge.normalize();
	Vector2f incEdge = edgeDir;
	
	dbool bFlip = false;
	
	if( absDist < minDistance )
	{
		Vector2f temp = incEdge;
		incEdge = refEdge;
		refEdge = temp;
		
		bFlip = true;
	}
	
	contactManifold->m_NumContacts = 2;
	dfloat dot1, dot2;
	
	if( bFlip )
	{
		contactManifold->m_ContactPoints[0].m_Point =  polyVertices[vertexIndex1];
		contactManifold->m_ContactPoints[1].m_Point = polyVertices[vertexIndex2];
		
		// Clip Against first Edge Plane of Reference Edge
		dint32 refIndex = ( vertexIndex1==0 ? numVertices-1 : vertexIndex1-1 );
		clip(refEdge, v1, contactManifold, refIndex);
		// Clip Against Second Edge Plane of Reference Edge
		refIndex = 0;
		Vector2f refNegEdge = -refEdge;
		clip(refNegEdge, v2, contactManifold, refIndex);
		
		// Clip the ContactPoints against the Reference Edge Normal.
		Vector2f refNormal( refEdge.y, -refEdge.x );
		// Flip the Normal
		if( collisionNormal.dot(refNormal) > 0.0f )
			refNormal = -refNormal;
		
		ContactPoint* contactPoints = contactManifold->m_ContactPoints;
		
		Vector2f T = contactPoints[0].m_Point - v1;
		dot1 = refNormal.dot(T);
		
		T = contactPoints[1].m_Point - v2;
		dot2 = refNormal.dot(T);
		
		contactPoints[0].m_Depth = dot1;
		contactPoints[1].m_Depth = dot2;
	}
	else 
	{
		contactManifold->m_ContactPoints[0].m_Point = v1;
		contactManifold->m_ContactPoints[1].m_Point = v2;
		
		// Clip Against first Edge Plane of Reference Edge
		dint32 refIndex = 0;
		clip(refEdge, polyVertices[vertexIndex1], contactManifold, refIndex);
		// Clip Against Second Edge Plane of Reference Edge
		refIndex = 0;
		Vector2f refNegEdge = -refEdge;
		clip(refNegEdge, polyVertices[vertexIndex2], contactManifold, refIndex);
		
		// Clip the ContactPoints against the Reference Edge Normal.
		Vector2f refNormal( -refEdge.y, refEdge.x );
		// Flip the Normal
		if( collisionNormal.dot(refNormal) < 0.0f )
			refNormal = -refNormal;
		
		
		ContactPoint* contactPoints = contactManifold->m_ContactPoints;
		
		Vector2f T = contactPoints[0].m_Point - polyVertices[vertexIndex1];
		dot1 = refNormal.dot(T);
		
		T = contactPoints[1].m_Point - polyVertices[vertexIndex2];
		dot2 = refNormal.dot(T);
		
		contactPoints[0].m_Depth = dot1;
		contactPoints[1].m_Depth = dot2;
	}
	
	if( dot1 > 0.0f )
	{
		contactManifold->m_ContactPoints[0].m_Point = contactManifold->m_ContactPoints[1].m_Point;
		contactManifold->m_ContactPoints[0].m_Depth = dot2;
		contactManifold->m_NumContacts = 1;
	}
	
	if( dot2 > 0.0f )
	{
		contactManifold->m_NumContacts = 1;
	}
	
	contactManifold->m_ContactNormal = collisionNormal;
	
	xform2.transformAsPoint(contactManifold->m_ContactPoints[0].m_Point);
	xform2.transformAsPoint(contactManifold->m_ContactPoints[1].m_Point);	
	xform2.transformAsVector(contactManifold->m_ContactNormal);
	if( contactManifold->m_bFlipShapes )
		contactManifold->m_ContactNormal = -contactManifold->m_ContactNormal;
	
	return true;
	
//	contactManifold->m_ContactPoints[0].m_ID.edgeIndex.m_Index1 = (p3==0 ? numVertices1-1 : p3-1);
//	contactManifold->m_ContactPoints[0].m_ID.edgeIndex.m_Index2 = p3;
//	contactManifold->m_ContactPoints[0].m_ID.edgeIndex.m_Type1 = ECT_VERTEX;
//	contactManifold->m_ContactPoints[0].m_ID.edgeIndex.m_Type2 = ECT_VERTEX;
//	
//	contactManifold->m_ContactPoints[1].m_ID.edgeIndex.m_Index1 = p4;
//	contactManifold->m_ContactPoints[1].m_ID.edgeIndex.m_Index2 = (p4==0 ? numVertices1-1 : p4-1);
//	contactManifold->m_ContactPoints[1].m_ID.edgeIndex.m_Type1 = ECT_VERTEX;
//	contactManifold->m_ContactPoints[1].m_ID.edgeIndex.m_Type2 = ECT_VERTEX;
//	
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
	if( contactManifold->m_bFlipShapes )
		contactManifold->m_ContactNormal = -contactManifold->m_ContactNormal;
	
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
		D2 = v2.distanceSquared(outPoint2);
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
		D2 = v2.distanceSquared(outPoint2);
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
		D2 = v2.distanceSquared(outPoint2);
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
	if( contactManifold->m_bFlipShapes )
		contactManifold->m_ContactNormal = -contactManifold->m_ContactNormal;
	
	return contactManifold->m_NumContacts > 0;
}

END_NAMESPACE_DIRAC2D