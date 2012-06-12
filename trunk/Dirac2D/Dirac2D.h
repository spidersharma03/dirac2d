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

#include "memory/memoryAllocator.h"

#include "maths/MathUtil.h"


#include "dynamics/PhysicalBody.h"
#include "dynamics/PhysicalWorld.h"
#include "dynamics/PhysicalAppearance.h"
#include "dynamics/PhysicalShape.h"

#include "dynamics/contacts/Contact.h"
#include "dynamics/contacts/ContactSolver.h"

#include "dynamics/joints/Constraint.h"
#include "dynamics/joints/DistantConstraint.h"
#include "dynamics/joints/HingeConstraint.h"

#include "geometry/CollisionShape.h"
#include "geometry/Circle.h"
#include "geometry/Capsule.h"
#include "geometry/Box.h"
#include "geometry/ConvexPolygon.h"
#include "geometry/Edge.h"
#include "geometry/EdgeChain.h"

#include "collision/CollisionDetection.h"
#include "collision/CollisionDetectionPolygon.h"
#include "collision/CollisionManager.h"
#include "collision/broadPhase/BroadPhaseCollisionAlgorithm.h"

#include "draw/Renderer.h"
#include "draw/GLRenderer.h"

#endif