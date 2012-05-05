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


#define PROJECT_POLYGON( POLYGON, NUMVERTICES, AXIS, MININDEX , MAXINDEX ) \
{ \
  Vector2f* VERTICES = POLYGON->getVertices();	\
  float dot = AXIS.dot(VERTICES[0]); \
  dfloat min = dot; dfloat max = dot; \
  for( dint32 i = 1; i < NUMVERTICES; i++ ) \
  { \
	dot = AXIS.dot(VERTICES[i]); \
	if( dot < min ){ \
	  min = dot; \
      MININDEX = i; \
	} \
    if( dot > max ){ \
	  max = dot; \
      MAXINDEX = i; \
	} \
} \
} \

// Project both the Polygons on to the list of seperating axis of Polygon 1. if there is an Axis which seperates the Polygons, then return.
static dbool findSeperationAxis(RegularPolygon* poly1, Matrix3f& polyxForm1, RegularPolygon* poly2, Matrix3f& polyxForm2, dfloat& minDistance, Vector2f& collisionNormal )
{
	dint32 numVertices1 = poly1->getNumVertices();
	dint32 numVertices2 = poly2->getNumVertices();

	Vector2f* vertices = poly1->getVertices();
	
	Vector2f& v1 = vertices[0];
	Vector2f& v2 = vertices[0];
	Matrix2f xForm1, xForm2;
	// Transform which converts Seperating Normal into Polygon2's Space.
	xForm1 = polyxForm1.getRotationMatrix();
	xForm2 = polyxForm2.getRotationMatrixTransposed();
	xForm2 *= xForm1;
	
	for( dint32 i=0; i<numVertices1; i++ )
	{
		v1 = vertices[i];
		if( i == numVertices1-1)
			i = -1;
		v2 = vertices[i+1];
		
		Vector2f axisNormal(v1.x - v2.x, v2.y - v1.y); //Calculate the perpendicular to this edge
		
		dfloat min1 = 10000.0f, min2 = 10000.0f, max1 = -10000.0f, max2 = -10000.0f; 
		dint32 minIndex, maxIndex;
		//Project both Polygons on to the perpendicular
		PROJECT_POLYGON( poly1, numVertices1, axisNormal, minIndex, maxIndex )
		
		// Transform the seperating normal into Polygon2's space
		axisNormal = xForm2 * axisNormal;
		
		PROJECT_POLYGON( poly2 ,numVertices2, axisNormal, minIndex, maxIndex )
		
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
	return true;
}

// Finds an Edge on the Shape which is most perpendicular to the input Normal
static void findCandidateEdge( RegularPolygon* poly, Vector2f& normal, dint32& edgeVertex1, dint32& edgeVertex2 )
{
	// Find the candidate vertex which is farthest in normal direction
	dfloat maxDot = -100000.0f;
	dint32 index;
	dint32 numVertices = poly->getNumVertices();
	Vector2f* vertices = poly->getVertices();
	
	// Transform the input normal in polygon's space
	Vector2f localNormal = normal;
	
	for( dint32 i=0; i<numVertices; i++ )
	{
		Vector2f& p =  vertices[i];
		float dot = p.dot(localNormal);
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