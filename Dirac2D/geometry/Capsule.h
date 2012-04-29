/*
 *  Capsule.h
 *  Dirac2D
 *
 *  Created by Prashant on 24/04/12.
 *
 */

#ifndef _CAPSULE_H_
#define _CAPSULE_H_

BEGIN_NAMESPACE_DIRAC2D

class Capsule : public CollisionShape
{
public:
	Capsule();
	
	Capsule(const Capsule& other);
	
	virtual dfloat getArea();
	
	virtual Vector2f getSupportPoint(Vector2f& d);

protected:
	
	virtual void findCentroid();
	
protected:
	
	dfloat m_Radius;
	dfloat m_Height;
};

END_NAMESPACE_DIRAC2D

#endif