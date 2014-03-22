//
//  CollisionShapeSoftBody.h
//  Dirac2D
//
//  Created by Prashant Sharma on 25/05/13.
//

#ifndef _COLLISION_SHAPE_SOFT_BODY_H_
#define _COLLISION_SHAPE_SOFT_BODY_H_


#include "CollisionShape.h"
#include "Edge.h"

BEGIN_NAMESPACE_DIRAC2D

class Edge;

class CollisionShapeSoftBody : public CollisionShape
{
public:
    virtual Vector2f getSupportPoint(const Vector2f& d);
	
	virtual Vector2f getSupportPointWithoutRadius(const Vector2f& d);
    
	virtual dbool isPointInside(Vector2f& p);
	
	virtual dbool intersectRaySegment(const Matrix3f& xForm, const RaySegment2f& raySeg, RayIntersectionInfo& intersectInfo);
    
	virtual void updateAABB(Matrix3f& xForm);
	
	Edge* getEdge(dint32 index) const;
    
	inline dint32 getNumEdges() const
	{
		return m_NumEdges;
	}
	
	inline dbool isClosed() const
	{
		return ( m_EdgeList->m_Vertex1 == (m_EdgeList+m_NumEdges)->m_Vertex1
				&& (m_EdgeList->m_Vertex2 == (m_EdgeList+m_NumEdges)->m_Vertex2) );
	}
	
    virtual ~CollisionShapeSoftBody()
    {
        delete [] m_EdgeList;
    }
    
    friend class PhysicalBody;
    
protected:
    
    CollisionShapeSoftBody(Vector2f* vertices, dint32 numVertices);
	
	CollisionShapeSoftBody(const CollisionShapeSoftBody& other);
	
	void operator= ( CollisionShapeSoftBody& other);
    
    virtual CollisionShape* clone(MemoryBlockAllocator* pAllocator);
    
	virtual void updateShape(Matrix3f& xForm);
	
	virtual void findCentroid();
	
	virtual void findMomentOfInertia();
	
public:
	Vector2f* m_Normals;
	
protected:
    
    Vector2f m_Positions[MAX_POLY_VERTICES];
    Vector2f m_Velocities[MAX_POLY_VERTICES];
    
	dint32 m_NumEdges;
	Edge* m_EdgeList;
};

END_NAMESPACE_DIRAC2D

#endif
