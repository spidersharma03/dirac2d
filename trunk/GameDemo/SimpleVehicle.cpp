//
//  Vehicle.cpp
//  Dirac2D
//
//  Created by Prashant Sharma on 27/10/12.
//

#include "SimpleVehicle.h"
#include "Camera.h"
#include "SimpleGun.h"
#include "LaserGun2.h"
#include "FirstGame.h"
#include "GunRayCastCallback.h"

class RayCastTerrainCallBack : public WorldRayIntersectionCallBackClass
{
public:
	RayCastTerrainCallBack(FirstGame* pGame )
	{
	}
	
	float rayIntersectionCallBack(const RaySegment2f& raySeg, PhysicalShape* pShape, RayIntersectionInfo& info)
	{
		GameObject* pObject = (GameObject*)pShape->getUserData();
		
		if( !pObject ) // This is a Terrain
		{
			m_IntersectionInfo = info;
		}
		else
		{
			return -1.0f;
		}
		return info.m_HitT;
	}
	
	friend class SimpleVehicle;
private:
	RayIntersectionInfo m_IntersectionInfo; // Closest Intersection Info
protected:
	FirstGame* m_pGame;
};

SimpleVehicle::SimpleVehicle(FirstGame* pGame)
{
    m_pGame = pGame;
    	
	m_pGun = new LaserGun2(m_pGame);
	
	m_pRayCastTerrainCallBack = new RayCastTerrainCallBack(m_pGame);
	
    init();
}

void SimpleVehicle::applyForce( const Vector2f& force )
{
	m_pVehicleBody->applyForce(force);
}

void SimpleVehicle::applyImpulse( const Vector2f& impulse)
{
	m_pVehicleBody->applyImpulse(impulse, Vector2f(0.0f,0.0f));
}

void SimpleVehicle::applyTorqueImpulse( const float torqueImpulse)
{
	m_pVehicleBody->applyImpulsiveTorque(torqueImpulse);
}

void SimpleVehicle::init()
{
    PhysicalWorld* pWorld = m_pGame->getPhysicalWorld();
    
    m_MinSpeed = 20.0f;
	m_MaxSpeed = 250.0f;
	m_BoostIncreament = 10.0f;
	m_DistanceFromTerrain = 0.01f;
	
	for( dint32 i=0; i< 1; i++ )
	{
		m_pVehicleBody = pWorld->createPhysicalBody();
		
		float xPos = 25.0f;
		float yPos = 2.0f;
        
		m_pVehicleBody->setPosition(Vector2f(1.0f+xPos,1.05f+yPos));		
		
		PhysicalAppearance pApp;
        pApp.m_CollisionAttributes.m_Filter.m_CollisionBit = EOCB_PHYSICAL_BODY;
        pApp.m_CollisionAttributes.m_Filter.m_CollisionMask = EOCB_PARTICLE_DEBRIS;

		pApp.m_MassAttributes.m_Density = 10.0f;
        CapsuleInfo capsuleInfo(0.07f, 0.6f);
		dfloat vehicleWidth = 0.4f; dfloat vehicleHeight = 0.08f;
		Vector2f vertices[4] = { Vector2f(vehicleWidth, vehicleHeight), 
								 Vector2f(-vehicleWidth, vehicleHeight), 
								 Vector2f(-vehicleWidth, -vehicleHeight), 
								 Vector2f(vehicleWidth, -vehicleHeight) };
		PolygonInfo pInfo(vertices, 4);
		pApp.m_CollisionAttributes.m_CollisionShapeInfo = &pInfo;
		
        //pApp.m_CollisionAttributes.m_CollisionShapeInfo = &capsuleInfo;
		PhysicalShape* pShape = m_pVehicleBody->createPhysicalShape(pApp);
		pShape->setUserData(this);
        
		m_pVehicleBody->m_AngularDamping = 50.0f;
		
		PhysicalBody* circle1 = pWorld->createPhysicalBody();
		PhysicalBody* circle2 = pWorld->createPhysicalBody();
        circle1->setPosition(Vector2f(1.3f+xPos,0.8f+yPos));		
		circle2->setPosition(Vector2f(1.0f-0.3f+xPos,0.8f+yPos));		
		
		pApp.m_MassAttributes.m_Density = 50.0f;
        CircleInfo cInfo(0.1f);
		pApp.m_CollisionAttributes.m_CollisionShapeInfo = &cInfo;
		
		pShape = circle1->createPhysicalShape(pApp);
		pShape->setUserData(this);
		        
        pApp.m_PhysicalAttributes.m_Friction = 1.0f;
    
        pShape = circle2->createPhysicalShape(pApp);
		pShape->setUserData(this);
		
        WheelConstraintInfo wInfo;
        wInfo.m_PhysicalBody1 = m_pVehicleBody;
		wInfo.m_PhysicalBody2 = circle1;
		wInfo.m_Anchor = Vector2f(1.3f+xPos,0.8f+yPos);
		wInfo.m_LocalAxis = Vector2f(1.0f,-1.0f);
		wInfo.m_Frequency = 2.0f;
		wInfo.m_DampingRatio = 1.5f;
		wInfo.m_bCollideConnected = false;
		 
		WheelConstraint* lc1 = (WheelConstraint*)pWorld->createConstraint(wInfo);
        lc1->initialize();
                
        wInfo.m_PhysicalBody1 = m_pVehicleBody;
		wInfo.m_PhysicalBody2 = circle2;
		wInfo.m_Anchor = Vector2f(1.0f-0.3f+xPos,0.8f+yPos);
		wInfo.m_LocalAxis = Vector2f(-1.0f,-1.0f);
        
		WheelConstraint* lc2 = (WheelConstraint*)pWorld->createConstraint(wInfo);
        lc2->initialize();

        
		MotorConstraintInfo mInfo;
        mInfo.m_PhysicalBody1 = circle2;
		mInfo.m_MaxTorque = 100.0f;
		mInfo.m_Speed = m_MinSpeed;
        
        m_pMotor = (MotorConstraint*)pWorld->createConstraint(mInfo);
		m_pMotor->initialize();
	}

}

