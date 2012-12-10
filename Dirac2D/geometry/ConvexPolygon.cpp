/*
 *  ConvexPolygon.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 03/05/12.
 *
 */


#include "ConvexPolygon.h"
#include "../common.h"

BEGIN_NAMESPACE_DIRAC2D


ConvexPolygon::ConvexPolygon(Vector2f* vertices, dint32 numVertices) : m_NumVertices(numVertices)
{
	dAssert(m_NumVertices > 2);
	dAssert(m_NumVertices<MAX_POLY_VERTICES);
	
	for( dint32 v=0; v< m_NumVertices; v++ )
		m_Vertices[v] = vertices[v];

	for( dint32 v=0; v< m_NumVertices-1; v++ )
	{
		Vector2f edge = m_Vertices[v+1] - m_Vertices[v];
		m_Normals[v].set(edge.y, -edge.x);
		m_Normals[v].normalize();
	}
	Vector2f edge = m_Vertices[0] - m_Vertices[m_NumVertices-1];
	m_Normals[m_NumVertices-1].set(edge.y, -edge.x);
	m_Normals[m_NumVertices-1].normalize();
	
	m_ShapeType = EST_REGULARPOLY;
	findCentroid();
	findMomentOfInertia();
}

ConvexPolygon::ConvexPolygon(const ConvexPolygon& other) : CollisionShape(other)
{
	m_NumVertices = other.m_NumVertices;
	for( dint32 v=0; v< m_NumVertices; v++ )
		m_Vertices[v] = other.m_Vertices[v];
	
	for( dint32 v=0; v< m_NumVertices; v++ )
		m_Normals[v] = other.m_Normals[v];
	
	m_I = other.m_I;
	m_Centroid = other.m_Centroid;
	m_Radius   = other.m_Radius;
}

void ConvexPolygon::operator=( ConvexPolygon& other)
{
	m_NumVertices = other.m_NumVertices;
	for( dint32 v=0; v< m_NumVertices; v++ )
		m_Vertices[v] = other.m_Vertices[v];
	
	for( dint32 v=0; v< m_NumVertices; v++ )
		m_Normals[v] = other.m_Normals[v];
	
	m_I = other.m_I;
	m_Centroid = other.m_Centroid;
	m_Radius   = other.m_Radius;
}

ConvexPolygon* ConvexPolygon::createRegularPolygon( dint32 numSides, dfloat radius)
{
	dAssert(numSides > 2);
	dAssert(numSides < MAX_POLY_VERTICES);
	Vector2f *vertices = new Vector2f[numSides];
	dfloat angle = 0.0f;
	dfloat dAngle = 2*PI/(numSides);
	dfloat x, y;
	
	for( dint32 v=0; v<numSides; v++ )
	{
		x = radius * cos(angle);
		y = radius * sin(angle);
		vertices[v].x = x;
		vertices[v].y = y;
		angle += dAngle;
	}
	ConvexPolygon* convexPoly = new ConvexPolygon(vertices, numSides);
	delete[] vertices;
	return convexPoly;
}

CollisionShape* ConvexPolygon::clone()
{
	return new ConvexPolygon(*this);
}

Vector2f ConvexPolygon::getSupportPoint(const Vector2f& d)
{
	dfloat max = -10000.0f;
	dint32 pointIndex = 0;
	for( dint32 i=0; i<m_NumVertices; i++ )
	{
		Vector2f& p = m_Vertices[i];
		dfloat proj = p.dot(d);
		if( proj > max )
		{
			max = proj;
			pointIndex = i;
		}
	}
	return m_Vertices[pointIndex];
}

dbool ConvexPolygon::isPointInside(Vector2f& p)
{
	for (dint32 v=0; v<m_NumVertices; v++)
	{
		Vector2f& n = m_Normals[v];
		Vector2f& vertex = m_Vertices[v];
		if( (p - vertex).dot(n) > 0.0f )
			return false;
	}
	return true;
}

/*
  Any Point on the Ray Segment is given by P = P0 + D*t, where D = P1 - P0
  if the Edge of the Polygon is given by vertices V1, V2, and Normal N
  and if the point P on the ray segment lies on the Edge then
   ( P - V1 ) DOT N = 0
   
   solving the above we get
   t = ( V1 - P0 ) DOT N / ( D DOT N )
 
   Also we need to make sure 
 */
