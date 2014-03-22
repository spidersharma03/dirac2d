/*
 *  PhysicalAppearance.h
 *  Dirac2D
 *
 *
 */

#include "../definitions.h"
#include "../Settings.h"
#include "../maths/Vector2.h"
#include "../geometry/CollisionShape.h"

#ifndef _PHYSICAL_APPEARANCE_H_
#define _PHYSICAL_APPEARANCE_H_

BEGIN_NAMESPACE_DIRAC2D

class CollisionShape;

/*
 All the Attributes related to the Physical Shape are declared. PhysicalAppearance is a Collection of all the Attributes.
 PhysicalAttributes contains all the attributes related to PhysicalShape. eg friction, elasticity.
 CollisionAttributes contains Shape data and Collision Filters
 MassAttributes contains mass, moment of inertia, density and centre of mass of the PhysicalShape.
 */

// This Structure Contains all the physical attributes to construct a Physical Shape
struct PhysicalAttributes
{
	Vector2f m_Position;
	dfloat m_Angle;
	dfloat m_Friction;
	dfloat m_Restitution;
	
	PhysicalAttributes()
	{
		m_Angle = 0.0f;
		m_Friction = 0.4f;
		m_Restitution = 0.0f;
	}
};

struct CollisionFilter
{
	CollisionFilter()
	{
		m_CollisionBit = 0xFFFFFFFF;
		m_CollisionMask = 0x0;
	}
	
	CollisionFilter( const CollisionFilter& other)
	{
		m_CollisionBit  = other.m_CollisionBit;
		m_CollisionMask = other.m_CollisionMask;
	}
	
	duint32 m_CollisionBit;
	duint32 m_CollisionMask;
};

//enum COLLISION_SHAPE_TYPE { ECOL_SHAPE_CIRCLE, ECOL_SHAPE_CAPSULE, ECOL_SHAPE_EDGE, ECOL_SHAPE_POLY, ECOL_SHAPE_EDGE_CHAIN }; 

struct CollisionShapeInfo
{
    SHAPE_TYPE m_ColShapeType;
};

struct CircleInfo : public CollisionShapeInfo
{
	CircleInfo(dfloat radius) : m_Radius ( radius )
	{
        m_ColShapeType = EST_CIRCLE;
	}
	dfloat m_Radius;
};

struct CapsuleInfo : public CollisionShapeInfo
{
	CapsuleInfo(dfloat radius, dfloat height) : m_Radius ( radius ), m_Height( height )
	{
        m_ColShapeType = EST_CAPSULE;
	}
	dfloat m_Radius, m_Height;
};

struct PolygonInfo : public CollisionShapeInfo
{
	PolygonInfo(Vector2f* vertices, dint32 numVertices):m_NumVertices( numVertices )
	{
        m_ColShapeType = EST_REGULARPOLY;
        for( dint32 i=0; i<m_NumVertices; i++ )
        {
            m_Vertices[i] = vertices[i];
        }
	}
	Vector2f m_Vertices[MAX_POLY_VERTICES];
    dint32 m_NumVertices;
};

struct EdgeInfo : public CollisionShapeInfo
{
	EdgeInfo(Vector2f v1, Vector2f v2) : m_Vertex1 ( v1 ), m_Vertex2( v2 )
	{
        m_ColShapeType = EST_EDGE;
	}
	Vector2f m_Vertex1, m_Vertex2;
};

struct EdgeChainInfo : public CollisionShapeInfo
{
	EdgeChainInfo(Vector2f* vertices, dint32 numVertices):m_NumVertices( numVertices )
	{
        m_ColShapeType = EST_EDGE_CHAIN;
        m_Vertices = vertices;
	}
	Vector2f *m_Vertices;
    dint32 m_NumVertices;
};

struct CollisionAttributes
{
    CollisionShapeInfo *m_CollisionShapeInfo;
	CollisionFilter m_Filter;
	
	CollisionAttributes()
	{
        m_CollisionShapeInfo = 0;
	}
};

struct MassAttributes
{
	dfloat m_Mass;
	dfloat m_I;
	dfloat m_Density;
	Vector2f m_C;
	
	MassAttributes()
	{
		m_Density = 100.0f;
		m_Mass = m_I = 0.0f;
	}
};

class PhysicalAppearance
{
public:
	PhysicalAppearance()
	{
	}
		
public:
	PhysicalAttributes m_PhysicalAttributes;
	CollisionAttributes m_CollisionAttributes;
	MassAttributes m_MassAttributes;
};

END_NAMESPACE_DIRAC2D

#endif