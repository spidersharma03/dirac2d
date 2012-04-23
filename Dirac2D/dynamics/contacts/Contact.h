/*
 *  Contact.h
 *  Dirac2D
 *
 *  Created by Prashant on 23/04/12.
 *  Copyright 2012 Sourcebits Technologies. All rights reserved.
 *
 */

#include "../../definitions.h"
#include "../../Settings.h"
#include "../../maths/MathUtil.h"

#ifndef _CONTACT_H_
#define _CONTACT_H_

BEGIN_NAMESPACE_DIRAC2D


struct Contact
{
	Point2f m_ContactPoint;
	dfloat m_Depth;
};

struct ContactManifold
{
	Contact m_ContactPoints[MAX_CONTACTS];
};

END_NAMESPACE_DIRAC2D

#endif