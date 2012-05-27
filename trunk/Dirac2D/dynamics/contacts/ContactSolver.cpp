/*
 *  ContactSolver.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 30/04/12.
 *
 */

#include "ContactSolver.h"
#include "Contact.h"
#include "../PhysicalShape.h"
#include "../PhysicalBody.h"
#include "../PhysicalWorld.h"

#include <stdio.h>

BEGIN_NAMESPACE_DIRAC2D


ContactSolver::ContactSolver(PhysicalWorld* pPhysicalWorld):m_PhysicalWorld(pPhysicalWorld)
{
}
	
void ContactSolver::buildJacobian()
{
	Contact* contactList = m_PhysicalWorld->m_ContactList;
	Contact* contact = contactList;
	while( contact )
	{
		PhysicalShape* shape1 = contact->m_PhysicalShape1;
		PhysicalShape* shape2 = contact->m_PhysicalShape2;		
		PhysicalBody* body1 = shape1->m_ParentBody;
		PhysicalBody* body2 = shape2->m_ParentBody;
		Vector2f c1 = body1->m_Centre;
		Vector2f c2 = body2->m_Centre;
		
		Vector2f n = contact->m_ContactNormal;
		Vector2f t;
		t = n.cross(-1.0f);
		
		for( dint32 i=0; i<contact->m_NumContactConstraints; i++ )
		{
			ContactConstraint& constraint = contact->m_ContactConstraint[i];
			
			Vector2f& p = contact->m_ContactPoint[i].m_Point;

			Vector2f r1 = p - c1;
			Vector2f r2 = p - c2;
			dfloat r1cross_n = r1.cross(n);
			dfloat r2cross_n = r2.cross(n);
			dfloat JInvMJT = body1->m_InvMass + body2->m_InvMass + r1cross_n * r1cross_n * body1->m_InvI + r2cross_n * r2cross_n * body2->m_InvI; 
			
			//dfloat depth = contact->m_ContactPoint[i].m_Depth;
			dfloat depth = contact->m_ContactPoint[i].m_Depth;
			
			// Effective mass for Normal Impulses
			contact->m_NormalMass[i] = 1.0f/JInvMJT + contact->m_CFM;
			// Positional Error for Position Stabilization( Baumgarte )
			constraint.m_PositionError = -contact->m_ERP * MIN( 0.0f, depth + ALLOWED_PENETRATION) * 600.0f;
			
			dfloat r1cross_t = r1.cross(t);
			dfloat r2cross_t = r2.cross(t);
			JInvMJT = body1->m_InvMass + body2->m_InvMass + r1cross_t * r1cross_t * body1->m_InvI + r2cross_t * r2cross_t * body2->m_InvI; 
			// Effective mass for Frictional Impulses
			contact->m_FrictionalMass[i] = 1.0f/JInvMJT;
			
			Vector2f relvel = ( body2->m_Velocity + Vector2f::cross(body2->m_AngularVelocity, r2) - body1->m_Velocity - Vector2f::cross(body1->m_AngularVelocity, r1) );
			dfloat velBias = 0.0f*relvel.dot(contact->m_ContactNormal);
						
			if( velBias > VELOCITY_BIAS_THRESHOLD )
				constraint.m_VelocityBias = velBias;
			else
				constraint.m_VelocityBias = 0.0f;
			
			// Apply Corrective impulse on the bodies due to Tangent/Frictional Impulses
			if( m_PhysicalWorld->m_bWarmStart )
			{
				Vector2f totalImpulse = contact->m_ContactNormal * constraint.m_NormalImpulse + t * constraint.m_TangentImpulse;
				body1->m_Velocity        +=  totalImpulse * body1->m_InvMass;
				body1->m_AngularVelocity += body1->m_InvI * Vector2f::cross( r1, totalImpulse);
				
				body2->m_Velocity        -= totalImpulse * body2->m_InvMass;
				body2->m_AngularVelocity -= body2->m_InvI * Vector2f::cross( r2, totalImpulse);
			}
			else 
			{
				constraint.m_NormalImpulse = 0.0f;
				constraint.m_TangentImpulse = 0.0f;
			}

		}
		
		contact = contact->m_Next;
	}
}
	
