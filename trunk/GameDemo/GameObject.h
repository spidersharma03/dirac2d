/*
 *  GameObject.h
 *  Dirac2D
 *
 *  Created by Prashant on 30/10/12.
 *
 */

#include "maths/MathUtil.h"
#include "IUpdatable.h"
#include "IGameCollisionListener.h"


USE_NAMESPACE_DIRAC2D

#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#define AVG_OBJECT_DENSITY 1.0

enum GAME_OBJECT_TYPE { EOT_COIN = 1, EOT_CRATE, EOT_DEBRIS ,EOT_TUMBLER, EOT_VEHICLE, EOT_PATH_HURDLE, EOT_CABLE_CAR, EOT_NONE };

enum OBJECT_COLLISION_BIT { EOCB_COIN = 0x1, EOCB_PHYSICAL_BODY = 0x2, EOCB_TERRAIN = 0x4, EOCB_PARTICLE_DEBRIS = 0x8};
	
struct GameObjectInfo
{
	GameObjectInfo()
	{
		m_ObjectType = EOT_COIN;
		m_AngularVelocity = 0.0f;
		m_Density = AVG_OBJECT_DENSITY;
		m_Restitution = 1.0f;
		m_Friction = 1.0f;
		m_BodyType = 0;
		m_bAutoCull = true;
	}
	
    Vector2f m_Position;
    Vector2f m_Velocity;
    float m_AngularVelocity;
	float m_Density;
	float m_Restitution;
	float m_Friction;
	GAME_OBJECT_TYPE m_ObjectType;
	int m_BodyType;
	bool m_bAutoCull;
};

class GameObject : public IUpdatable, public IGameCollisionListener
{
public:
	GameObject()
	{
        m_ObjectInfo.m_ObjectType = EOT_NONE;
		m_pNext = 0;
		m_pPrev = 0;
		m_ObjectCount++;
	}
	    
    GameObject(GameObjectInfo gInfo)
    {
        m_ObjectInfo = gInfo;
		m_pNext = 0;
		m_pPrev = 0;
		m_ObjectCount++;
    }
	
	inline Matrix3f getTransform() const
	{
		return m_Transform;
	}
	
	inline Vector2f getPosition() const
	{
		return m_Position;
	}
	
	inline float getAngle() const
	{
		return m_Angle;
	}
	
	inline GameObjectInfo getGameObjectInfo()
	{
		return m_ObjectInfo;
	}
    
	virtual void update(float dt){};

    virtual ~GameObject()
    {
        m_ObjectCount--;
    }
	
    // COLLISION CALL BACKS
    
    virtual void onCollisionEnter( GameObject* pObject, ContactManifold& manifold ){};
    
    virtual void onCollisionExit( GameObject* pObject, ContactManifold& manifold ){};
    
	GameObject* m_pNext;
	GameObject* m_pPrev;
	
protected:
	Matrix3f m_Transform;
	Vector2f m_Position;
	Vector2f m_Velocity;
	float m_Angle;
	GameObjectInfo m_ObjectInfo;
	static int m_ObjectCount;
};

#endif

