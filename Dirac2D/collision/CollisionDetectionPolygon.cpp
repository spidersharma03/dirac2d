/*
 *  CollisionDetectionPolygon.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 30/04/12.
 *
 */

#include "CollisionDetectionPolygon.h"

BEGIN_NAMESPACE_DIRAC2D


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////         POLYGON COLLISION TESTS      ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Find Whether two Regular/ConvexPolygons intersect.
dbool intersectPolygons( RegularPolygon& poly1, Matrix3f& xform1, RegularPolygon& poly2, Matrix3f& xform2)
{
	return 0;
}


// Find Whether two Regular/ConvexPolygons intersect. Also find the Contact Points if the Polygons intersect.
dbool intersectPolygons( RegularPolygon* poly1, Matrix3f& xform1, RegularPolygon* poly2, Matrix3f& xform2, ContactManifold* contactManifold)
{
	dint32 numVertices1 = poly1->getNumVertices();
	dint32 numVertices2 = poly2->getNumVertices();
	Vector2f* vertices1 = poly1->getVertices();
	Vector2f* vertices2 = poly2->getVertices();
	
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
		
		dfloat min1 = 0.0f, min2 = 0.0f, max1 = 0.0f, max2 = 0.0f; 
		
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
	
	Vector2f d = poly1->m_Centroid  - poly2->m_Centroid;
	
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
	
	contactManifold->m_ContactPoints[0].m_Point = vertices1[p3];
	contactManifold->m_ContactPoints[0].m_Point = vertices1[p4];
	
	// Clip Against first Edge Plane
	clip(refEdge, vertices1[p1], contactManifold);
	
	Vector2f refNegEdge = -refEdge;
	
	// Clip Against Second Edge Plane
	clip(refNegEdge, vertices1[p2], contactManifold);
	
	// Clip the ContactPoints against the Reference Edge Normal.
	Vector2f refNormal( -refEdge.y, refEdge.x);
	
	if( bFlip )
		refNormal = -refNormal;
	
	ContactPoint* contactPoints = contactManifold->m_ContactPoints;
	
	Vector2f T = contactPoints[0].m_Point - vertices2[p1];
	float dot1 = refNormal.dot(T);
	
	T = contactPoints[1].m_Point - vertices2[p2];
	float dot2 = refNormal.dot(T);
	
	contactPoints[0].m_Depth = dot1;
	contactPoints[1].m_Depth = dot2;
	
	contactManifold->m_NumContacts = 2;
	
	if( dot1 < 0.0f )
	{
		contactPoints[0].m_Point = vertices2[p2];
		contactPoints[0].m_Depth = dot2;
		contactManifold->m_NumContacts = 1;
	}
	
	
	
	if( dot2 < 0.0f )
	{
		contactPoints[0].m_Point = vertices2[p1];
		contactPoints[0].m_Depth = dot1;
		contactManifold->m_NumContacts = 1;
	}
	
	contactManifold->m_ContactNormal = collisionNormal;
	
	return true;
}


END_NAMESPACE_DIRAC2D
