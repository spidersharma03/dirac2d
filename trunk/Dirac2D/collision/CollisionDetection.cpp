/*
 *  CollisionDetection.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 30/04/12.
 *
 */

#include "CollisionDetection.h"
#include "../geometry/CollisionShape.h"
#include "CollisionDetectionPolygon.h"
#include "CollisionDetectionCirclePolygon.h"
#include "CollisionDetectionCapsulePolygon.h"

BEGIN_NAMESPACE_DIRAC2D


// Find Whether two Shapes intersect.
dbool intersectShapes( CollisionShape* shape1, Matrix3f& xform1, CollisionShape* shape2, Matrix3f& xform2)
{
	return false;
}

// Find Whether two Shapes intersect. Also find the Contact Points if the Shapes intersect.
dbool intersectShapes( CollisionShape* shape1, Matrix3f& xform1, CollisionShape* shape2, Matrix3f& xform2, ContactManifold* contactManifold)
{
	if( shape1->getShapeType() == EST_REGULARPOLY && shape2->getShapeType() == EST_REGULARPOLY )
		return intersectPolygons((RegularPolygon*)shape1, xform1, (RegularPolygon*)shape2, xform2, contactManifold);
	
	if( (shape1->getShapeType() == EST_REGULARPOLY && shape2->getShapeType() == EST_CIRCLE) ) 
	{
		contactManifold->m_bFlipShapes = true;
		return intersectCirclePolygon((Circle*)shape2, xform2, (RegularPolygon*)shape1, xform1, contactManifold);
	}

	if( (shape1->getShapeType() == EST_CIRCLE && shape2->getShapeType() == EST_REGULARPOLY) )
		return intersectCirclePolygon((Circle*)shape1, xform1, (RegularPolygon*)shape2, xform2, contactManifold);
	
	if( shape1->getShapeType() == EST_CIRCLE && shape2->getShapeType() == EST_CIRCLE )
		return intersectCircles((Circle*)shape1, xform1, (Circle*)shape2, xform2, contactManifold);
	
	if( shape1->getShapeType() == EST_CAPSULE && shape2->getShapeType() == EST_REGULARPOLY )
	{
		contactManifold->m_bFlipShapes = true;
		return intersectCapsulePolygon((RegularPolygon*)shape2, xform2, (Capsule*)shape1, xform1, contactManifold);
	}
	
	if( shape1->getShapeType() == EST_REGULARPOLY && shape2->getShapeType() == EST_CAPSULE )
		return intersectCapsulePolygon((RegularPolygon*)shape1, xform1, (Capsule*)shape2, xform2, contactManifold);
	
	dAssert(0);
	return false;
}

END_NAMESPACE_DIRAC2D