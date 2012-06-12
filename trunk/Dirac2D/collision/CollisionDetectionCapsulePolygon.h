/*
 *  CollisionDetectionCapsulePolygon.h
 *  Dirac2D
 *
 *  Created by Prashant on 23/05/12.
 *
 */

#include <math.h>
#include "../common.h"
#include "../definitions.h"
#include "../Settings.h"
#include "../maths/MathUtil.h"
#include "../geometry/CollisionShape.h"
#include "../geometry/ConvexPolygon.h"
#include "../geometry/Circle.h"
#include "../geometry/Capsule.h"
#include "../dynamics/contacts/Contact.h"

#ifndef _COLLISION_DETECTION_CAPSULE_POLYGON_H_
#define _COLLISION_DETECTION_CAPSULE_POLYGON_H_

BEGIN_NAMESPACE_DIRAC2D

// Find Whether two Capsules intersect.
dbool intersectCapsules( Capsule* capsule1, Matrix3f& xform1, Capsule* capsule2, Matrix3f& xform2);

// Find Whether two Capsules intersect.Also find the Contact Points if the Circles intersect.
dbool intersectCapsules( Capsule* capsule1, Matrix3f& xform1, Capsule* capsule2, Matrix3f& xform2, ContactManifold* contactManifold);



// Find Whether Capsule/Circle intersect. Also find the Contact Points if the Circles intersect.
dbool intersectCapsuleCircle( Circle* circle, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2);

// Find Whether two Circles intersect. Also find the Contact Points if the Circles intersect.
dbool intersectCapsuleCircle( Circle* circle, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2, ContactManifold* contactManifold);


// Find Whether two Capsule/ConvexPolygons intersect.
dbool intersectCapsulePolygon( ConvexPolygon* poly, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2);

// Find Whether two Capsule/ConvexPolygons intersect.
dbool intersectCapsulePolygon( ConvexPolygon* poly, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2, ContactManifold* contactManifold);

END_NAMESPACE_DIRAC2D

#endif
