/*
 *  Box.h
 *  Dirac2D
 *
 *  Created by Prashant on 25/04/12.
 *
 */
#include "CollisionShape.h"


#ifndef _BOX_H_
#define _BOX_H_


BEGIN_NAMESPACE_DIRAC2D

class Box : public CollisionShape
{
public:
	Box(dfloat width, dfloat height);
	
	Box(const Box& other);
		
	// For GJK/EPA 
	virtual Vector2f getSupportPoint(Vector2f& d);
	
	virtual dbool isPointInside(Point2f& p);
	
	virtual void updateAABB(Matrix3f& xForm);
	
protected:
	
	virtual void findCentroid();
	
protected:
	dfloat m_Width, m_Height;
};

END_NAMESPACE_DIRAC2D

#endif