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

struct ContactID
{
};

struct ContactPoint
{
	Vector2f m_Point;
	dfloat m_Depth;
	ContactID m_ID;
};

struct ContactManifold
{
	ContactPoint m_ContactPoints[MAX_CONTACTS];
	Vector2f m_ContactNormal;
	dchar m_NumContacts;
};

class Contact
{
public:
	Contact()
	{
		m_ERP = 0.0f;
		m_CFM = 0.0f;
	}
	
	Contact(const Contact& other)
	{
		
	}
	
public:
	ContactManifold m_Manifold;
	
	// Colliding Shapes
	PhysicalShape* m_PhysicalShape1;
	PhysicalShape* m_PhysicalShape2;
	
	// Effective mass matrix for Normal impulses. if the number of contacts are one then first element is used. if it is two then based upon the solution strategy the full matrix or the 
	// first two elements are used.
	Matrix2f m_NormalMassMatrix;
	// Effective mass matrix for frictional impulses.if the number of contacts are one then first diagonal element is used. if it is two then first two elements are used.
	Matrix2f m_FrictionMassMatrix;
	
	dfloat m_NormalImpulse;
	dfloat m_TangentImpulse;
	
	dfloat m_PositionError[MAX_CONTACTS];
	// Error reduction parameter. this is used to make sure that the constarint dosent drift over time. should be less than 1.0f/timeStep.
	dfloat m_ERP;
	// Constraint force mixing. this is used to soften the constraint and can be used as a softness parameter.
	dfloat m_CFM;
};

END_NAMESPACE_DIRAC2D

#endif