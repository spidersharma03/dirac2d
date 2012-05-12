/*
 *  Settings.h
 *  Dirac2D
 *
 *  Created by Prashant on 23/04/12.
 *
 */

#include "definitions.h"

#ifndef _SETTINGS_H_
#define _SETTINGS_H_

BEGIN_NAMESPACE_DIRAC2D

// Maximum Contact Points a manifold can hold.
#define MAX_CONTACTS 2
// Maximum Number of vertices a Polygon can have
#define MAX_POLY_VERTICES 20
// Allowed penetration for collision
#define ALLOWED_PENETRATION 0.01f
// Maximum nuber of Contact Points in the Physical World
#define MAX_CONTACT_POINTS 100

END_NAMESPACE_DIRAC2D

#endif