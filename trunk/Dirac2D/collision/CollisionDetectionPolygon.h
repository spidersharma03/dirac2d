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
#include "../geometry/CollisionShape.h"
#include "../geometry/RegularPolygon.h"
#include "../dynamics/contacts/Contact.h"

#ifndef _COLLISION_DETECTION_POLYGON_H_
#define _COLLISION_DETECTION_POLYGON_H_

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
static void findCandidateEdge( RegularPolygon* poly, Vector2f& normal, dint32& edgeVertex1, dint32& edgeVertex2 )
{
	// Find the candidate vertex which is farthest in normal direction
	dfloat maxDot = -100000.0f;
	dint32 index;
	dint32 numVertices = poly->getNumVertices();
	Vector2f* vertices = poly->getVertices();
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
static void clip(Vector2f& refEdge, Vector2f& p, ContactManifold* contactManifold)
{
	Vector2f& cp0 = contactManifold->m_ContactPoints[0].m_Point;
	Vector2f& cp1 = contactManifold->m_ContactPoints[1].m_Point;

	Vector2f o1 = cp0 - p;
	dfloat u1 = refEdge.dot(o1);
	o1 = cp1 - p;
	dfloat u2 = refEdge.dot(o1);
	dfloat lambda = u1/(u1-u2);
	
	if( u1*u2 < 0.0f ) 
	{
		cp0.x = cp0.x + lambda*(cp1.x - cp0.x); 
		cp0.y = cp0.y + lambda*(cp1.y - cp0.y); 
	}
}


// Find Whether two Regular/ConvexPolygons intersect.
dbool intersectPolygons( RegularPolygon* poly1, Matrix3f& xform1, RegularPolygon* poly2, Matrix3f& xform2);


// Find Whether two Regular/ConvexPolygons intersect. Also find the Contact Points if the Polygons intersect.
dbool intersectPolygons( RegularPolygon* poly1, Matrix3f& xform1, RegularPolygon* poly2, Matrix3f& xform2, ContactManifold* contactManifold);


END_NAMESPACE_DIRAC2D

#endif