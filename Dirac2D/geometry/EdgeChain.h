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

class Edge;

class EdgeChain : public CollisionShape
{
public:
	EdgeChain(Vector2f* vertices);
	
	EdgeChain(const EdgeChain& other);
	
	virtual Vector2f getSupportPoint(Vector2f& d);
	
	virtual dbool isPointInside(Point2f& p);
	
	virtual void updateAABB(Matrix3f& xForm);
	
	Edge* getEdge(dint32 index);
	
protected:
	
	virtual void updateShape(Matrix3f& xForm);
	
	virtual void findCentroid();
	
	virtual void findMomentOfInertia();
	
public:
	Vector2f* m_Vertices;
	Vector2f* m_Normals;
	
protected:
	Edge* m_EdgeList;
};

END_NAMESPACE_DIRAC2D

#endif