/*
 *  EdgeChain.h
 *  Dirac2D
 *
 *  Created by Prashant on 28/05/12.
 *
 */

#ifndef _EDGE_CHAIN_H_
#define _EDGE_CHAIN_H_

#include "CollisionShape.h"

BEGIN_NAMESPACE_DIRAC2D

class EdgeChain : public CollisionShape
{
public:
	EdgeChain(Vector2f* vertices);
	
	EdgeChain(const EdgeChain& other);
	
	virtual Vector2f getSupportPoint(Vector2f& d);
	
	virtual dbool isPointInside(Point2f& p);
	
	virtual void updateAABB(Matrix3f& xForm);
	
protected:
	
	virtual void updateShape(Matrix3f& xForm);
	
	virtual void findCentroid();
	
	virtual void findMomentOfInertia();
	
public:
	Vector2f* m_Vertices;
	Vector2f* m_Normals;
};

END_NAMESPACE_DIRAC2D

#endif