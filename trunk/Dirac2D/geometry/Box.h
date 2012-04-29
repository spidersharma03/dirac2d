/*
 *  Box.h
 *  Dirac2D
 *
 *  Created by Prashant on 25/04/12.
 *
 */

#ifndef _BOX_H_
#define _BOX_H_

#include "CollisionShape.h"

BEGIN_NAMESPACE_DIRAC2D

class Box : public CollisionShape
{
public:
	Box();
	
	Box(const Box& other);
	
	virtual dfloat getArea();
	
	virtual Vector2f getSupportPoint(Vector2f& d);

protected:
	
	virtual void findCentroid();
	
protected:
	dfloat m_Width, m_Height;
};

END_NAMESPACE_DIRAC2D

#endif