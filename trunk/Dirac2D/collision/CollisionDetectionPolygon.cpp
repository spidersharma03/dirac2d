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
dbool intersectPolygons( RegularPolygon* poly1, Matrix3f& polygonXform1, RegularPolygon* poly2, Matrix3f& polygonXform2, ContactManifold* contactManifold)
{
	Vector2f* vertices1 = poly1->getVertices();
	Vector2f* vertices2 = poly2->getVertices();
	
	float minDistance = 10000.0f; //Initialize the length of the collision vector to a relatively large value
	Vector2f collisionNormal;
	
	dbool bRes = findSeperationAxis(poly1, polygonXform1, poly2, polygonXform2, minDistance, collisionNormal);
	
	if( !bRes )
		return false;
	
	bRes = findSeperationAxis(poly2, polygonXform2, poly1, polygonXform1, minDistance, collisionNormal);

	if( !bRes )
		return false;
	
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
