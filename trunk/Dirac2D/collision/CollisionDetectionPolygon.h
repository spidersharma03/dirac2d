/*
 *  CollisionDetection.h
 *  Dirac2D
 *
 *  Created by Prashant on 24/04/12.
 *
 */

#include <math.h>
#include "../definitions.h"
#include "../Settings.h"
#include "../maths/MathUtil.h"
#include "../geometry/Shape.h"
#include "../geometry/RegularPolygon.h"
#include "../dynamics/contacts/Contact.h"

#ifndef _COLLISION_DETECTION_H_
#define _COLLISION_DETECTION_H_

BEGIN_NAMESPACE_DIRAC2D


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////         POLYGON COLLISION TESTS      ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define PROJECT_POLYGON( VERTICES, NUMVERTICES, AXIS, MINIMUM, MAXIMUM ) \
{ \
  float dot = AXIS.dot(VERTICES[0]); \
  min1 = max1 = dot; \
  for( dint32 i = 1; i < NUMVERTICES; i++ ) \
  { \
	dot = AXIS.dot(VERTICES[i]); \
	MINIMUM = MIN( dot, MINIMUM ); \
	MAXIMUM = MAX( dot, MAXIMUM ); \
  } \
} \

// Finds an Edge on the Shape which is most perpendicular to the input Normal
static void findCandidateEdge( RegularPolygon& shape, Vector2f& normal, dint32& edgeVertex1, dint32& edgeVertex2 )
{
	// Find the candidate vertex which is farthest in normal direction
	dfloat maxDot = -100000.0f;
	dint32 index;
	dint32 numVertices = shape.getNumVertices();
	Vector2f* vertices = shape.getVertices();
	for( dint32 i=0; i<numVertices; i++ )
	{
		Vector2f& p =  vertices[i];
		float dot = p.dot(normal);
		if( dot > maxDot )
		{
			maxDot = dot;
			index = i;
		}
	}
	duint16 index1 = index;
	duint16 newindex = (index == numVertices-1) ? 0 : index+1;
	duint16 index2 = newindex;
	
	Vector2f& p1 = vertices[index1];
	Vector2f& p2 = vertices[index2];
	edgeVertex1 = index1;
	edgeVertex2 = index2;
	
	Vector2f d = p2 - p1;
	dfloat dot = d.dot(normal);
	dint32 edgeIndex = index;
	newindex = index - 1;
	
	if( index == 0 )
		newindex = numVertices-1;
		
		index2 = newindex;
		p1 = vertices[index1];
		p2 = vertices[index2];
		d = p2 - p1;
		dfloat d1 = d.dot(normal);
		if( fabs(dot) > fabs(d1) )
		{
			edgeIndex = newindex;
			edgeVertex1 = index1;
			edgeVertex2 = index2;
		}
}

// Clip Against Edge Plane
static void clip(Vector2f& refEdge, Vector2f& p, ContactManifold& contactManifold)
{
	Vector2f& cp0 = contactManifold.m_ContactPoints[0].m_ContactPoint;
	Vector2f& cp1 = contactManifold.m_ContactPoints[1].m_ContactPoint;

	Vector2f o1 = cp0 - p;
	dfloat u1 = refEdge.dot(o1);
	o1 = cp1 - p;
	dfloat u2 = refEdge.dot(o1);
	dfloat lambda = u1/(u1-u2);
	
	if( u1 < 0.0f ) 
	{
		cp0.x = cp0.x + lambda*(cp1.x - cp0.x); 
		cp0.y = cp0.y + lambda*(cp1.y - cp0.y); 
	}
	if( u2 < 0.0f )
	{
		cp1.x = cp0.x + lambda*(cp1.x - cp0.x); 
		cp1.y = cp0.y + lambda*(cp1.y - cp0.y); 
	}	
}


// Find Whether two Regular/ConvexPolygons intersect.
dbool intersectPolygons( RegularPolygon& poly1, Matrix3f& xform1, RegularPolygon& poly2, Matrix3f& xform2)
{
	return 0;
}


