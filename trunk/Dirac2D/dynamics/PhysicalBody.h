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
class Contact;
class ContactEdge;

enum BODY_TYPE { EBT_DYNAMIC, EBT_STATIC, EBT_KINEMATIC	};

class PhysicalBody
{

public:	
	PhysicalBody(const PhysicalBody& other);
	
	void operator=(const PhysicalBody& other);
	
	PhysicalBody* clone();
	
	// adds this Body to the physical World. only useful for adding Clones to the Physical World. if this body is already part of the world it wont get added.
	void addToPhysicalWorld(PhysicalWorld* pWorld);

	// Removes this Body from the physical World.
	void removeFromPhysicalWorld(PhysicalWorld* pWorld);

	void applyForce( Vector2f& force );	
	
	void applyForce( Vector2f& force, Vector2f& point );	

	void applyImpulse( Vector2f& impulse );	
	
	void applyImpulse( Vector2f& impulse, Vector2f& point );

	void applyTorque( Vector2f& torque );	

	PhysicalShape* createPhysicalShape(PhysicalAppearance& pApp);
	
	// Gets the worldPosition in Local Coordinate Frame of the Body.
	Vector2f getLocalPoint(Vector2f& worldPosition)
	{
		return  ( worldPosition * m_Transform - m_Centre);
	}
	
	// Gets the localPosition in World Coordinate Frame. localPosition is assumed to be given from the centre of mass of the Body.  
	Vector2f getWorldPoint(Vector2f& localPosition)
	{
		return m_Transform * ( localPosition + m_Centre );
	}
	
	// Gets the velocity at the given local Coordinate of the body.
	Vector2f getVelocityAtPosition( Vector2f& localPosition )
	{
		return m_Velocity + Vector2f::cross(m_AngularVelocity, localPosition);
	}
	
	void setPosition(Vector2f position)
	{
		m_Position = position;
		m_Transform.translate(position);
	}
	
	void setAngle(dfloat angle)
	{
		m_Angle = angle;
		m_Transform.rotate(m_Angle);
	}
	
	void updateTransform();
	
	Matrix3f getTransform()
	{
		return m_Transform;
	}
	
	void setTransform( Matrix3f& xForm )
	{
		m_Transform = xForm;
	}
	
	inline void setSleepingPolicy(dbool bSleepingPolicy)
	{
		m_bSleepingPolicy = bSleepingPolicy;
	}
	
	inline void setSleeping(dbool bSleep)
	{
		if( !bSleep )
			m_SleepTime = 0.0f;
		m_bSleeping = bSleep;
	}
	
	void updateSleepingStatus(dfloat dt);
	
	void updateAABB();
		
	virtual ~PhysicalBody();
public:
	Vector2f m_Centre, m_Position;
	Vector2f m_Velocity;
	dfloat m_AngularVelocity;
	dfloat m_Angle;
	dfloat m_LinearDamping;
	dfloat m_AngularDamping;
	Matrix3f m_Transform;
	BODY_TYPE m_BodyType;
	dfloat m_Mass, m_InvMass;
	dfloat m_I, m_InvI;
	
	dbool m_bSleepingPolicy;
	dbool m_bSleeping;
	dfloat m_SleepTime;
	
	Vector2f m_Force;
	AABB2f m_AABB;
	
	ContactEdge* m_ContactEdgeList;

	friend class CollisionManager;
	friend class ContactSolver;
	friend class PhysicalWorld;
	
protected:
	duchar m_bIsAlive; // Is the Body inside the PhysicalWorld.
	
	PhysicalBody* m_Next;
	PhysicalBody* m_Prev;
	// Calculates MassAttributes( mass, moment of inertia, centre of mass) of this Body from the Physical Shapes associated with this Body.
	void calculateMassAttributes();
	
	PhysicalBody(PhysicalWorld* world);

	PhysicalShape* m_PhysicalShapeList;
		
	PhysicalWorld* m_PhysicalWorld;
};

END_NAMESPACE_DIRAC2D

#endif