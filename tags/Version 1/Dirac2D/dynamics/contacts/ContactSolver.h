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

class PhysicalWorld;
class Contact;

class ContactSolver
{
public:
	ContactSolver(PhysicalWorld* pPhysicalWorld);

	// Initialize the effective mass matrix data.
	void buildJacobian();

	// Solve for the Normal and Tangent(Frictional) impulses and apply them on to the physical bodies.
	void correctVelocities();
	
	friend class PhysicalWorld;
	
private:
	PhysicalWorld* m_PhysicalWorld;
	Contact *m_Contacts;
	dint32 m_NumContacts;
};

END_NAMESPACE_DIRAC2D

#endif