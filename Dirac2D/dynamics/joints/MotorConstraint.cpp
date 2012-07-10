/*
 *  MotorConstraint.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 08/07/11.
 *
 */

#include "MotorConstraint.h"
#include "../../common.h"
#include "../PhysicalBody.h"
#include <stdio.h>

BEGIN_NAMESPACE_DIRAC2D

MotorConstraint::MotorConstraint()
{
	m_Type = ECT_MOTOR;
}

void MotorConstraint::initialize()
{
	dAssert(m_PhysicalBody1 || m_PhysicalBody2);
}

void MotorConstraint::buildJacobian()
{
	dfloat JInvmJT = 0.0f;
	if( m_PhysicalBody1 )
	{
		JInvmJT += m_PhysicalBody1->m_InvI;
	}
	if( m_PhysicalBody2 )
	{
		JInvmJT += m_PhysicalBody2->m_InvI;
	}
	if( JInvmJT != 0.0f )
		m_EffectiveMass = 1.0f/JInvmJT;
	
	//printf("Angular Speed = %f\n", m_PhysicalBody1->m_AngularVelocity);
	
	if( 1 )
	{
		if( m_PhysicalBody1 )
		{
			m_PhysicalBody1->m_AngularVelocity += m_PhysicalBody1->m_InvI * m_Impulse;
		}
		if( m_PhysicalBody2 )
		{
			m_PhysicalBody2->m_AngularVelocity += m_PhysicalBody2->m_InvI * m_Impulse;
		}
	}
	else 
	{
		m_Impulse = 0.0f;
	}

}

void MotorConstraint::correctVelocities()
{
	dfloat Cdot = m_Speed;
	
	if( m_PhysicalBody1 )
	{
		Cdot += m_PhysicalBody1->m_AngularVelocity;
	}
	if( m_PhysicalBody2 )
	{
		Cdot += m_PhysicalBody2->m_AngularVelocity;
	}
	dfloat correctiveImpulse = -m_EffectiveMass * Cdot;
	dfloat maxImpulse = m_MaxTorque/600.0f;
	dfloat oldImpulse = m_Impulse;
	m_Impulse = CLAMP(m_Impulse + correctiveImpulse, -maxImpulse, maxImpulse);
	correctiveImpulse = m_Impulse - oldImpulse;

	if( m_PhysicalBody1 )
	{
		m_PhysicalBody1->m_AngularVelocity += m_PhysicalBody1->m_InvI * correctiveImpulse;
	}
	if( m_PhysicalBody2 )
	{
		m_PhysicalBody2->m_AngularVelocity += m_PhysicalBody2->m_InvI * correctiveImpulse;
	}
}

END_NAMESPACE_DIRAC2D