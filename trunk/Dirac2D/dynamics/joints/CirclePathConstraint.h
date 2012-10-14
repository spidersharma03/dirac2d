//
//  CirclePathConstraint.h
//  Dirac2D
//
//  Created by Prashant Sharma on 16/09/12.
//

#ifndef _CIRCLE_PATH_CONSTRAINT_H_
#define _CIRCLE_PATH_CONSTRAINT_H_

#include "Constraint.h"

BEGIN_NAMESPACE_DIRAC2D

class CirclePathConstraint : public Constraint
{
public:
    
    CirclePathConstraint();
	
	virtual void buildJacobian();
	
	virtual void correctVelocities();
	
	virtual void initialize();
    
public:
    Vector2f m_Centre;
    dfloat m_Radius;
};

END_NAMESPACE_DIRAC2D

#endif
