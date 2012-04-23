/*
 *  Body.h
 *  Dirac2D
 *
 *  Created by Prashant on 23/04/12.
 *  Copyright 2012 Sourcebits Technologies. All rights reserved.
 *
 */

#include "../definitions.h"
#include "../maths/MathUtil.h"

#ifndef _PHYSICAL_BODY_H_
#define _PHYSICAL_BODY_H_

BEGIN_NAMESPACE_DIRAC2D

class PhysicalBody
{
	enum BODY_TYPE { EBT_DYNAMIC, EBT_STATIC, EBT_KINEMATIC	};

public:
	PhysicalBody();
	
public:
	Vector2f m_Position;
	Vector2f m_Velocity;
	float m_AngularVelocity;
	float m_Angle;
	float m_LinearDamping;
	float m_AngularDamping;
	Matrix3f m_Transform;
	BODY_TYPE m_Type;
};

END_NAMESPACE_DIRAC2D

#endif