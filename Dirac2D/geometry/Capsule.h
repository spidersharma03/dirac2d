/*
 *  Capsule.h
 *  Dirac2D
 *
 *  Created by Prashant on 24/04/12.
 *
 */

#ifndef _CAPSULE_H_
#define _CAPSULE_H_

#include "CollisionShape.h"

BEGIN_NAMESPACE_DIRAC2D

class Capsule : public CollisionShape
{
public:
	Capsule(dfloat radius, dfloat height);
	
	Capsule(const Capsule& other);
		
	virtual Vector2f getSupportPoint(Vector2f& d);

	virtual dbool isPointInside(Point2f& p);
	
	virtual void updateAABB(Matrix3f& xForm);
	
protected:
	
	virtual void findCentroid();
	
protected:
	
	dfloat m_Radius;
	dfloat m_Height;
};

END_NAMESPACE_DIRAC2D

#endif