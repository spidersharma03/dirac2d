/*
 *  CollisionDetection.h
 *  Dirac2D
 *
 *  Created by Prashant on 30/04/12.
 *
 */

#include "../definitions.h"
#include "../maths/MathUtil.h"

#ifndef _COLLISION_DETECTION_H_
#define _COLLISION_DETECTION_H_


BEGIN_NAMESPACE_DIRAC2D

struct ContactManifold;
class CollisionShape;

// Find Whether two Shapes intersect.
dbool intersectShapes( CollisionShape* poly1, Matrix3f& xform1, CollisionShape* poly2, Matrix3f& xform2);


// Find Whether two Shapes intersect. Also find the Contact Points if the Shapes intersect.
dbool intersectShapes( CollisionShape* shape1, Matrix3f& xform1, CollisionShape* shape2, Matrix3f& xform2, ContactManifold* contactManifold);

END_NAMESPACE_DIRAC2D

#endif
