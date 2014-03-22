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
struct ContactEdge;
struct ConstraintEdge;

enum BODY_TYPE { EBT_DYNAMIC, EBT_STATIC, EBT_KINEMATIC	};

class PhysicalBody
{
public:		
	void operator=(const PhysicalBody& other);
	
	PhysicalBody* clone();
	
	// adds this Body to the physical World. only useful for adding Clones to the Physical World. if this body is already part of the world it wont get added.
	void addToPhysicalWorld(PhysicalWorld* pWorld);

	// Removes this Body from the physical World. removes it from the doubly linked list.
	void removeFromPhysicalWorld(PhysicalWorld* pWorld);

	void applyForce( const Vector2f& force );	
	
	void applyForce( const Vector2f& force, const Vector2f& point );	

	void applyImpulse(const Vector2f& impulse );	
	
	void applyImpulse(const Vector2f& impulse, const Vector2f& point );

	void applyTorque( const Vector2f& torque );	

	void applyImpulsiveTorque( const float torqueImpulse );

	PhysicalShape* createPhysicalShape(PhysicalAppearance& pApp);

    void deletePhysicalShape(PhysicalShape* pShape);

	// Gets the worldVector in Local Coordinate Frame of the Body.
	Vector2f getLocalVector(const Vector2f& worldVector)
	{
		return m_Transform.getRotationMatrixTransposed() * worldVector;
	}
	
	// Gets the worldPosition in Local Coordinate Frame of the Body.
	Vector2f getLocalPoint(const Vector2f& worldPosition)
	{
		return  ( worldPosition * m_Transform - m_Centre);
	}
	
	// Gets the localPosition in World Coordinate Frame. localPosition is assumed to be given from the centre of mass of the Body.  
	Vector2f getWorldPoint(const Vector2f& localPosition)
	{
		return m_Transform * ( localPosition + m_Centre );
	}
	
	// Gets the localVector in World Coordinate Frame.  
	Vector2f getWorldVector(const Vector2f& localVector)
	{
		return m_Transform.getRotationMatrix() * localVector;
	}
	
	// Gets the velocity at the given local Coordinate of the body.
	Vector2f getVelocityAtPosition( Vector2f& localPosition )
	{
		return m_Velocity + Vector2f::cross(m_AngularVelocity, localPosition);
	}
	
	inline void setPosition(const Vector2f& position)
	{
		m_Position = position;
		m_Transform.translate(position);
        updateAABB();
	}
	
    inline void setVelocity(const Vector2f& velocity)
	{
		m_Velocity = velocity;
	}
    
    inline void setAngularVelocity( const dfloat angularVelocity )
    {
        m_AngularVelocity = angularVelocity;
    }
    
	void setAngle(dfloat angle)
	{
		m_Angle = angle;
		m_Transform.rotate(m_Angle);
	}
	
	void setFixedRotation( dbool bFixedRotation )
	{
		m_bFixedRotation = bFixedRotation;
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
	
	PhysicalShape* getPhysicalShapeList()
	{
		return m_PhysicalShapeList;
	}

	PhysicalBody* getNext()
	{
		return m_Next;
	}
	
	PhysicalBody* getPrevious()
	{
		return m_Prev;
	}
	
	dbool getCollisionStatus( PhysicalBody* pOther);
	
	void updateSleepingStatus(dfloat dt);
	
	void updateAABB();
		
	virtual ~PhysicalBody();

public:
	Vector2f m_Velocity;
	dfloat m_AngularVelocity;
	dfloat m_InvMass, m_InvI;
	
	/* Centre of Mass of the body. this is always local to the body. This can be different from origin for instance if the coordinates are defined not wrt the origin */
	Vector2f m_Centre; 
	
	Matrix3f m_Transform;

	dfloat m_Angle;
	dfloat m_LinearDamping;
	dfloat m_AngularDamping;
	
	Vector2f m_Position;

	dfloat m_Mass, m_I;
	
	dbool m_bSleepingPolicy;
	dbool m_bSleeping;
	dfloat m_SleepTime;
	
	AABB2f m_AABB;
	BODY_TYPE m_BodyType;
	Vector2f m_Force;

	dbool m_bFixedRotation;
	
	ContactEdge* m_ContactEdgeList;
    ConstraintEdge* m_ConstraintEdgeList;
    
	friend class CollisionManager;
	friend class ContactSolver;
	friend class PhysicalWorld;
	friend class Contact;
    
protected:
	duchar m_bIsAlive; // Is the Body inside the PhysicalWorld.
	
	PhysicalBody* m_Next;
	PhysicalBody* m_Prev;
	
	// Calculates MassAttributes( mass, moment of inertia, centre of mass) of this Body from the Physical Shapes associated with this Body.
	void calculateMassAttributes();
	
    PhysicalBody(const PhysicalBody& other);

	PhysicalBody(PhysicalWorld* world);

	PhysicalShape* m_PhysicalShapeList;
		
	PhysicalWorld* m_PhysicalWorld;
    
};

END_NAMESPACE_DIRAC2D

#endif