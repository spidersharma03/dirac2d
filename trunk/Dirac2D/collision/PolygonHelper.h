/*
 *  PolygonHelper.h
 *  Dirac2D
 *
 *  Created by Prashant on 05/06/12.
 *
 */

#include "../Settings.h"
#include "../dynamics/contacts/Contact.h"

#ifndef _POLYGON_HELPER_H_
#define _POLYGON_HELPER_H_

BEGIN_NAMESPACE_DIRAC2D

// Finds an Edge on the Shape which is most perpendicular to the input Normal
static void findCandidateEdge( ConvexPolygon* poly, Vector2f& normal, dint32& edgeVertex1, dint32& edgeVertex2 )
{
	// Find the candidate vertex which is farthest in normal direction
	dfloat maxDot = -100000.0f;
	dint32 index;
	dint32 numVertices = poly->getNumVertices();
	Vector2f* vertices = poly->getVertices();
	
	// Find the Vertex which is Farthest in Normal Direction
	for( dint32 i=0; i<numVertices; i++ )
	{
		Vector2f& p =  vertices[i];
		dfloat dot = p.dot(normal);
		if( dot > maxDot )
		{
			maxDot = dot;
			index = i;
		}
	}
	duint16 index1 = index;
	duint16 newindex = (index == numVertices-1) ? 0 : index+1;
	duint16 index2 = newindex;
	
	Vector2f p1 = vertices[index1];
	Vector2f p2 = vertices[index2];
	edgeVertex1 = index1;
	edgeVertex2 = index2;
	
	Vector2f d = p2 - p1;
	d.normalize();
	dfloat dot = d.dot(normal);
	dint32 edgeIndex = index;
	newindex = index - 1;
	index2 = index;
	index1 = newindex;
	
	if( index == 0 )
	{
		newindex = numVertices-1;
		index2 = index;
		index1 = newindex;
	}
	
	p1 = vertices[index1];
	p2 = vertices[index2];
	
	d = p2 - p1;
	d.normalize();
	dfloat dot1 = d.dot(normal);
	if( fabs(dot) > fabs(dot1) )
	{
		edgeIndex = newindex;
		edgeVertex1 = index1;
		edgeVertex2 = index2;
	}
}

// Clip Against Edge Plane
static void clip(Vector2f& refEdge, Vector2f& p, ContactManifold* contactManifold, dint32 refIndex)
{
	Vector2f& cp0 = contactManifold->m_ContactPoints[0].m_Point;
	Vector2f& cp1 = contactManifold->m_ContactPoints[1].m_Point;
	
	Vector2f o1 = cp0 - p;
	dfloat u1 = refEdge.dot(o1);
	o1 = cp1 - p;
	dfloat u2 = refEdge.dot(o1);
	
	dfloat lambda = u1/(u1-u2);
	
	if( u1 < 0.0f ) 
	{
		cp0.x = cp0.x + lambda*(cp1.x - cp0.x); 
		cp0.y = cp0.y + lambda*(cp1.y - cp0.y); 
		contactManifold->m_ContactPoints[0].m_ID.edgeIndex.m_Index1 = refIndex;
		contactManifold->m_ContactPoints[0].m_ID.edgeIndex.m_Type1  = ECT_EDGE;
	}
	if( u2 < 0.0f ) 
	{
		cp1.x = cp0.x + lambda*(cp1.x - cp0.x); 
		cp1.y = cp0.y + lambda*(cp1.y - cp0.y); 
		contactManifold->m_ContactPoints[1].m_ID.edgeIndex.m_Index1 = refIndex;
		contactManifold->m_ContactPoints[1].m_ID.edgeIndex.m_Type1  = ECT_EDGE;
	}
}


END_NAMESPACE_DIRAC2D


#endif