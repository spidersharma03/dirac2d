/*
 *  Contact.h
 *  Dirac2D
 *
 *  Created by Prashant on 23/04/12.
 *
 */

#include "../../definitions.h"
#include "../../Settings.h"
#include "../../maths/MathUtil.h"


#ifndef _CONTACT_H_
#define _CONTACT_H_

BEGIN_NAMESPACE_DIRAC2D

class PhysicalShape;
class CollisionShape;


// Pair 
struct ContactPair
{
	ContactPair( CollisionShape* shape1, CollisionShape* shape2 )
	{
		if( shape1 < shape2 )
		{
			m_Shape1 = shape1;
			m_Shape2 = shape2;
		}
		else 
		{
			m_Shape1 = shape2;
			m_Shape2 = shape1;
		}

	}
	CollisionShape* m_Shape1;
	CollisionShape* m_Shape2;
};


inline bool operator < (const ContactPair& p1, const ContactPair& p2)
{
	if (p1.m_Shape1 < p2.m_Shape1)
		return true;
	
	if (p1.m_Shape1 == p2.m_Shape1 && p1.m_Shape2 < p2.m_Shape2)
		return true;
	
	return false;
}


// Contact ID's are for comparing contacts. used for caching contacts. 
enum ContactType { ECT_VERTEX, ECT_EDGE };
union ContactID
{
	ContactID()
	{
		m_Key = 0;
	}
	struct EdgeIndex
	{
		duchar m_Type1; // Type of Contact on Shape 1.( Vertex/Edge )
		duchar m_Type2; // Type of Contact on Shape 2.( Vertex/Edge )
		duchar m_Index1; // Edge Index1.
		duchar m_Index2; // Edge Index2.
	};
	EdgeIndex edgeIndex;
	duint32 m_Key;
};

struct ContactPoint
{
	void operator=( ContactPoint other )
	{
		m_Point = other.m_Point;
		m_Depth = other.m_Depth;
		m_ID    = other.m_ID;
	}
	
	Vector2f m_Point;
	dfloat m_Depth;
	ContactID m_ID;
};

struct ContactManifold
{
	ContactManifold()
	{
		m_bFlipShapes = false;
	}
	
	void operator=( ContactManifold other )
	{
		m_ContactPoints[0] = other.m_ContactPoints[0];
		m_ContactPoints[1] = other.m_ContactPoints[1];
		m_ContactNormal    = other.m_ContactNormal;
		m_NumContacts      = other.m_NumContacts;
		m_bFlipShapes	   = other.m_bFlipShapes;
	}
	
	ContactPoint m_ContactPoints[MAX_CONTACTS];
	Vector2f m_ContactNormal;
	dchar m_NumContacts;
	dchar m_bFlipShapes;
};


// This Structure holds the necessary data for each contact point.
struct ContactConstraint
{
	ContactConstraint()
	{
		m_NormalImpulse = 0.0f;
		m_TangentImpulse = 0.0f;
		m_VelocityBias = 0.0f;
		m_PositionError = 0.0f;
	}
	dfloat m_NormalImpulse;
	dfloat m_TangentImpulse;
	
	dfloat m_VelocityBias;
	dfloat m_PositionError;
};

class Contact;

struct ContactEdge
{
	Contact* contact;
	ContactEdge* m_Next;
	ContactEdge* m_Prev;
};

class Contact
{
public:
	Contact()
	{
		m_ERP = 0.2f;
	}
	
	void operator= ( Contact& other )
	{
		m_Manifold = other.m_Manifold;
		m_ContactPoint[0] = other.m_ContactPoint[0];
		m_ContactPoint[1] = other.m_ContactPoint[1];
		m_ContactNormal = other.m_ContactNormal;
		
		m_PhysicalShape1 = other.m_PhysicalShape1;
		m_PhysicalShape2 = other.m_PhysicalShape2;
		
		//m_NormalMassMatrix = other.m_NormalMassMatrix;
		//m_FrictionMassMatrix = other.m_FrictionMassMatrix;
		m_NormalMass[0] = other.m_NormalMass[0];
		m_NormalMass[1] = other.m_NormalMass[1];
		m_FrictionalMass[0] = other.m_FrictionalMass[0];
		m_FrictionalMass[1] = other.m_FrictionalMass[1];
		
		m_ERP = other.m_ERP;
	}
	
	Contact(const Contact& other)
	{
		m_Manifold = other.m_Manifold;
		m_ContactPoint[0] = other.m_ContactPoint[0];
		m_ContactPoint[1] = other.m_ContactPoint[1];
		m_ContactNormal = other.m_ContactNormal;
		
		m_PhysicalShape1 = other.m_PhysicalShape1;
		m_PhysicalShape2 = other.m_PhysicalShape2;
		
		//m_NormalMassMatrix = other.m_NormalMassMatrix;
		//m_FrictionMassMatrix = other.m_FrictionMassMatrix;
		m_NormalMass[0] = other.m_NormalMass[0];
		m_NormalMass[1] = other.m_NormalMass[1];
		m_FrictionalMass[0] = other.m_FrictionalMass[0];
		m_FrictionalMass[1] = other.m_FrictionalMass[1];
		m_ERP = other.m_ERP;
	}
	
	void update();
	
public:
	ContactManifold m_Manifold;
	ContactPoint m_ContactPoint[MAX_CONTACTS];
	Vector2f     m_ContactNormal;
	
	// Colliding Shapes
	PhysicalShape* m_PhysicalShape1;
	PhysicalShape* m_PhysicalShape2;
	CollisionShape* m_CollisionShape1;
	CollisionShape* m_CollisionShape2;
	
	// Effective mass matrix for Normal impulses. if the number of contacts are one then first element is used. if it is two then based upon the solution strategy the full matrix or the 
	// first two elements are used.
	//Matrix2f m_NormalMassMatrix;
	dfloat m_NormalMass[MAX_CONTACTS];
	// Effective mass matrix for frictional impulses.if the number of contacts are one then first diagonal element is used. if it is two then first two elements are used.
	//Matrix2f m_FrictionMassMatrix;
	dfloat m_FrictionalMass[MAX_CONTACTS];

	ContactConstraint m_ContactConstraint[MAX_CONTACTS];
	dchar m_NumContactConstraints;
	
	// Friction due to the Physical Shapes Attached to this Contact.
	dfloat m_Friction;
	// Error reduction parameter. this is used to make sure that the constarint dosent drift over time. should be less than 1.0f/timeStep.
	dfloat m_ERP;
	
	Contact* m_Prev;
	Contact* m_Next;
	
	ContactEdge m_ContactEdge1;
	ContactEdge m_ContactEdge2;
};


END_NAMESPACE_DIRAC2D

#endif