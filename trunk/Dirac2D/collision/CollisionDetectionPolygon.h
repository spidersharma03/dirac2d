/*
 *  CollisionDetection.h
 *  Dirac2D
 *
 *  Created by Prashant on 24/04/12.
 *
 */

#include <math.h>
#include "../common.h"
#include "../definitions.h"
#include "../Settings.h"
#include "../maths/MathUtil.h"
#include "../geometry/CollisionShape.h"
#include "../geometry/RegularPolygon.h"
#include "../dynamics/contacts/Contact.h"

#include <stdio.h>

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

	Vector2f* vertices1 = poly1->getVertices();
	Vector2f* vertices2 = poly2->getVertices();
	
	Vector2f v1 = vertices1[0];
	Vector2f v2 = vertices1[0];
	Matrix2f xForm1, xForm2;
	// Transform which converts Seperating Normal into Polygon2's Space.
	xForm1 = polyxForm1.getRotationMatrix();
	xForm2 = polyxForm2.getRotationMatrixTransposed();
	xForm2 *= xForm1;
	Vector2f axisNormalLocal, axisNormalWorld;
	
	for( dint32 i=0; i<numVertices1; i++ )
	{
		v1 = vertices1[i];
		dint32 j = i;
		if( i == numVertices1-1)
			j = -1;
		v2 = vertices1[j+1];
		
		axisNormalLocal.set(v1.x - v2.x, v2.y - v1.y); //Calculate the perpendicular to this edge
		// Normal in world space.
		axisNormalWorld = xForm1 * axisNormalLocal;
		
		dfloat min1 = 10000.0f, min2 = 10000.0f, max1 = -10000.0f, max2 = -10000.0f; 
		dint32 minIndex = 0, maxIndex = 0;
		//Project both Polygons on to the perpendicular
		PROJECT_POLYGON( poly1, numVertices1, axisNormalLocal, minIndex, maxIndex )
		Vector2f p1 = vertices1[minIndex];
		Vector2f p2 = vertices1[maxIndex];
		polyxForm1.transformAsPoint(p1);
		polyxForm1.transformAsPoint(p2);		
		
		min1 = p1.dot(axisNormalWorld);
		max1 = p2.dot(axisNormalWorld);
		// Transform the seperating normal into Polygon2's space
		axisNormalLocal = xForm2 * axisNormalLocal;
		
		minIndex = 0, maxIndex = 0;
		PROJECT_POLYGON( poly2 ,numVertices2, axisNormalLocal, minIndex, maxIndex )
		p1 = vertices2[minIndex];
		p2 = vertices2[maxIndex];
		polyxForm2.transformAsPoint(p1);
		polyxForm2.transformAsPoint(p2);
		
		min2 = p1.dot(axisNormalWorld);
		max2 = p2.dot(axisNormalWorld);
		
		dfloat distance; //Calculate the distance between the two intervals
		distance = min1 < min2 ? min2-max1 : min1-max2;
		
		if( distance > 0.0f ) //If the intervals don't overlap, return, since there is no collision
			return false;
		
		if( fabs( distance ) < minDistance )
		{
			minDistance = fabs( distance );
			collisionNormal = axisNormalWorld; //Save collision information for later
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
	
	// Find the Vertex which is Farthest in Normal Direction
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
		contactManifold->m_ContactPoints[0].m_ID.edgeIndex.m_Type1  = ECT_VERTEX;
	}
	if( u2 < 0.0f ) 
	{
		cp1.x = cp0.x + lambda*(cp1.x - cp0.x); 
		cp1.y = cp0.y + lambda*(cp1.y - cp0.y); 
		contactManifold->m_ContactPoints[1].m_ID.edgeIndex.m_Index1 = refIndex;
		contactManifold->m_ContactPoints[1].m_ID.edgeIndex.m_Type1  = ECT_VERTEX;
	}
}


// Find Whether two Regular/ConvexPolygons intersect.
dbool intersectPolygons( RegularPolygon* poly1, Matrix3f& xform1, RegularPolygon* poly2, Matrix3f& xform2);


// Find Whether two Regular/ConvexPolygons intersect. Also find the Contact Points if the Polygons intersect.
dbool intersectPolygons( RegularPolygon* poly1, Matrix3f& xform1, RegularPolygon* poly2, Matrix3f& xform2, ContactManifold* contactManifold);


END_NAMESPACE_DIRAC2D

#endif