// Find Whether two Regular/ConvexPolygons intersect. Also find the Contact Points if the Polygons intersect.
dbool intersectPolygons( RegularPolygon& poly1, Matrix3f& xform1, RegularPolygon& poly2, Matrix3f& xform2, ContactManifold& contactManifold)
{
	dint32 numVertices1 = poly1.getNumVertices();
	dint32 numVertices2 = poly2.getNumVertices();
	Vector2f* vertices1 = poly1.getVertices();
	Vector2f* vertices2 = poly2.getVertices();
	
	// Loop through the vertices of polygon1
	Vector2f& v1 = vertices1[0];
	Vector2f& v2 = vertices1[1];
	
	float minDistance = 10000.0f; //Initialize the length of the collision vector to a relatively large value
	Vector2f collisionNormal;
	
	for( dint32 i=0; i<numVertices1+numVertices2; i++ )
	{
		if( i < numVertices1 )
		{
		    v1 = vertices1[i];
			if( i == numVertices1-1)
				i = -1;
		    v2 = vertices1[i+1];
		}
		else
		{
			v1 = vertices2[i-numVertices1];
			if( i == numVertices1+numVertices2-1)
				i = numVertices1-1;
			v2 = vertices2[i-numVertices1+1];
		}
		Vector2f axisNormal(v1.x - v2.x, v2.y - v1.y); //Calculate the perpendicular to this edge and normalize it
		
		dfloat min1, min2, max1, max2; 
		
		//Project both Polygons on to the perpendicular
		PROJECT_POLYGON( vertices1, numVertices1, axisNormal, min1, max1 )
		
		PROJECT_POLYGON( vertices2, numVertices2, axisNormal, min2, max2 )
		
		dfloat distance; //Calculate the distance between the two intervals
		distance = min1 < min2 ? min2-max1 : min1-max2;
		
		if( distance > 0.0f ) //If the intervals don't overlap, return, since there is no collision
			return false;
		
		if( fabs( distance ) < minDistance )
		{
			minDistance = fabs( distance );
			collisionNormal = axisNormal; //Save collision information for later
		}
		
	}
	
	//dfloat m_CollisionDepth = minDistance;
	
	Vector2f d = poly1.m_Centroid  - poly2.m_Centroid;
	
	dfloat dot = d.dot(collisionNormal);
	
	// Collision Normal Always Points from Shape 2 to Shape 1
	if( dot < 0.0f )
		collisionNormal = -collisionNormal;
	
	// Find the Best Candidate Edge of Polygons for Clipping.
	Vector2f edge1, edge2;
	dint32 p1, p2, p3, p4;
	
	findCandidateEdge( poly2, collisionNormal , p1, p2 );
	
	Vector2f negNormal = -collisionNormal;

	findCandidateEdge( poly1, negNormal , p3, p4 );
	
	// Find Reference and incident Edges
	Vector2f refEdge      = vertices2[p2] - vertices2[p1];
	Vector2f incidentEdge = vertices1[p4] - vertices1[p3];
	
	dot = refEdge.dot(collisionNormal);
	dbool bFlip = false;
	if( fabs(dot) > fabs(incidentEdge.dot(collisionNormal)) )
	{
		refEdge      = vertices1[p4] - vertices1[p3];
		incidentEdge = vertices2[p2] - vertices2[p1];
		
		Vector2f* tempVertices = vertices1;
		vertices1 = vertices2;
		vertices2 = tempVertices;
		
		dint32 temp = p1;
		p1 = p3;
		p3 = temp;
		
		temp = p2;
		p2 = p4;
		p4 = temp;
		bFlip = true;
	}
	
	contactManifold.m_ContactPoints[0].m_ContactPoint = vertices1[p3];
	contactManifold.m_ContactPoints[0].m_ContactPoint = vertices1[p4];
		
	// Clip Against first Edge Plane
	clip(refEdge, vertices1[p1], contactManifold);
	
	Vector2f refNegEdge = -refEdge;

	// Clip Against Second Edge Plane
	clip(refNegEdge, vertices1[p2], contactManifold);
	
	// Clip the ContactPoints against the Reference Edge Normal.
	Vector2f refNormal( -refEdge.y, refEdge.x);
	
	if( bFlip )
		refNormal = -refNormal;
	
	Vector2f T = contactManifold.m_ContactPoints[0].m_ContactPoint - vertices2[p1];
	float dot1 = refNormal.dot(T);
	
	T = contactManifold.m_ContactPoints[1].m_ContactPoint - vertices2[p2];
	float dot2 = refNormal.dot(T);
	
	contactManifold.m_ContactPoints[0].m_Depth = fabs(dot1);
	contactManifold.m_ContactPoints[1].m_Depth = fabs(dot2);

	contactManifold.m_NumContacts = 2;
	
	if( dot1 < 0.0f )
	{
		contactManifold.m_ContactPoints[0].m_ContactPoint = vertices2[p2];
		contactManifold.m_ContactPoints[0].m_Depth = dot2;
		contactManifold.m_NumContacts = 1;
	}
	
	

	if( dot2 < 0.0f )
	{
		contactManifold.m_ContactPoints[0].m_ContactPoint = vertices2[p1];
		contactManifold.m_ContactPoints[0].m_Depth = dot1;
		contactManifold.m_NumContacts = 1;
	}
	
	contactManifold.m_ContactNormal = collisionNormal;

	return true;
}


END_NAMESPACE_DIRAC2D

#endif