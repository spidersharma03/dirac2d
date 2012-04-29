/*
 *  RegularPolygon.h
 *  Dirac2D
 *
 *  Created by Prashant on 24/04/12.
 *
 */

#ifndef _REGULAR_POLYGON_H_
#define _REGULAR_POLYGON_H_

BEGIN_NAMESPACE_DIRAC2D

class RegularPolygon : public CollisionShape
{
public:
	RegularPolygon();
	
	RegularPolygon(const RegularPolygon& other);
	
	virtual dfloat getArea();
	
	virtual Vector2f getSupportPoint(Vector2f& d);

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
	
protected:
	dint32 m_NumVertices;
	Vector2f m_Vertices[MAX_POLY_VERTICES];
	dfloat m_Radius;
};

END_NAMESPACE_DIRAC2D

#endif