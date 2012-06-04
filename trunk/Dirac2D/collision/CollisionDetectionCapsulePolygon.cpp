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
	// Transform Capsule2 to Capsule1's frame
	Vector2f c = xform2 * capsule2->m_Centroid;
	c *= xform1;
	dfloat capsuleHeight1 = capsule1->m_Height;
	dfloat capsuleRadius1 = capsule1->m_Radius;
	dfloat capsuleHeight2 = capsule2->m_Height;
	dfloat capsuleRadius2 = capsule2->m_Radius;
	Vector2f axis(1.0f, 0.0f);
	xform2.transformAsVector(axis);
	axis = xform1.getRotationMatrixTransposed() * axis;
		
	Vector2f p0(-capsuleHeight1*0.5f, 0.0f), p1(capsuleHeight1*0.5f, 0.0f); // Capsule 1
	Vector2f p2, p3; // Capsule 2
	p2 = c + axis * ( capsuleHeight2*0.5f );
	p3 = c - axis * ( capsuleHeight2*0.5f );
	
	Vector2f outPoint1, outPoint2;
	// Find Closest Points on Capsule 1.
	findClosestPoint(p0, p1, p2, outPoint1);
	findClosestPoint(p0, p1, p3, outPoint2);

	// Accept/Reject Points
	contactManifold->m_NumContacts = 0;
	
	dfloat D1 = outPoint1.distanceSquared(p2);
	dfloat D2 = outPoint2.distanceSquared(p3);
	
	dfloat R = capsuleRadius1 + capsuleRadius2;
	R *= R; 
	
	// Collision Normal Points from Shape2(Capsule2) to Shape1(Capsule1).
	if( D1 < D2 )
	{
		contactManifold->m_ContactNormal = outPoint1 - p2;
	}
	else 
	{
		contactManifold->m_ContactNormal = outPoint2 - p3;
	}
	
	contactManifold->m_ContactNormal.normalize();

	
	if( D1 < R )
	{
		p2		  += contactManifold->m_ContactNormal * -capsule1->m_Radius;
		outPoint1 += contactManifold->m_ContactNormal * capsule2->m_Radius;
		
		contactManifold->m_ContactPoints[contactManifold->m_NumContacts].m_Point = (p2+outPoint1)*0.5f;
		contactManifold->m_ContactPoints[contactManifold->m_NumContacts].m_Depth = -0.5f*( capsuleRadius1 + capsuleRadius2 - sqrt(D1) );
		contactManifold->m_NumContacts++;
	}
	
	if( D2 < R )
	{
		p3		  += contactManifold->m_ContactNormal * -capsule1->m_Radius;
		outPoint2 += contactManifold->m_ContactNormal * capsule2->m_Radius;
		
		contactManifold->m_ContactPoints[contactManifold->m_NumContacts].m_Point = (p3+outPoint2)*0.5f;
		contactManifold->m_ContactPoints[contactManifold->m_NumContacts].m_Depth = -0.5f*( capsuleRadius1 + capsuleRadius2 - sqrt(D2) );
		contactManifold->m_NumContacts++;
	}
	
	ContactPoint* contactPoints = contactManifold->m_ContactPoints;
	
	xform1.transformAsPoint(contactPoints[0].m_Point);
	xform1.transformAsPoint(contactPoints[1].m_Point);
	xform1.transformAsVector(contactManifold->m_ContactNormal);
	
	return contactManifold->m_NumContacts > 0;
}


// Find Whether Capsule/Circle intersect. Also find the Contact Points if the Capsule/Circle intersect.
dbool intersectCapsuleCircle( Circle* circle, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2)
{
	return 0;
}

