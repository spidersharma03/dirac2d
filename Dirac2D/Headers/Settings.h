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
#define MAX_POLY_VERTICES 10
// Allowed penetration for collision
#define ALLOWED_PENETRATION 0.01f
// Maximum nuber of Contact Points in the Physical World
#define MAX_CONTACT_POINTS 2000
// Maximum number of bodies in the Physical world.
#define MAX_BODIES 1000
// Maximum number of bodies in the Physical world.
#define MAX_PHYSICAL_SHAPES 1000
// Maximum number of Proxies in Boroad Phase
#define MAX_PROXIES 1000

// Maximum Angular Speed of a Physical Body.
#define MAX_ANGULAR_SPEED 10.0f
// Maximum Linear Speed of a Physical Body.
#define MAX_LINEAR_SPEED 10.0f

// Below this velocity the collision are considered inelastic
#define VELOCITY_BIAS_THRESHOLD 1.0f

#define ANGULAR_ERROR (1.0f*PI/180.0f)
#define LINEAR_ERROR (0.01f) 

// 
#define EDGE_THICKNESS 0.02f
// if the sleeping flag for a Physical Body is on, the body will go to sleep when (body->m_SleepTime >  SLEEPING_THRESHOLD)
#define SLEEPING_THRESHOLD 1.0f
// Below this kinetic energy, the Physical body may go to sleep.
#define ENERGY_THRESHOLD 1e-5f

// Extend the AABB of the Physical Bodies by this factor in Dynamic Tree. this exploits temporal coherence.
#define AABB_EXTENSION_FACTOR 0.01f

END_NAMESPACE_DIRAC2D

#endif