/*
 *  Body.h
 *  Dirac2D
 *
 *  Created by Prashant on 23/04/12.
 *
 */

#include "../definitions.h"
#include "../maths/MathUtil.h"
#include "../common.h"

#ifndef _PHYSICAL_BODY_H_
#define _PHYSICAL_BODY_H_

BEGIN_NAMESPACE_DIRAC2D

class PhysicalWorld;
class CollisionShape;
class PhysicalShape;
class PhysicalAppearance;

class PhysicalBody
{
	enum BODY_TYPE { EBT_DYNAMIC, EBT_STATIC, EBT_KINEMATIC	};

public:	
	void applyForce( Vector2f& force );	
	
	void applyForce( Vector2f& force, Vector2f& point );	

	void applyImpulse( Vector2f& impulse );	
	
	void applyImpulse( Vector2f& impulse, Vector2f& point );

	void applyTorque( Vector2f& torque );	

	PhysicalShape* createPhysicalShape(PhysicalAppearance& pApp);
	
	void updateTransform();
	
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
	
	Vector2f m_Force;
	
	friend class CollisionManager;
	friend class ContactSolver;
	friend class PhysicalWorld;
	
protected:
	
	PhysicalBody(PhysicalWorld* world, PhysicalShape*);

	PhysicalShape* m_PhysicalShapeList;
	PhysicalShape* m_PhysicalShape;
};

END_NAMESPACE_DIRAC2D

#endif