// Find Whether two Capsule/Circle intersect. Also find the Contact Points if the Capsule/Circle intersect.
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
	dfloat D2 = Vector2f::distanceSquared(c, outPoint);
	if( D2 > r2 )
	{
		return false;
	}
	contactManifold->m_NumContacts = 1;
	contactManifold->m_ContactNormal = c - outPoint; // Normal Points from Shape2(Capsule) to Shape1( Circle)
	contactManifold->m_ContactNormal.normalize();

	c += contactManifold->m_ContactNormal * -circle->m_Radius;
	outPoint += contactManifold->m_ContactNormal * capsule->m_Radius;
	
	contactManifold->m_ContactPoints[0].m_Point = ( c + outPoint ) * 0.5f;
	contactManifold->m_ContactPoints[0].m_Depth = -0.5f * ( capsule->m_Radius + circle->m_Radius - sqrt(D2) ); 
	
	xform2.transformAsPoint(contactManifold->m_ContactPoints[0].m_Point);
	xform2.transformAsVector(contactManifold->m_ContactNormal);
	
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
	
	p0 = c + axis * ( capsuleHeight*0.5f );
	p1 = c - axis * ( capsuleHeight*0.5f );
	
	dint32 numVertices = poly->m_NumVertices;
	Vector2f* vertices = poly->m_Vertices;
	
	Vector2f collisionNormal = axis;
	
	Vector2f v1;
	Vector2f v2;
	Vector2f outPoint;
	
	dint32 edgeIndex = -1;
	dfloat distance = 10000.0f;
	
	for( dint32 v=0; v<numVertices; v++ )
	{
		dint32 i = v+1;
		if( v == numVertices-1 )
			i = 0;
		
		findClosestPoint(vertices[v], vertices[i], p0, outPoint);
		dfloat d = outPoint.distanceSquared(p0);
		if( distance > d )
		{
			distance = d;
			edgeIndex = v;
		}
		findClosestPoint(vertices[v], vertices[i], p1, outPoint);
		d = outPoint.distanceSquared(p1);
		if( distance > d )
		{
			distance = d;
			edgeIndex = v;
		}
		findClosestPoint(p0, p1, vertices[v], outPoint);
		d = outPoint.distanceSquared(vertices[v]);
		if( distance > d )
		{
			distance = d;
			edgeIndex = v;
		}
		findClosestPoint(p0, p1, vertices[i], outPoint);
		d = outPoint.distanceSquared(vertices[i]);
		if( distance > d )
		{
			distance = d;
			edgeIndex = v;
		}
	}
	
	if( distance > capsuleRadius * capsuleRadius )
		return false;
	
	dint32 vertexIndex1 = edgeIndex;
	dint32 vertexIndex2 = edgeIndex == numVertices-1 ? 0 : edgeIndex+1;
	
	Vector2f edgeVertex1 = vertices[vertexIndex1];
	Vector2f edgeVertex2 = vertices[vertexIndex2];
	
	// Find Closest Point on the Poly Edge.
	findClosestPoint(edgeVertex1, edgeVertex2, p0, v1);
	findClosestPoint(edgeVertex1, edgeVertex2, p1, v2);

	contactManifold->m_NumContacts = 0;
	
	// Accept/Reject Above Points
	// Find First Closest Point on Capsule
	findClosestPoint(p0, p1, v1, outPoint);

	dfloat D1 = outPoint.distanceSquared(v1);
	if(  D1 < capsuleRadius * capsuleRadius )
	{
		contactManifold->m_ContactNormal = v1 - outPoint;
		contactManifold->m_ContactNormal.normalize();
		contactManifold->m_ContactPoints[contactManifold->m_NumContacts].m_Point = v1;
		contactManifold->m_ContactPoints[contactManifold->m_NumContacts].m_Depth = -( capsuleRadius - sqrt(D1) );
		contactManifold->m_NumContacts++;
	}
	
	// Find Second Closest Point on Capsule
	findClosestPoint(p0, p1, v2, outPoint);
	dfloat D2 = outPoint.distanceSquared(v2);
	if( D2 < capsuleRadius * capsuleRadius )
	{
		contactManifold->m_ContactNormal = v2 - outPoint;
		contactManifold->m_ContactNormal.normalize();
		contactManifold->m_ContactPoints[contactManifold->m_NumContacts].m_Point = v2;
		contactManifold->m_ContactPoints[contactManifold->m_NumContacts].m_Depth = -( capsuleRadius - sqrt(D2) );
		contactManifold->m_NumContacts++;
	}
	
	ContactPoint* contactPoints = contactManifold->m_ContactPoints;
	
	xform1.transformAsPoint(contactPoints[0].m_Point);
	xform1.transformAsPoint(contactPoints[1].m_Point);
	xform1.transformAsVector(contactManifold->m_ContactNormal);
		
	return contactManifold->m_NumContacts > 0;
}




