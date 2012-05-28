/*
 *  CollisionDtectionEdge.h
 *  Dirac2D
 *
 *  Created by Prashant on 28/05/12.
 *
 */

#include <math.h>
#include "../common.h"
#include "../definitions.h"
#include "../Settings.h"
#include "../maths/MathUtil.h"
#include "../geometry/CollisionShape.h"
#include "../geometry/RegularPolygon.h"
#include "../geometry/Circle.h"
#include "../geometry/Capsule.h"
#include "../geometry/Edge.h"
#include "../dynamics/contacts/Contact.h"

#ifndef _COLLISION_DETECTION_POLYGON_EDGE_H_
#define _COLLISION_DETECTION_POLYGON_EDGE_H_

BEGIN_NAMESPACE_DIRAC2D


// Find Whether Edge/ConvexPolygons intersect.
dbool intersectEdgePolygon( Edge* edge, Matrix3f& xform1, RegularPolygon* poly, Matrix3f& xform2);

// Find Whether Edge/ConvexPolygons intersect. Also find the Contact Points if the Edge/ConvexPolygons intersect.
dbool intersectEdgePolygon( Edge* edge, Matrix3f& xform1, RegularPolygon* poly, Matrix3f& xform2, ContactManifold* contactManifold);



// Find Whether Edge/Circle intersect.
dbool intersectEdgeCircle( Edge* edge, Matrix3f& xform1, Circle* circle, Matrix3f& xform2);

// Find Whether Edge/Circle intersect. Also find the Contact Points if the Edge/Circle intersect.
dbool intersectEdgeCircle( Edge* edge, Matrix3f& xform1, Circle* circle, Matrix3f& xform2, ContactManifold* contactManifold);



// Find Whether Edge/Capsule intersect.
dbool intersectEdgeCapsule( Edge* edge, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2);

// Find Whether Edge/Capsule intersect. Also find the Contact Points if the Edge/Circle intersect.
dbool intersectEdgeCapsule( Edge* edge, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2, ContactManifold* contactManifold);


END_NAMESPACE_DIRAC2D

#endif