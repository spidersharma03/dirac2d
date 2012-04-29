/*
 *  Body.h
 *  Dirac2D
 *
 *  Created by Prashant on 23/04/12.
 *
 */

#include "../definitions.h"
#include "../maths/MathUtil.h"

#ifndef _PHYSICAL_BODY_H_
#define _PHYSICAL_BODY_H_

BEGIN_NAMESPACE_DIRAC2D

class PhysicalWorld;
class PhysicalShape;
class PhysicalAppearance;

class PhysicalBody
{
	enum BODY_TYPE { EBT_DYNAMIC, EBT_STATIC, EBT_KINEMATIC	};

public:
	PhysicalBody(PhysicalWorld* world, PhysicalShape*);
	
	void applyForce( Vector2f& force );	
	
	void applyForce( Vector2f& force, Vector2f& point );	

	void applyImpulse( Vector2f& impulse );	
	
	void applyImpulse( Vector2f& impulse, Vector2f& point );

	void applyTorque( Vector2f& torque );	

	PhysicalShape* createPhysicalShape(PhysicalAppearance*);
public:
	Vector2f m_Centre;
	Vector2f m_Velocity;
	dfloat m_AngularVelocity;
	dfloat m_Angle;
	dfloat m_LinearDamping;
	dfloat m_AngularDamping;
	Matrix3f m_Transform;
	BODY_TYPE m_Type;
	dfloat m_Mass, m_InvMass;
	dfloat m_I, m_InvI;
protected:
	dList<PhysicalShape*> m_ShapeList;
};

END_NAMESPACE_DIRAC2D

#endif