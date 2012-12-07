/*
 *  GameObject.h
 *  Dirac2D
 *
 *  Created by Prashant on 30/10/12.
 *
 */

#include "../Dirac2D/maths/MathUtil.h"
#include "IUpdatable.h"

USE_NAMESPACE_DIRAC2D

#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#define AVG_OBJECT_DENSITY 1.0

enum GAME_OBJECT_TYPE { EOT_COIN, EOT_CRATE , EOT_NONE };

struct GameObjectInfo
{
	GAME_OBJECT_TYPE m_ObjectType;
};

class GameObject : public IUpdatable
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