//{
//	// Transform Capsule to Poly's frame
//	Vector2f c = xform2 * capsule->m_Centroid;
//	c *= xform1;
//	dfloat capsuleHeight = capsule->m_Height;
//	dfloat capsuleRadius = capsule->m_Radius;
//	Vector2f axis(1.0f, 0.0f);
//	Vector2f p0, p1;
//    xform2.transformAsVector(axis);
//	axis = xform1.getRotationMatrixTransposed() * axis;
//	
//	p0 = c + axis * ( capsuleHeight*0.5f );
//	p1 = c - axis * ( capsuleHeight*0.5f );
//	
//	dint32 numVertices = poly->m_NumVertices;
//	Vector2f* vertices = poly->m_Vertices;
//	Vector2f* normals  = poly->m_Normals;
//	
//	Vector2f closestPointOnCapsuleAxis;
//	Vector2f closestPointOnPolyEdge;
//	
//	Vector2f axisPerp(-axis.y, axis.x);
//	Vector2f collisionNormal = axis;
//	
//	dint32 minIndex = 0;
//	dint32 maxIndex = 0;
//	dfloat min1 = 10000.0f, min2 = 10000.0f, max1 = -10000.0f, max2 = -10000.0f; 
//	
//	// Project Poly on the Perpendicular Axis of the Capsule.
//	PROJECT_POLYGON( poly, numVertices, collisionNormal, minIndex, maxIndex );
//	Vector2f v1 = vertices[minIndex];
//	Vector2f v2 = vertices[maxIndex];
//	min1 = v1.dot(collisionNormal);
//	max1 = v2.dot(collisionNormal);
//	
//	dfloat proj = c.dot(collisionNormal);
//	min2 = proj - ( capsuleRadius + capsuleHeight * 0.5f );
//	max2 = proj + ( capsuleRadius + capsuleHeight * 0.5f );
//	
//	dfloat distance; //Calculate the distance between the two intervals
//	distance = min1 < min2 ? min2-max1 : min1-max2;
//	
//	if( distance > 0.0f ) //If the intervals don't overlap, return, since there is no collision
//		return false;
//	
//	dfloat minDistance = 10000.0f;
//	dint32 edgeIndex = -1;
//	
//	// Project Polygon on to the seperating Axis defined by the normals of the Poly
//	for( dint32 v=0; v<numVertices; v++ )
//	{
//		minIndex = maxIndex = 0;
//		PROJECT_POLYGON( poly, numVertices, normals[v], minIndex, maxIndex );
//		v1 = vertices[minIndex];
//		v2 = vertices[maxIndex];
//		min1 = v1.dot(normals[v]);
//		max1 = v2.dot(normals[v]);
//		
//		proj = c.dot(normals[v]);
//		min2 = proj - ( capsuleRadius + capsuleHeight * 0.5f );
//		max2 = proj + ( capsuleRadius + capsuleHeight * 0.5f );
//		
//		distance = min1 < min2 ? min2-max1 : min1-max2;
//		
//		if( distance > 0.0f ) //If the intervals don't overlap, return, since there is no collision
//			return false;
//		
//		if( fabs( distance ) < minDistance )
//		{
//			minDistance = fabs( distance );
//			collisionNormal = normals[v]; 
//			edgeIndex = v;
//		}
//	}
//	dint32 vertexIndex1 = edgeIndex;
//	dint32 vertexIndex2 = edgeIndex == numVertices-1 ? 0 : edgeIndex+1;
//	
//	Vector2f edgeVertex1 = vertices[vertexIndex1];
//	Vector2f edgeVertex2 = vertices[vertexIndex2];
//	
//	Vector2f incidentEdge = edgeVertex1 - edgeVertex2;
//	incidentEdge.normalize();
//	
//	dfloat dot = axis.dot(collisionNormal);
//	dfloat FUZZINESS_FACTOR = 1e-03f;
//	dbool bIncidentEdgeCapsule = false;
//	
//	if( fabs(dot) - fabs(incidentEdge.dot(collisionNormal)) > FUZZINESS_FACTOR )
//	{
//		Vector2f tempEdge = axis;
//		axis = incidentEdge;
//		incidentEdge = tempEdge;
//		
//		Vector2f temp = edgeVertex1;
//		edgeVertex1 = p0;
//		p0 = temp;
//		
//		temp = edgeVertex2;
//		edgeVertex2 = p1;
//		p1 = temp;
//		bIncidentEdgeCapsule = true;
//	}		
//	contactManifold->m_ContactPoints[0].m_Point = edgeVertex1;
//	contactManifold->m_ContactPoints[1].m_Point = edgeVertex2;
//	contactManifold->m_NumContacts = 2;
//	
//	// Clip the Incident Edge Against the Edge Planes of the Reference Edge.
//	dint32 refIndex = 0;
//	
//	// Clip Against first Edge Plane of Reference Edge
//	Vector2f negAxis = -axis;
//	clip1(negAxis, p0, contactManifold, refIndex, capsuleRadius);
//
//	// Clip Against Second Edge Plane of Reference Edge
//	clip1(axis, p1, contactManifold, refIndex, capsuleRadius);
//	
//	// Clip the ContactPoints against the Reference Edge Normal.	
//	ContactPoint* contactPoints = contactManifold->m_ContactPoints;
//	
//	// Make sure that the Collision Normal Points from Shape2(Capsule) to Shape1(Poly).
//	if( (poly->m_Centroid-c).dot(collisionNormal) < 0.0f )
//		collisionNormal = -collisionNormal;
//	
//	if( bIncidentEdgeCapsule )
//	{
//		contactPoints[0].m_Point += collisionNormal * capsuleRadius;
//		contactPoints[1].m_Point += collisionNormal * capsuleRadius;
//	}
//	Vector2f refNormal = collisionNormal;
//	if( bIncidentEdgeCapsule )
//		refNormal *= -1.0f;
//	
//	Vector2f T = contactPoints[0].m_Point - p0;
//	dfloat dot1 = refNormal.dot(T);
//	
//	T = contactPoints[1].m_Point - p1;
//	dfloat dot2 = refNormal.dot(T);
//	dfloat d = fabs(capsuleRadius/(axisPerp.dot(refNormal)) );
//
//	if( bIncidentEdgeCapsule )
//		d = 0.0f;
//	
//	contactPoints[0].m_Depth = -(d - dot1);
//	contactPoints[1].m_Depth = -(d - dot2);
//
//	
//	xform1.transformAsPoint(contactPoints[0].m_Point);
//	xform1.transformAsPoint(contactPoints[1].m_Point);
//	
//	if( contactPoints[0].m_Depth > 0.0f )
//	{
//		contactPoints[0].m_Point = contactPoints[1].m_Point;
//		contactPoints[0].m_Depth = -(d + dot2);
//		contactManifold->m_NumContacts = 1;
//	}
//	
//	if( contactPoints[1].m_Depth > 0.0f )
//	{
//		contactManifold->m_NumContacts = 1;
//	}
//	
//	contactManifold->m_ContactNormal = collisionNormal;
//	
//	
//	return true;
//}

