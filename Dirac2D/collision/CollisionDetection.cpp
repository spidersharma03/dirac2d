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

BEGIN_NAMESPACE_DIRAC2D


// Find Whether two Shapes intersect.
dbool intersectShapes( CollisionShape* shape1, Matrix3f& xform1, CollisionShape* shape2, Matrix3f& xform2)
{
	return false;
}

// Find Whether two Shapes intersect. Also find the Contact Points if the Shapes intersect.
dbool intersectShapes( CollisionShape* shape1, Matrix3f& xform1, CollisionShape* shape2, Matrix3f& xform2, ContactManifold* contactManifold)
{
	return intersectPolygons((RegularPolygon*)shape1, xform1, (RegularPolygon*)shape2, xform2, contactManifold); 
}

END_NAMESPACE_DIRAC2D