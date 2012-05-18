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

// Contact ID's are for comparing contacts. used for caching contacts.  
// To make sure that the ID is uniquely defined for the two shapes, 
union ContactID
{
	void operator=( ContactID& other )
	{
	}
	duchar m_Index1; // Index on Physical Shape 1.
	duchar m_Index2; // Index on Physical Shape 2.
	duchar m_Type1; // Type of Contact on Shape 1.( Vertex/Edge )
	duchar m_Type2; // Type of Contact on Shape 2.( Vertex/Edge )
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
	void operator=( ContactManifold other )
	{
		m_ContactPoints[0] = other.m_ContactPoints[0];
		m_ContactPoints[1] = other.m_ContactPoints[1];
		m_ContactNormal    = other.m_ContactNormal;
		m_NumContacts      = other.m_NumContacts;
	}
	
	ContactPoint m_ContactPoints[MAX_CONTACTS];
	Vector2f m_ContactNormal;
	dchar m_NumContacts;
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

class Contact
{
public:
	Contact()
	{
		m_ERP = 0.2f;
		m_CFM = 0.0f;
	}
	
	void operator= ( Contact& other )
	{
		m_Manifold = other.m_Manifold;
		m_ContactPoint[0] = other.m_ContactPoint[0];
		m_ContactPoint[1] = other.m_ContactPoint[1];
		m_ContactNormal = other.m_ContactNormal;
		
		m_PhysicalShape1 = other.m_PhysicalShape1;
		m_PhysicalShape2 = other.m_PhysicalShape2;
		
		m_NormalMassMatrix = other.m_NormalMassMatrix;
		m_FrictionMassMatrix = other.m_FrictionMassMatrix;
		
		m_ERP = other.m_ERP;
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
		
		m_NormalMassMatrix = other.m_NormalMassMatrix;
		m_FrictionMassMatrix = other.m_FrictionMassMatrix;
		
		m_ERP = other.m_ERP;
		m_ERP = other.m_ERP;
	}
	
	void update();
	
public:
	ContactManifold m_Manifold;
	ContactPoint m_ContactPoint[MAX_CONTACT_POINTS];
	Vector2f     m_ContactNormal;
	
	// Colliding Shapes
	PhysicalShape* m_PhysicalShape1;
	PhysicalShape* m_PhysicalShape2;
	
	// Effective mass matrix for Normal impulses. if the number of contacts are one then first element is used. if it is two then based upon the solution strategy the full matrix or the 
	// first two elements are used.
	Matrix2f m_NormalMassMatrix;
	// Effective mass matrix for frictional impulses.if the number of contacts are one then first diagonal element is used. if it is two then first two elements are used.
	Matrix2f m_FrictionMassMatrix;
	
	ContactConstraint m_ContactConstraint[MAX_CONTACT_POINTS];
	dchar m_NumContactConstraints;
	
	// Error reduction parameter. this is used to make sure that the constarint dosent drift over time. should be less than 1.0f/timeStep.
	dfloat m_ERP;
	// Constraint force mixing. this is used to soften the constraint and can be used as a softness parameter.
	dfloat m_CFM;
	
	Contact* m_Prev;
	Contact* m_Next;
	
};

END_NAMESPACE_DIRAC2D

#endif