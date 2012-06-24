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
#include "../geometry/ConvexPolygon.h"
#include "../dynamics/contacts/Contact.h"


#ifndef _COLLISION_DETECTION_POLYGON_H_
#define _COLLISION_DETECTION_POLYGON_H_

BEGIN_NAMESPACE_DIRAC2D


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////         POLYGON COLLISION TESTS      ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NEW

#ifndef NEW
// Project both the Polygons on to the list of seperating axis of Polygon 1. if there is an Axis which seperates the Polygons, then return.
static dbool findSeperationAxis(ConvexPolygon* poly1, Matrix3f& polyxForm1, ConvexPolygon* poly2, Matrix3f& polyxForm2, dfloat& minDistance, Vector2f& collisionNormal )
{
	dint32 numVertices1 = poly1->getNumVertices();
	dint32 numVertices2 = poly2->getNumVertices();

	Vector2f* vertices1 = poly1->getVertices();
	Vector2f* vertices2 = poly2->getVertices();
	
	Vector2f* normals = poly1->getNormals();
	

	Matrix2f xForm1, xForm2;
	// Transform which converts Seperating Normal into Polygon2's Space.
	xForm1 = polyxForm1.getRotationMatrix();
	xForm2 = polyxForm2.getRotationMatrixTransposed();
	xForm2 *= xForm1;
	Vector2f axisNormalLocal = normals[0];
	Vector2f axisNormalWorld;
	
	for( dint32 i=0; i<numVertices1; i++ )
	{
		axisNormalLocal = normals[i];
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
		
		//Calculate the distance between the two intervals
		dfloat distance = min1 < min2 ? min2-max1 : min1-max2;
		//dfloat distance = min2 - max1;
		
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

#else

//
static void findIncidentEdge()
{
}
// Finds seperation between Poly1 and Poly2 along the given nomal
static dfloat findMinSeperation(ConvexPolygon* poly1, Matrix3f& polyxForm1, ConvexPolygon* poly2, Matrix3f& polyxForm2, Vector2f normal, dint32 edgeIndex )
{
	dint32 numVertices2 = poly2->getNumVertices();
	
	Vector2f* vertices1 = poly1->getVertices();
	Vector2f* vertices2 = poly2->getVertices();
	
	Vector2f normalWorld;
	// Transform normal in poly2's space
	polyxForm1.transformAsVector(normal);
	normalWorld = normal;
	normal *= polyxForm2.getRotationMatrix();
	
	// Find the farthest vertex in Poly2 along the negative normal
	dint32 index = 0;
	dfloat proj = 1000000.0f;
	for( dint32 i=0; i<numVertices2; i++ )
	{
		dfloat dot = vertices2[i].dot(normal);
		if( dot < proj )
		{
			index = i;
			proj = dot;
		}
	}
	
	Vector2f v1 = polyxForm1 * vertices1[edgeIndex];
	Vector2f v2 = polyxForm2 * vertices2[index];
	
	dfloat d = ( v2 - v1 ).dot( normalWorld );
	return d;
}
// Project both the Polygons on to the list of seperating axis of Polygon 1. if there is an Axis which seperates the Polygons, then return.
static dbool findSeperationAxis(ConvexPolygon* poly1, Matrix3f& polyxForm1, ConvexPolygon* poly2, Matrix3f& polyxForm2, dfloat& minDistance, dint32& edgeIndex )
{
	dint32 numVertices1 = poly1->getNumVertices();
		
	Vector2f* normals = poly1->getNormals();
	
	// Find Vector from Poly2 centroid to Poly1 centroid in Poly 1's frame.
	Vector2f c2c1 = polyxForm2 * poly2->m_Centroid - polyxForm1 * poly1->m_Centroid; // World space
	c2c1 *= polyxForm1.getRotationMatrix();  // Local space of poly1
	
	// Find the Normal on the Poly 1 which is most parallel to the direction c2c1.
	dint32 index = 0;
	dfloat proj = -1000000.0f;
	for( dint32 i=0; i<numVertices1; i++ )
	{
		dfloat dot = c2c1.dot(normals[i]);
		if( dot > proj )
		{
			index = i;
			proj = dot;
		}
	}
	
	// Minimum seperation between poly1 and poly2 along the edge normal 
	dfloat d = findMinSeperation(poly1, polyxForm1, poly2, polyxForm2, normals[index], index);
	
	// Early Exit. no seperation axis exists
	if( d > 0.0f )
		return false;
	
	// Minimum separation for the previous edge normal.
	dint32 prevEdgeIndex = index - 1 >= 0 ? index - 1 : numVertices1 - 1;
	dfloat d1 = findMinSeperation(poly1, polyxForm1, poly2, polyxForm2, normals[prevEdgeIndex], prevEdgeIndex);
	
	// Minimum separation for the next edge normal.
	dint32 nextEdgeIndex = index + 1 < numVertices1 ? index + 1 : 0;
	dfloat d2 = findMinSeperation(poly1, polyxForm1, poly2, polyxForm2, normals[nextEdgeIndex], nextEdgeIndex);
	
	dint32 bestEdge;
	dint32 increment;
	if (d1 > d && d1 > d2)
	{
		increment = -1;
		bestEdge = prevEdgeIndex;
		minDistance = d1;
	}
	else if (d2 > d)
	{
		increment = 1;
		bestEdge = nextEdgeIndex;
		minDistance = d2;
	}
	else
	{
		minDistance = d;
		edgeIndex = index;
		return minDistance < 0.0f;
	}
	
	while( true )
	{
		if (increment == -1)
			index = bestEdge - 1 >= 0 ? bestEdge - 1 : numVertices1 - 1;
		else
			index = bestEdge + 1 < numVertices1 ? bestEdge + 1 : 0;
		
		d = findMinSeperation(poly1, polyxForm1, poly2, polyxForm2, normals[index], index);
		
		if (d > minDistance)
		{
			bestEdge = index;
			minDistance = d;
		}
		else
		{
			break;
		}
	}
	edgeIndex = bestEdge;
	return minDistance < 0.0f;
}

#endif


// Find Whether two Regular/ConvexPolygons intersect.
dbool intersectPolygons( ConvexPolygon* poly1, Matrix3f& xform1, ConvexPolygon* poly2, Matrix3f& xform2);


// Find Whether two Regular/ConvexPolygons intersect. Also find the Contact Points if the Polygons intersect.
dbool intersectPolygons( ConvexPolygon* poly1, Matrix3f& xform1, ConvexPolygon* poly2, Matrix3f& xform2, ContactManifold* contactManifold);

END_NAMESPACE_DIRAC2D

#endif