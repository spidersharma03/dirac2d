/*
 *  PhysicalWorld.h
 *  Dirac2D
 *
 *  Created by Prashant on 24/04/12.
 *
 */

#include "../definitions.h"
#include "../Settings.h"
#include "../maths/MathUtil.h"
#include "PhysicalAppearance.h"
#include "../memory/memoryAllocator.h"
#include "joints/Constraint.h"
#include "../CallBacks.h"


#include <vector>

using namespace std;

#ifndef _PHYSICAL_WORLD_H_
#define _PHYSICAL_WORLD_H_

BEGIN_NAMESPACE_DIRAC2D

class PhysicalBody;
class CollisionManager;
class ContactSolver;
class Contact;
class Renderer;
class BroadPhaseCollisionAlgorithm;
class BroadPhaseNode;
class PhysicalShape;

class DistanceConstraint;
class HingeConstraint;
class WeldConstraint;
class CatenaryConstraint;
class CatenaryConstraintFixedRotation;
class LineConstraint;
class PrismaticConstraint;
class WheelConstraint;
class PulleyConstraint;
class MotorConstraint;
class MinMaxConstraint;

enum BROAD_PHASE_ALGORITHM { EBPA_NAIVE, EBPA_SAP, EBPA_DYNAMIC_TREE };

class PhysicalWorld
{
private:
	class WorldOverlapAABBCallBackClass : public OverlapCallBackClass
	{
	  public:
		virtual void overlapCallBack(dint32 overlapNodeID, void* userData);
	};

	class CWorldRayIntersectionCallBackClass : public WorldRayIntersectionCallBackClass
	{
	  public:
		virtual dfloat rayIntersectionCallBack(const RaySegment2f& raySeg, PhysicalShape* pShape, RayIntersectionInfo& info);
	};
	
	class CRayIntersectionCallBackClass : public RayIntersectionCallBackClass
	{
	  public:
		virtual dfloat rayIntersectionCallBack(const RaySegment2f& raySeg, dint32 overlapNodeID, void* userData);
	};

	class CCollisionListener : public ICollisionLisetner
    {
      public:
        // Called when two Physical Shapes begin to touch
        virtual void onCollisionEnter(PhysicalShape* pShape1, PhysicalShape* pShape2,
        ContactManifold& manifold)
        {
        }
        
        // Called when two Physical Shapes are in contact
        virtual void onCollision(PhysicalShape* pShape1, PhysicalShape* pShape2, 
        ContactManifold& manifold)
        {
            
        }
        
        // Called when two Physical Shapes begin to seperate
        virtual void onCollisionExit(PhysicalShape* pShape1, PhysicalShape* pShape2, 
        ContactManifold& manifold)
        {
        }
    };
    
	CRayIntersectionCallBackClass m_RayIntersectionCallBackClass;
public:
	PhysicalWorld();
	
	Constraint* createConstraint(CONSTRAINT_TYPE constraintType);

	void deleteConstraint(Constraint* pConstraint);

	PhysicalBody* createPhysicalBody();

	void deletePhysicalBody(PhysicalBody* pBody);

	void Step(dfloat dt);
	
	PhysicalBody* pickBodyFromScreenCoordinates(Vector2f p);
	
	void draw();
	
	void setRenderer(Renderer*);
	
	Renderer* getRenderer()
	{
		return m_Renderer;
	}
	
	inline void setGravityOn(dbool btrue)
	{
		m_bGravityOn = btrue;
	}
	
	inline void addExternalForce( Vector2f& fext )
	{
		m_TotalForce += fext;
	}
	
	void setWarmStart( dbool bWarmStart )
	{
		m_bWarmStart = bWarmStart;
	}
	
	BroadPhaseCollisionAlgorithm* getBroadPhaseAlgorithm()
	{
		return m_pBroadPhaseAlgorithm;
	}

	void overlapAABB( AABB2f& queryAABB, OverlapCallBackClass* pOverlapCallBackClass = 0 );

	void intersectRaySegment( const RaySegment2f& raySeg, RayIntersectionCallBackClass* callBack );

	void intersectRaySegment( const RaySegment2f& raySeg );

	
	PhysicalBody* getPhysicalBodyList()
	{
		return m_PhysicalBodyList;
	}
    
    void setCollisionListener( ICollisionLisetner* pCollisionListener );

	void setRayIntersectionListener(WorldRayIntersectionCallBackClass* pRayIntersectionCallback);
	
public:
	
	friend class CollisionManager;
	friend class ContactSolver;
	friend class PhysicalBody;
	
protected:
	
	void addToBroadPhase( PhysicalShape* pShape);

    void removeFromBroadPhase( PhysicalShape* pShape);

private:
    // Default Collision Listener
    ICollisionLisetner *m_pCollisionListener;
    WorldRayIntersectionCallBackClass* m_pRayIntersectionCallback;
	
	CollisionManager* m_CollisionManager;
	BroadPhaseCollisionAlgorithm* m_pBroadPhaseAlgorithm;
	ContactSolver*	  m_ContactSolver;
	Renderer*         m_Renderer;

	Contact* m_ContactList;  // Doubly linked list of all the Contacts in this world.
	
	PhysicalBody* m_PhysicalBodyList; // Doubly linked list of all the Bodies in this world.
	
	Constraint* m_ConstraintList; // Doubly linked list of all the Constraints in this world.
	
	static Vector2f GRAVITY;
	
	Vector2f m_TotalForce;
	Vector2f m_TotalTorque;
	
	dfloat m_CFM, m_ERP;
	dbool m_bGravityOn;
	dfloat m_GravityScale;
	
	dint16 m_VelocityIterations;
	
	dbool m_bDrawAABB;
	
	dbool m_bWarmStart;
	
    MemoryAllocator<PhysicalShape> *m_PhysicaShapePool;
	MemoryAllocator<PhysicalBody> *m_PhysicalBodyPool;
	MemoryAllocator<BroadPhaseNode> *m_BroadPhaseNodePool;
	// Constraint Pools
	MemoryAllocator<DistanceConstraint> *m_DistanceConstraintPool;
	MemoryAllocator<MinMaxConstraint> *m_MinMaxConstraintPool;
	MemoryAllocator<HingeConstraint> *m_HingeConstraintPool;
	MemoryAllocator<WeldConstraint> *m_WeldConstraintPool;
	MemoryAllocator<CatenaryConstraint> *m_CatenaryConstraintPool;
	MemoryAllocator<CatenaryConstraintFixedRotation> *m_CatenaryConstraintFixedRotationPool;
	MemoryAllocator<LineConstraint> *m_LineConstraintPool;
	MemoryAllocator<WheelConstraint> *m_WheelConstraintPool;
	MemoryAllocator<MotorConstraint> *m_MotorConstraintPool;
	MemoryAllocator<PrismaticConstraint> *m_PrismaticConstraintPool;
	MemoryAllocator<PulleyConstraint> *m_PulleyConstraintPool;
	
	dbool m_bDrawShapes;
	dbool m_bDrawBoundingBoxes;
	dbool m_bDrawContacts;
	dbool m_bDrawConstraints;
	dbool m_bDrawCentreOfMass;	
};

END_NAMESPACE_DIRAC2D

#endif