/*
 *  CollisionDetectionPolygon.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 30/04/12.
 *
 */

#include "CollisionDetectionPolygon.h"
#include <stdio.h>

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
	dint32 numVertices1 = poly1->getNumVertices();
	dint32 numVertices2 = poly2->getNumVertices();
	
	float minDistance = 10000.0f; //Initialize the length of the collision vector to a relatively large value
	Vector2f collisionNormal;
	
	dbool bRes = findSeperationAxis(poly1, polygonXform1, poly2, polygonXform2, minDistance, collisionNormal);
	
	if( !bRes )
		return false;
	
	bRes = findSeperationAxis(poly2, polygonXform2, poly1, polygonXform1, minDistance, collisionNormal);

	if( !bRes )
		return false;
	
	//printf("Collision Detected %f\n", minDistance);
	collisionNormal.normalize();
	
	Vector2f c1 = poly1->m_Centroid;
	Vector2f c2 = poly2->m_Centroid;
	polygonXform1.transformAsPoint(c1);
	polygonXform2.transformAsPoint(c2);
		
	Vector2f d = c1 - c2;
	
	dfloat dot = d.dot(collisionNormal);
	
	// Collision Normal Always Points from Shape 2 to Shape 1
	if( dot < 0.0f )
		collisionNormal = -collisionNormal;
	
	// Find the Best Candidate Edge of Polygons for Clipping.
	Vector2f edge1, edge2;
	dint32 p1, p2, p3, p4;
	
	Vector2f normalLocal = (polygonXform2.getRotationMatrixTransposed() * collisionNormal);
	
	findCandidateEdge( poly2, normalLocal , p1, p2 );
	
	Vector2f negNormal = -(polygonXform1.getRotationMatrixTransposed() * collisionNormal);
	
	findCandidateEdge( poly1, negNormal , p3, p4 );
	
	// Find Reference and incident Edges
	Vector2f refEdge      = vertices2[p2] - vertices2[p1];
	Vector2f incidentEdge = vertices1[p4] - vertices1[p3];
	
	polygonXform2.transformAsVector(refEdge);
	polygonXform1.transformAsVector(incidentEdge);
	
	refEdge.normalize();
	incidentEdge.normalize();
	
	dot = refEdge.dot(collisionNormal);
	Matrix3f Xform1 = polygonXform1;
	Matrix3f Xform2 = polygonXform2;
	
	if( fabs(dot) > fabs(incidentEdge.dot(collisionNormal)) )
	{
		Matrix3f tempMat = Xform2;
		Xform2 = Xform1;
		Xform1 = tempMat;
		
		Vector2f tempEdge = refEdge;
		refEdge = incidentEdge;
		incidentEdge = tempEdge;
		
		Vector2f* tempVertices = vertices1;
		vertices1 = vertices2;
		vertices2 = tempVertices;
		
		dint32 temp = p1;
		p1 = p3;
		p3 = temp;
		
		temp = p2;
		p2 = p4;
		p4 = temp;
		
		temp = numVertices1;
		numVertices1 = numVertices2;
		numVertices2 = temp;
	}
	
	
	contactManifold->m_ContactPoints[0].m_Point = Xform1 * vertices1[p3];
	contactManifold->m_ContactPoints[0].m_ID.edgeIndex.m_Index1 = (p3==0 ? numVertices1-1 : p3-1);
	contactManifold->m_ContactPoints[0].m_ID.edgeIndex.m_Index2 = p3;
	contactManifold->m_ContactPoints[0].m_ID.edgeIndex.m_Type1 = ECT_VERTEX;
	contactManifold->m_ContactPoints[0].m_ID.edgeIndex.m_Type2 = ECT_VERTEX;
	
	contactManifold->m_ContactPoints[1].m_Point = Xform1 * vertices1[p4];
	contactManifold->m_ContactPoints[1].m_ID.edgeIndex.m_Index1 = p4;
	contactManifold->m_ContactPoints[1].m_ID.edgeIndex.m_Index2 = (p4==0 ? numVertices1-1 : p4-1);
	contactManifold->m_ContactPoints[1].m_ID.edgeIndex.m_Type1 = ECT_VERTEX;
	contactManifold->m_ContactPoints[1].m_ID.edgeIndex.m_Type2 = ECT_VERTEX;
	
	contactManifold->m_NumContacts = 2;
	
	// Clip Against first Edge Plane of Reference Edge
	Vector2f edgeVertex1 = Xform2 * vertices2[p1];
	dint32 refIndex = ( p1==0 ? numVertices2-1 : p1-1 );
	clip(refEdge, edgeVertex1, contactManifold, refIndex);

	Vector2f refNegEdge = -refEdge;

	// Clip Against Second Edge Plane of Reference Edge
	Vector2f edgeVertex2 = Xform2 * vertices2[p2];
	clip(refNegEdge, edgeVertex2, contactManifold, p2);

	// Clip the ContactPoints against the Reference Edge Normal.
	Vector2f refNormal( refEdge.y, -refEdge.x);
	
	ContactPoint* contactPoints = contactManifold->m_ContactPoints;
	
	refNormal.normalize();
	
	Vector2f T = contactPoints[0].m_Point - edgeVertex1;
	float dot1 = refNormal.dot(T);
	
	T = contactPoints[1].m_Point - edgeVertex2;
	float dot2 = refNormal.dot(T);
	
	contactPoints[0].m_Depth = dot1;
	contactPoints[1].m_Depth = dot2;
	
	
	if( dot1 > 0.0f )
	{
		contactPoints[0].m_Point = contactPoints[1].m_Point;
		contactPoints[0].m_Depth = dot2;
		contactManifold->m_NumContacts = 1;
	}
	
	if( dot2 > 0.0f )
	{
		contactManifold->m_NumContacts = 1;
	}
	
	contactManifold->m_ContactNormal = collisionNormal;
	return true;
}


END_NAMESPACE_DIRAC2D
