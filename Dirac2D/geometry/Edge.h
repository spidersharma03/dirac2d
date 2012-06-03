/*
 *  Edge.h
 *  Dirac2D
 *
 *  Created by Prashant on 28/05/12.
 *
 */

#ifndef _EDGE_H_
#define _EDGE_H_

#include "CollisionShape.h"

BEGIN_NAMESPACE_DIRAC2D

class Edge : public CollisionShape
{
public:
	Edge();
	
	Edge(Vector2f& vertex1, Vector2f& vertex2);
	
	Edge(const Edge& other);
	
	virtual Vector2f getSupportPoint(Vector2f& d);
	
	virtual dbool isPointInside(Point2f& p);
	
	virtual void updateAABB(Matrix3f& xForm);
	
protected:
	
	virtual void updateShape(Matrix3f& xForm);
	
	virtual void findCentroid();
	
	virtual void findMomentOfInertia();
	
	friend class EdgeChain;
public:
	Vector2f m_Vertex1, m_Vertex2;
	duchar m_bHasPrev, m_bHasNext;
};

END_NAMESPACE_DIRAC2D

#endif