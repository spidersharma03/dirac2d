/*
 *  CollisionDetectionCirclePolygon.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 22/05/12.
 *
 */

#include "CollisionDetectionCirclePolygon.h"

BEGIN_NAMESPACE_DIRAC2D

// Find Whether two Regular/ConvexPolygons intersect.
dbool intersectCircles( Circle* circle1, Matrix3f& xform1, Circle* circle2, Matrix3f& xform2)
{
	Vector2f c1 = xform1 * circle1->m_Centroid;
	Vector2f c2 = xform2 * circle2->m_Centroid;
	
	dfloat d2 = c1.distanceSquared(c2);
	dfloat r2 = (circle1->m_Radius + circle2->m_Radius);
	r2 *= r2;
	
	return d2 > r2;
}

dbool intersectCircles( Circle* circle1, Matrix3f& xform1, Circle* circle2, Matrix3f& xform2, ContactManifold* contactManifold)
{
	Vector2f c1 = xform1 * circle1->m_Centroid;
	Vector2f c2 = xform2 * circle2->m_Centroid;
	
	dfloat d2 = c1.distanceSquared(c2);
	dfloat r2 = (circle1->m_Radius + circle2->m_Radius);
	r2 *= r2;
	
	if( d2 > r2 )
	{
		contactManifold->m_NumContacts = 0;
		return false;
	}
	
	contactManifold->m_ContactNormal = c1 - c2;
	contactManifold->m_ContactNormal.normalize();
	contactManifold->m_NumContacts = 1;
	c1 -= contactManifold->m_ContactNormal * circle1->m_Radius;
	c2 += contactManifold->m_ContactNormal * circle2->m_Radius;
	
	contactManifold->m_ContactPoints[0].m_Point = ( c1 + c2 ) * 0.5f;
	contactManifold->m_ContactPoints[0].m_Depth = -Vector2f::distance(c2,c1)*0.5f;
	contactManifold->m_ContactPoints[0].m_ID.m_Key = 0;
	
	return true;
}

// Find Whether  Circle/ConvexPolygons intersect. Also find the Contact Points if the Polygons intersect.
dbool intersectCirclePolygon( Circle* circle, Matrix3f& xform1, RegularPolygon* poly, Matrix3f& xform2)
{
	return false;
}

dbool intersectCirclePolygon( Circle* circle, Matrix3f& xform1, RegularPolygon* poly, Matrix3f& xform2, ContactManifold* contactManifold)
{
	// Transform the Circle in the local coordinate frame of the Polygon.
	Vector2f c = xform1 * circle->m_Centroid;
	c *= xform2;
	
	dint32 numVertices = poly->m_NumVertices;
	Vector2f* vertices = poly->m_Vertices;
	Vector2f* normals  = poly->m_Normals;
	
	dfloat radius = circle->m_Radius;
	
	// Find whether the closest distance of circle's centre from any of the Edge is less than the radius of the circle.
	dint32 index = -1;
	dfloat minDistance = -100000.0f;
	for( dint32 v=0; v<numVertices; v++ )
	{
		dfloat distance = (c - vertices[v]).dot(normals[v]);
		if( distance > radius )
			return false;
		if( minDistance < distance )
		{
			minDistance = distance;
			index = v;
		}
	}
	
	Vector2f v1 = vertices[index];
	dint32 newIndex = index < numVertices-1 ? index+1 : 0;
	Vector2f v2 = vertices[newIndex];
	
	// Circle Centre inside Polygon
	if( minDistance < EPSILON )
	{
		contactManifold->m_NumContacts = 1;
		contactManifold->m_ContactNormal = normals[index];
		contactManifold->m_ContactPoints[0].m_Point = (c + contactManifold->m_ContactNormal * -minDistance ) * 0.5f;
		xform2.transformAsPoint(contactManifold->m_ContactPoints[0].m_Point);
		contactManifold->m_ContactPoints[0].m_Depth = minDistance;
		xform2.transformAsVector(contactManifold->m_ContactNormal);		
		return true;
	}
	
	Vector2f edge = v2 - v1;
	/*
		|          |
	R2	|    R3    |		R1
	   v2|----------|v1 
		|          |
	 */
	// Find out the voronoi region in which the circle centre lies.
	dfloat u1 = (c-v1).dot(edge);
	edge = -edge;
	dfloat u2 = (c-v2).dot(edge);
	
	// Left of Edge vertex v1. so v1 is the closest point from the centre of the circle. Region 1.
	if( u1 < 0.0f )
	{
		if( c.distanceSquared(v1) > radius*radius )
		{
			return false;
		}
		contactManifold->m_NumContacts = 1;
		contactManifold->m_ContactNormal = (c - v1);
		contactManifold->m_ContactNormal.normalize();
		c -= contactManifold->m_ContactNormal * radius;
		contactManifold->m_ContactPoints[0].m_Point = (c + v1 ) * 0.5f;
		xform2.transformAsPoint(contactManifold->m_ContactPoints[0].m_Point);
		contactManifold->m_ContactPoints[0].m_Depth = -(c - v1 ).length() * 0.5f;
		xform2.transformAsVector(contactManifold->m_ContactNormal);
		return true;
	}
	// Left of Edge vertex v2. so v2 is the closest point from the centre of the circle. Region 2.
	else if( u2 < 0.0f )
	{
		if( c.distanceSquared(v2) > radius*radius )
		{
			return false;
		}
		contactManifold->m_NumContacts = 1;
		contactManifold->m_ContactNormal = (c - v2);
		contactManifold->m_ContactNormal.normalize();
		c -= contactManifold->m_ContactNormal * radius;
		contactManifold->m_ContactPoints[0].m_Point = (c + v2 ) * 0.5f;
		xform2.transformAsPoint(contactManifold->m_ContactPoints[0].m_Point);
		contactManifold->m_ContactPoints[0].m_Depth = -(c - v2 ).length() * 0.5f;
		xform2.transformAsVector(contactManifold->m_ContactNormal);
		return true;
		
	}
	// Circle Centre lies inside Reigion 3.
	else 
	{
		dfloat shortestDistance = (c-v1).dot(normals[index]);
		if( shortestDistance > radius )
		{
			return false;
		}
		contactManifold->m_NumContacts = 1;
		contactManifold->m_ContactNormal = normals[index];
		xform2.transformAsVector(contactManifold->m_ContactNormal);
		Vector2f cWorld = xform1 * circle->m_Centroid;
		Vector2f closestPoint = cWorld - contactManifold->m_ContactNormal * shortestDistance;
		cWorld -= contactManifold->m_ContactNormal * radius;
		contactManifold->m_ContactPoints[0].m_Point = (cWorld + closestPoint ) * 0.5f;
		contactManifold->m_ContactPoints[0].m_Depth = -(radius - shortestDistance) * 0.5f;
		return true;
		
	}

	return true;
}
END_NAMESPACE_DIRAC2D