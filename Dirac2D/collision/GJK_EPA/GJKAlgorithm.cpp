//
//  GJKAlgorithm.cpp
//  GJK2D
//
//  Created by Prashant on 21/07/11.
//

#include "GJKAlgorithm.h"
#include "../../geometry/CollisionShape.h"

BEGIN_NAMESPACE_DIRAC2D

bool bStepByStep = true;
int counter = 0;

GJKAlgorithm* GJKAlgorithm::m_Instance = 0;
int GJKAlgorithm::numIterations = 0;

bool GJKAlgorithm::pointInsideShape( CollisionShape& polygon, Vector2f& p )
{
	// 1. Create Simplex 0 using any point in Polygon
	Simplexf s;
	Vector2f dir(1.0f,0.0f);
	Vector2f sp = polygon.getSupportPoint(dir);
	s.addPoint( sp );
	
	// Get support point in the direction towards the test point
	Vector2f d = p;
	d -= s.m_simplexPoints[0];
	static Vector2f temp;

	int counter = 0;
	
	while (true)
	{		
		//s.render();
		
		Vector2f supportPoint;
		
		// Get a new Support Point.
		supportPoint = polygon.getSupportPoint(d);
		
		// If we get a support point which is on the same side of the first point, return false
		Vector2f a;
		
		a = supportPoint;
		
		a -= p;
		
		if( a.dot(d) < 0.0f )	
			return false;
		
		// Add the support point to evolve the Simplex
		s.addPoint(supportPoint);
		counter++;
		if ( s.process(d, p) )
			return true;
	}
}

dbool GJKAlgorithm::pointInsideShapeStepWise( CollisionShape& polygon, Vector2f& p )
{	
		Vector2f supportPoint;
		
		// Get a new Support Point.
		supportPoint = polygon.getSupportPoint(direction);
		
		// If we get a support point which is on the same side of the first point, return false
		Vector2f a = supportPoint;
		a -= p;
	
		if( a.dot(direction) < 0.0f )	
			return false;
		
		// Add the support point to evolve the Simplex
		simplex.addPoint(supportPoint);
		
		// Process the Simplex
		if ( simplex.process(direction, p) )
			return true;
	
	return false;
}

dbool GJKAlgorithm::intersectShapes( CollisionShape* polygon1, Matrix3f m1, CollisionShape* polygon2, Matrix3f m2 )
{
	// 1. Create Simplex 0 using any point in the Minkowski difference of polygon 1 and 2.
	Simplexf s;
	
	Vector2f point;
    Vector2f dir(1.0f,0.0f);
	Vector2f point1 = polygon1->getSupportPoint(dir);
	Vector2f point2 = polygon2->getSupportPoint(dir);
	
	// Transform the support point 2 into Polygon1's local space
	Matrix3f m1Inv;
	Matrix3f m2Inv;
	m1.getInverse(m1Inv);
	m2.getInverse(m2Inv);
	
	m1 = m2Inv * m1;
	m2 = m1Inv * m2;
	
	m2.transformAsPoint(point2);
	
	point = point1 - point2;
	
	s.addPoint( point );
	
	// Get support point in the direction towards the origin
	Vector2f d;
	d -= point;
	
	while (true)
	{				
		Vector2f supportPoint, supportPoint1, supportPoint2;
		
		// Get a new Support Point in the Minkowski difference.
		supportPoint1 = polygon1->getSupportPoint(d);
		Vector2f negd = -d;
		// Transform the search direction into Polygon2's local space
		m1.transformAsVector(negd);
		supportPoint2 = polygon2->getSupportPoint(negd);
		
		// Transform the support point 2 into Polygon1's space
		m2.transformAsPoint(supportPoint2);
		
		supportPoint = supportPoint1 -  supportPoint2;
		
		// If we get a support point which is on the same side of the origin, return false
		Vector2f a = supportPoint;
		
		if( a.dot(d) < 0.0f )	
			return false;
		
		// Add the support point to evolve the Simplex
		s.addPoint(supportPoint);
		// Process the Simplex
		Vector2f origin;
		if ( s.process(d, origin) )
			return true;
	}
}

