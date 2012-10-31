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

class GameObject : public IUpdatable
{
public:
	GameObject()
	{
	}
	
	Matrix3f getTransform() const
	{
		return m_Transform;
	}
	
	Vector2f getPosition() const
	{
		return m_Position;
	}
	
	float getAngle() const
	{
		return m_Angle;
	}
	
	virtual void update(float dt){};

protected:
	Matrix3f m_Transform;
	Vector2f m_Position;
	Vector2f m_Velocity;
	float m_Angle;
};

#endif