void SimpleVehicle::initFromFile(const char* fileName)
{
    
}

void SimpleVehicle::update(float dt)
{
	m_Position = m_pVehicleBody->m_Position;
	m_Transform = m_pVehicleBody->getTransform();
	
	// Cast a Ray Downwards to get distance of the Vehicle from the Terrain.
	RaySegment2f raySegment(m_Position, m_Position + Vector2f(0.0f, -20.0f) );
	m_pGame->getPhysicalWorld()->intersectRaySegment(raySegment, m_pRayCastTerrainCallBack);
	
	m_DistanceFromTerrain = m_pRayCastTerrainCallBack->m_IntersectionInfo.m_HitPoint.distance(m_Position);
	
	if( m_DistanceFromTerrain > 20.0f )
		m_DistanceFromTerrain = 20.0f;
	
	m_pGun->update(dt);
	
	if( m_bBoostEnable )
	{
		accelerate();
	}
	else 
	{
		applyBrake();
	}
}

void SimpleVehicle::render()
{
	m_pGun->render();
}

void SimpleVehicle::accelerate()
{
    float speed = m_pMotor->getSpeed();
	m_pMotor->setSpeed(speed + m_BoostIncreament);
	if( m_pMotor->getSpeed() >= m_MaxSpeed )
		m_pMotor->setSpeed(m_MaxSpeed);
}

void SimpleVehicle::applyBrake()
{
	float speed = m_pMotor->getSpeed();
	m_pMotor->setSpeed(speed - m_BoostIncreament);
	if( m_pMotor->getSpeed() < m_MinSpeed )
		m_pMotor->setSpeed(m_MinSpeed);
}

void SimpleVehicle::shoot(const Vector2f& direction)
{
	m_pGun->shoot(direction);
}


// COLLISION CALL BACKS

void SimpleVehicle::onCollisionEnter( GameObject* pObject, ContactManifold& manifold )
{
}

void SimpleVehicle::onCollisionExit( GameObject* pObject, ContactManifold& manifold )
{
}