dbool GJKAlgorithm::intersectShapes( CollisionShape& polygon1, Matrix3f& m1, CollisionShape& polygon2, Matrix3f& m2, Simplexf& simplex )
{
	// 1. Create Simplex 0 using any point in the Minkowski difference of polygon 1 and 2.
	Simplexf s;
	
	Vector2f point;
	Vector2f point1;// =  *polygon1.m_vecVertices[0];
	Vector2f point2;// =  *polygon2.m_vecVertices[0];
	Vector2f dir(1.0f,0.0f);
	point1 = polygon1.getSupportPoint(dir);
	point2 = polygon2.getSupportPoint(dir);
	
	
	// Transform the support point 2 into Polygon1's local space
	//Matrix3f m1 = polygon1.m_Transform;
	//Matrix3f m2 = polygon2.m_Transform;
	
	Matrix3f m1Inv;
	Matrix3f m2Inv;
	m1.getInverse(m1Inv);
	m2.getInverse(m2Inv);

	m1.multiply(m2Inv, m1);
	m2.multiply(m1Inv, m2);
	m2.transformAsPoint(point2);

	point.x = point1.x - point2.x;
	point.y = point1.y - point2.y;

	s.addPoint( point );
	s.addPoint1( point1 );
	s.addPoint2( point2 );

	simplex = s;

	// Get support point in the direction towards the origin
	Vector2f d;
	d.sub(point);
	numIterations = 0;	
	while (true)
	{		
		numIterations++;
		Vector2f supportPoint, supportPoint1, supportPoint2;
		
		// Get a new Support Point in the Minkowski difference.
		supportPoint1 = polygon1.getSupportPoint(d);
		Vector2f negd = -d;
		// Transform the search direction into Polygon2's local space
		m1.transformAsVector(negd);
		supportPoint2 = polygon2.getSupportPoint(negd);
		
		// Transform the support point 2 into Polygon1's space
		m2.transformAsPoint(supportPoint2);

		supportPoint.x = supportPoint1.x -  supportPoint2.x;
		supportPoint.y = supportPoint1.y -  supportPoint2.y;

		// If we get a support point which is on the same side of the origin, return false
		Vector2f a = supportPoint;

		if( a.dot(d) < 0.0f )	
			return false;
		
		// Add the support point to evolve the Simplex
		s.addPoint(supportPoint);
		s.addPoint1( supportPoint1 );
		m1.transformAsPoint(supportPoint2);
		s.addPoint2( supportPoint2 );
		
		simplex = s;
		// Process the Simplex
		Vector2f origin;
		if ( s.process(d, origin) )
			return true;
	}
}

dbool GJKAlgorithm::intersectShapesStepWise( CollisionShape& polygon1, Matrix3f& m1, CollisionShape& polygon2, Matrix3f& m2 )
{
	Vector2f supportPoint, supportPoint1, supportPoint2;
	
	// Get a new Support Point in the Minkowski difference.
	supportPoint1 = polygon1.getSupportPoint(direction);
	Vector2f negd = -direction;
	supportPoint2 = polygon2.getSupportPoint(negd);
	
	supportPoint = supportPoint1 -  supportPoint2;
	
	// If we get a support point which is on the same side of the origin, return false
	Vector2f a = supportPoint;
	
	if( a.dot(direction) < 0.0f )	
		return false;
	
	// Add the support point to evolve the Simplex
	simplex.addPoint(supportPoint);
	//printf("Counter=%d\n", counter);
	// Process the Simplex
	Vector2f origin;
	if ( simplex.process(direction, origin) )
		return true;
	
	return false;
}


void GJKAlgorithm::initGJKStepWise(CollisionShape& polygon, Vector2f& p)
{
	// 1. Create Simplex 0 using any point in Polygon
	//simplex.addPoint( *polygon.m_vecVertices[0] );
	
	// Get support point in the direction towards the test point
	direction.set(p.x, p.y);
	direction -= simplex.m_simplexPoints[0];
}

END_NAMESPACE_DIRAC2D
