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


#ifndef _COLLISION_DETECTION_POLYGON_H_
#define _COLLISION_DETECTION_POLYGON_H_

BEGIN_NAMESPACE_DIRAC2D


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////         POLYGON COLLISION TESTS      ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Project both the Polygons on to the list of seperating axis of Polygon 1. if there is an Axis which seperates the Polygons, then return.
static dbool findSeperationAxis(RegularPolygon* poly1, Matrix3f& polyxForm1, RegularPolygon* poly2, Matrix3f& polyxForm2, dfloat& minDistance, Vector2f& collisionNormal )
{
	dint32 numVertices1 = poly1->getNumVertices();
	dint32 numVertices2 = poly2->getNumVertices();

	Vector2f* vertices1 = poly1->getVertices();
	Vector2f* vertices2 = poly2->getVertices();
	
	Vector2f* normals = poly1->getNormals();
	
	Vector2f v1 = vertices1[0];
	Vector2f v2 = vertices1[0];
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


// Find Whether two Regular/ConvexPolygons intersect.
dbool intersectPolygons( RegularPolygon* poly1, Matrix3f& xform1, RegularPolygon* poly2, Matrix3f& xform2);


// Find Whether two Regular/ConvexPolygons intersect. Also find the Contact Points if the Polygons intersect.
dbool intersectPolygons( RegularPolygon* poly1, Matrix3f& xform1, RegularPolygon* poly2, Matrix3f& xform2, ContactManifold* contactManifold);

END_NAMESPACE_DIRAC2D

#endif