//findClosestPoint(vertices[v], vertices[i], p0, closestPointOnPolyEdge);
//findClosestPoints(vertices[v], vertices[i], p0, p1, closestPointOnPolyEdge, closestPointOnCapsuleAxis);
//dfloat d2 = closestPointOnPolyEdge.distanceSquared(closestPointOnCapsuleAxis);
//if( d2 > capsuleRadius*capsuleRadius )
//continue;
//bRes = true;	
//contactManifold->m_NumContacts = 1;
//contactManifold->m_ContactNormal = closestPointOnPolyEdge - closestPointOnCapsuleAxis;
//contactManifold->m_ContactNormal.normalize();
//
//Vector2f closestPointOnCapsule = closestPointOnCapsuleAxis + contactManifold->m_ContactNormal * capsuleRadius;
//
//contactManifold->m_ContactPoints[0].m_Point = (closestPointOnCapsule + closestPointOnPolyEdge) * 0.5f;
//xform1.transformAsPoint(contactManifold->m_ContactPoints[0].m_Point);
//
//dfloat d = (closestPointOnPolyEdge - closestPointOnCapsuleAxis).dot(contactManifold->m_ContactNormal);
//contactManifold->m_ContactPoints[0].m_Depth = -(capsuleRadius - d);
//
//xform1.transformAsVector(contactManifold->m_ContactNormal);
//
//dAssert(contactManifold->m_ContactPoints[0].m_Depth < 0.0f);

END_NAMESPACE_DIRAC2D
