/*
 *  ContactSolver.h
 *  Dirac2D
 *
 *  Created by Prashant on 23/04/12.
 *
 */

#include "../../definitions.h"
#include "../../Settings.h"
#include "../../maths/MathUtil.h"

#ifndef _CONTACTSOLVER_H_
#define _CONTACTSOLVER_H_

BEGIN_NAMESPACE_DIRAC2D

class ContactSolver
{
public:
	ContactSolver();

	void buildJacobian();

	void correctVelocities();
};

END_NAMESPACE_DIRAC2D

#endif