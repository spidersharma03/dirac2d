/*
 *  Dirac2D.h
 *  Dirac2D
 *
 *  Created by Prashant on 31/03/12.
 */

/*
 -------------------------------------------------------------
 SEQUENTIAL IMPULSE BASED RIGID BODY PHYSICS ENGINE PIPELINE
 -------------------------------------------------------------
 
 1.  Accumulate External Forces( Forces due to gravity, wind, user supplied etc. )
 
 2.  Integrate velocities due to External Forces.
 
 3.  Find Corrective Impulses due to Constraints and using these impulses correct the velocities of the previous step.)
 
 4.  Integrate Positions and Orientations using the final corrected velocities.
 
 
 ---------------------------------------------------------------------------------
 COLLISION DETECTION FOR FINDING THE CORRECTIVE IMPULSES DUE TO INTERPENETRATION
 ---------------------------------------------------------------------------------
 
 1. Broad phase collision detection to find the potential colliding physical bodies.( ex. Dynamic AABB Tree )
 
 2. Narrow phase collision detection to find contacts( contactManifolds( multiple contact points) ) for each physical body, which passes the above broad phase collision detection.
 
 3. Find Impulses to prevent interpenetration for the above contactManifolds.
 
 ---------------------------------------------------------------------------------
 GENERAL CONSTRAINT METHODOLOGY TO FIND IMPULSES
 ---------------------------------------------------------------------------------
 
 1.  Set up the Constraint equation between pair of bodies.
 
 2.  Differentiate the above equation to find the Jacobian for the constraint. the Jacobian is a n x 6 vector where n is the number of constraint rows.
	the Jacobian actually kind of reduces the number of degrees of freedom of a system.
 
 3.  Find the Impulses by solving the constraint equations.
 
 
 
 */

#ifndef _DIRAC_2D_H_
#define _DIRAC_2D_H_

#include "Settings.h"
#include "definitions.h"
#include "Common.h"

#include "../Dirac2D/memory/memoryAllocator.h"

#include "../Dirac2D/maths/MathUtil.h"

#include "../Dirac2D/dynamics/PhysicalBody.h"
#include "../Dirac2D/dynamics/PhysicalWorld.h"
#include "../Dirac2D/dynamics/PhysicalAppearance.h"
#include "../Dirac2D/dynamics/PhysicalShape.h"

#include "../Dirac2D/dynamics/contacts/Contact.h"
#include "../Dirac2D/dynamics/contacts/ContactSolver.h"

#include "../Dirac2D/geometry/CollisionShape.h"
#include "../Dirac2D/geometry/Circle.h"
#include "../Dirac2D/geometry/Capsule.h"
#include "../Dirac2D/geometry/Box.h"
#include "../Dirac2D/geometry/RegularPolygon.h"

#include "../Dirac2D/collision/CollisionDetectionPolygon.h"
#include "../Dirac2D/collision/CollisionManager.h"

#endif