void ContactSolver::correctVelocities()
{
	Contact* contactList = m_PhysicalWorld->m_ContactList;
	Contact* contact = contactList;
	while( contact )
	{
		PhysicalShape* shape1 = contact->m_PhysicalShape1;
		PhysicalShape* shape2 = contact->m_PhysicalShape2;		
		PhysicalBody* body1 = shape1->m_ParentBody;
		PhysicalBody* body2 = shape2->m_ParentBody;
		Vector2f c1 = body1->m_Centre;
		Vector2f c2 = body2->m_Centre;
		
		Vector2f tangent = contact->m_ContactNormal.cross(-1.0f);
		
		for( dint32 i=0; i<contact->m_NumContactConstraints; i++ )
		{
			ContactConstraint& constraint = contact->m_ContactConstraint[i];
			Vector2f& p = contact->m_ContactPoint[i].m_Point;
			//Vector2f& p = contact->m_Manifold.m_ContactPoints[i].m_Point;

			Vector2f r1 = p - c1;
			Vector2f r2 = p - c2;
			
			dfloat Cdot_Normal, Cdot_Tangent; // Cdot = J * v
			// Relative Velocity of the Contact point on the Bodies = V2 - V1 = v2 + w2xr2 - v1 - w1xr1
			Vector2f relvel = ( body2->m_Velocity + Vector2f::cross(body2->m_AngularVelocity, r2) - body1->m_Velocity - Vector2f::cross(body1->m_AngularVelocity, r1) );
			
			Cdot_Normal  = relvel.dot(contact->m_ContactNormal);
			
			// Clamp the Total Impulse, not the Corrective Impulse, which could be negative.
			dfloat oldImpulseMag = constraint.m_NormalImpulse;
			dfloat correctiveImpulseMag = contact->m_NormalMass[i] * ( constraint.m_PositionError + Cdot_Normal + constraint.m_VelocityBias );
			
			// This Can't be negative.
			constraint.m_NormalImpulse = max(oldImpulseMag + correctiveImpulseMag, 0.0f);
			
			// This Could be negative.
			correctiveImpulseMag = constraint.m_NormalImpulse - oldImpulseMag;
			
			Vector2f correctiveImpulse = contact->m_ContactNormal * correctiveImpulseMag;
			
			// Apply Corrective impulse on the bodies due to Normal Impulse
			body1->m_Velocity        += correctiveImpulse * body1->m_InvMass;
			body1->m_AngularVelocity += body1->m_InvI * Vector2f::cross( r1, correctiveImpulse);
			
			body2->m_Velocity        -= correctiveImpulse * body2->m_InvMass;
			body2->m_AngularVelocity -= body2->m_InvI * Vector2f::cross( r2, correctiveImpulse);		
			
			relvel = ( body2->m_Velocity + Vector2f::cross(body2->m_AngularVelocity, r2) - body1->m_Velocity - Vector2f::cross(body1->m_AngularVelocity, r1) );
			Cdot_Tangent = relvel.dot(tangent);

			oldImpulseMag = constraint.m_TangentImpulse;
			dfloat mu = 0.9f;//( shape1->m_Friction + shape2->m_Friction ) * 0.5f + 0.9f; 
			dfloat maxFriction = constraint.m_NormalImpulse * mu;
			correctiveImpulseMag = contact->m_FrictionalMass[i] * Cdot_Tangent;
			
			
			constraint.m_TangentImpulse = CLAMP(oldImpulseMag + correctiveImpulseMag, -maxFriction, maxFriction);
			
			correctiveImpulseMag = constraint.m_TangentImpulse - oldImpulseMag;

			correctiveImpulse = tangent * correctiveImpulseMag;
			
			// Apply Corrective impulse on the bodies due to Tangent/Frictional Impulses
			body1->m_Velocity        += correctiveImpulse * body1->m_InvMass;
			body1->m_AngularVelocity += body1->m_InvI * Vector2f::cross( r1, correctiveImpulse);
			
			body2->m_Velocity        -= correctiveImpulse * body2->m_InvMass;
			body2->m_AngularVelocity -= body2->m_InvI * Vector2f::cross( r2, correctiveImpulse);
		}
		contact = contact->m_Next;
	}
}
	
END_NAMESPACE_DIRAC2D