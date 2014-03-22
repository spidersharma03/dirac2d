/*
 *  CatenaryConstraintFixedRotation.h
 *  Dirac2D
 *
 *  Created by Prashant on 06/07/12.
 *
 */


#ifndef _CATENARY_CONSTRAINT_FIXED_ROTATION_H_
#define _CATENARY_CONSTRAINT_FIXED_ROTATION_H_

#include "Constraint.h"

BEGIN_NAMESPACE_DIRAC2D

struct CatenaryConstraintFixedRotationInfo : public ConstraintInfo
{
    CatenaryConstraintFixedRotationInfo()
    {
        m_Type = ECT_CATENARY_FIXED_ROTATION;
        m_PhysicalBody1 = m_PhysicalBody2 = 0;
        m_FixedLength = -1.0f;
		m_Frequency = 3.0f;
		m_DampingRatio = 0.2f;
    }
    // Anchor Point.
    Vector2f m_Anchor;
    Vector2f m_FixedPoint1, m_FixedPoint2;
    dfloat m_FixedLength;
};

class CatenaryConstraintFixedRotation : public Constraint
{
public:
	
    virtual void buildJacobian(dfloat dt);
	
	virtual void correctVelocities();
	
	virtual void initialize();
	
    friend class PhysicalWorld;
    
protected:
    	
    CatenaryConstraintFixedRotation( const CatenaryConstraintFixedRotationInfo& cInfo);
    
protected:
	// the Anchor point is always assumed in the local coordinate frames of the PhysicalBody.
	Vector2f m_Anchor;
	Vector2f m_FixedPoint1, m_FixedPoint2;
	Vector2f m_ImpulseDirection;
	Vector2f m_r; // Local Anchor Point on the Body.
	dfloat m_Distance;
	dfloat m_TotalLength;
	dfloat m_FixedLength;
	Matrix2f m_EffectiveMassMatrix;
	Vector2f m_Impulse;
	dfloat m_PositionError;
	dfloat m_AngularError;
	dfloat m_ReferenceAngle;
	CONSTRAINT_LIMIT_STATE m_LengthLimitState;
};

END_NAMESPACE_DIRAC2D

#endif