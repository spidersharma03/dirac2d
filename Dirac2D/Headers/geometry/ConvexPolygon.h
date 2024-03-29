/*
 *  ConvexPolygon.h
 *  Dirac2D
 *
 *  Created by Prashant on 24/04/12.
 *
 */

#ifndef _CONVEX_POLYGON_H_
#define _CONVEX_POLYGON_H_

#include "CollisionShape.h"

BEGIN_NAMESPACE_DIRAC2D

class MemoryBlockAllocator;
struct ContactManifold;
class Circle;
class Capsule;

class ConvexPolygon : public CollisionShape
{
public:
	virtual Vector2f getSupportPoint(const Vector2f& d);

	virtual Vector2f getSupportPointWithoutRadius(const Vector2f& d);

	virtual dbool isPointInside(Vector2f& p);
	
	virtual void updateAABB(Matrix3f& xForm);
	
	virtual CollisionShape* clone(MemoryBlockAllocator* pAllocator);

	inline dint32 getNumVertices()
	{
		return m_NumVertices;
	}
	
	Vector2f* getVertices()
	{
		return m_Vertices;
	}
	
	Vector2f* getNormals()
	{
		return m_Normals;
	}
	
	virtual dbool intersectRaySegment(const Matrix3f& xForm, const RaySegment2f& raySeg, RayIntersectionInfo& intersectInfo);
	
    friend class PhysicalBody;

protected:
	
    ConvexPolygon(Vector2f* vertices, dint32 numVertices);
	
	ConvexPolygon(const ConvexPolygon& other);
	
	void operator=( ConvexPolygon& other);
    
	virtual void updateShape(Matrix3f& xForm);

	virtual void findCentroid();
	
	virtual void findMomentOfInertia();

	friend dbool intersectCirclePolygon( Circle* circle, Matrix3f& xform1, ConvexPolygon* poly, Matrix3f& xform2);
	friend dbool intersectCirclePolygon( Circle* circle, Matrix3f& xform1, ConvexPolygon* poly, Matrix3f& xform2, ContactManifold* contactManifold);
	friend dbool intersectCapsulePolygon( ConvexPolygon* poly, Matrix3f& xform1, Capsule* capsule, Matrix3f& xform2, ContactManifold* contactManifold);

protected:
	dint32 m_NumVertices;
	Vector2f m_Vertices[MAX_POLY_VERTICES];
	Vector2f m_Normals[MAX_POLY_VERTICES];
	dfloat m_Radius;
};

#define PROJECT_POLYGON( POLYGON, NUMVERTICES, AXIS, MININDEX , MAXINDEX ) \
{ \
Vector2f* VERTICES = POLYGON->getVertices();	\
float dot = AXIS.dot(VERTICES[0]); \
dfloat min = dot; dfloat max = dot; \
for( dint32 i = 1; i < NUMVERTICES; i++ ) \
{ \
dot = AXIS.dot(VERTICES[i]); \
if( dot < min ){ \
min = dot; \
MININDEX = i; \
} \
if( dot > max ){ \
max = dot; \
MAXINDEX = i; \
} \
} \
} \

END_NAMESPACE_DIRAC2D

#endif