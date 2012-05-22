/*
 *  RegularPolygon.h
 *  Dirac2D
 *
 *  Created by Prashant on 24/04/12.
 *
 */

#ifndef _REGULAR_POLYGON_H_
#define _REGULAR_POLYGON_H_

#include "CollisionShape.h"

BEGIN_NAMESPACE_DIRAC2D

class RegularPolygon : public CollisionShape
{
public:
	RegularPolygon(Vector2f* vertices, dint32 numVertices);
	
	RegularPolygon(const RegularPolygon& other);
		
	virtual Vector2f getSupportPoint(Vector2f& d);

	virtual dbool isPointInside(Point2f& p);
	
	virtual void updateAABB(Matrix3f& xForm);
	
	inline dint32 getNumVertices()
	{
		return m_NumVertices;
	}
	
	Vector2f* getVertices()
	{
		return m_Vertices;
	}
protected:
	
	virtual void findCentroid();
	
	virtual void findMomentOfInertia();

protected:
	dint32 m_NumVertices;
	Vector2f m_Vertices[MAX_POLY_VERTICES];
	dfloat m_Radius;
};

END_NAMESPACE_DIRAC2D

#endif