dbool ConvexPolygon::intersectRaySegment(const Matrix3f& xForm, const RaySegment2f& raySeg, RayIntersectionInfo& intersectInfo)
{
	// Transform the Ray into Poly's space. this can be done by taking inverse of xForm and then transforming the ray. below implementation is more efficient.
	Matrix3f M = xForm;
	M.col3.x = M.col3.y = 0.0f;
	Vector2f T(xForm.col3.x, xForm.col3.y);
	Vector2f rayStart = ( raySeg.m_Start - T ) * M;
	Vector2f rayEnd   = ( raySeg.m_End - T ) * M;
	Vector2f D = rayEnd - rayStart;
	dfloat tmin = 10000000.0f;
	dfloat tmax = -10000000.0f;
	
	dbool bResult = false;
	dint32 index = -1;
	
	for (int i=0; i<m_NumVertices; i++) 
	{
		dfloat Denom = ( m_Normals[i].dot(D) );
		dfloat Numerator = ( m_Vertices[i] - rayStart ).dot(m_Normals[i]);
		if( Denom * Denom > EPSILON * EPSILON )
		{
			dfloat t = Numerator/Denom;
			// Ray Entering this Half plane defined by the Polygon's Edge.
			if( Denom < 0.0f && t < tmin )
			{
				tmin = t;
				index = i;
			}
			// Ray Exiting this Half plane defined by the Polygon's Edge.
			else if ( Denom > 0.0f && t < tmax )
			{
				tmax = t;
				index = i;
			}
		}
		// Ray can't intersect the Poly.
		if( tmax < tmin )
			return false;
	}
	// if tmin is negative, the Ray starts within the Poly.
	intersectInfo.m_HitT = tmin;
	intersectInfo.m_HitNormal = xForm * m_Normals[index];
	return bResult;
}

void ConvexPolygon::updateAABB(Matrix3f& xForm)
{
	dfloat min_x = 100000.0f;
	dfloat min_y = 100000.0f;

	dfloat max_x = -100000.0f;
	dfloat max_y = -100000.0f;

	for( dint32 v=0; v<m_NumVertices; v++ )
	{
		Vector2f p = m_Vertices[v];
		xForm.transformAsPoint(p);
		if( min_x > p.x )
			min_x = p.x;
		if( min_y > p.y )
			min_y = p.y;
		
		if( max_x < p.x )
			max_x = p.x;
		if( max_y < p.y )
			max_y = p.y;
	}
	
	m_AABB.m_LowerBounds.set(min_x, min_y);
	m_AABB.m_UpperBounds.set(max_x, max_y);
}

void ConvexPolygon::updateShape(Matrix3f& xForm)
{
	for( dint32 v=0; v<m_NumVertices; v++ )
	{
		xForm.transformAsPoint(m_Vertices[v]);
		xForm.transformAsVector(m_Normals[v]);
	}	
	xForm.transformAsPoint(m_Centroid);
}

void ConvexPolygon::findCentroid()
{
	m_Centroid.set(0.0f, 0.0f);
	dint32 i = 0;
	dfloat Area = 0.0f;
	const dfloat ONE_BY_THREE = 1.0f/3.0f;
	
	for( dint32 v=0; v<m_NumVertices; v++ )
	{
		Vector2f v1 = m_Vertices[v];
		i = v+1;
		if( v == m_NumVertices-1 )
			i = 0;
		Vector2f v2 = m_Vertices[i];
		
		// Find Area of the Triangle formed by the above vertices
		Vector2f edge1 = -v1;
		Vector2f edge2 = v2 - v1;
		dfloat A = 0.5f * edge2.cross(edge1);
		// Center of the Triangle
		Vector2f c = ( v1 + v2 ) * ONE_BY_THREE;

		// Weighted Sum ( Center = SUM ( Ai * Ci )/ SUM( Ai )
		Area += A;
		m_Centroid += c * A;
	}
	m_Area = Area;
	dAssert( m_Area > 0.0f );
	m_Centroid /= m_Area;
}

// From wikipedia.
void ConvexPolygon::findMomentOfInertia()
{
	dfloat denom     = 0.0f;
	dfloat numerator = 0.0f;
	
	for( dint32 v=0; v<m_NumVertices-1; v++ )
	{
		Vector2f p1 = m_Vertices[v] - m_Centroid;
		Vector2f p2 = m_Vertices[v+1] - m_Centroid;
		dfloat d1  = p1.dot(p1);
		dfloat d2  = p2.dot(p2);
		dfloat d12 = p1.dot(p2);
		dfloat p1_cross_p2 = fabs(p1.cross(p2));
		denom += (d1 + d2 + d12) * p1_cross_p2;
		numerator += p1_cross_p2;
	}

	m_I = denom/numerator/6.0f;	
}

END_NAMESPACE_DIRAC2D