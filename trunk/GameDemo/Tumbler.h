/*
 *  Tumbler.h
 *  Dirac2D
 *
 *  Created by Prashant on 20/12/12.
 *
 */

#ifndef _TUMBLER_H_
#define _TUMBLER_H_

#include "GameObject.h"

BEGIN_NAMESPACE_DIRAC2D
class PhysicalBody;
class HingeConstraint;
END_NAMESPACE_DIRAC2D

USE_NAMESPACE_DIRAC2D

class Crate;
class FirstGame;

struct TumblerInfo : public GameObjectInfo
{
    TumblerInfo()
    {
        m_ObjectType = EOT_TUMBLER;
		m_Width = 0.2f;
		m_Height = 0.2f;
        m_NumObjects = 0;
    }
	
	float m_Width, m_Height;
    int m_NumObjects;
};

class Tumbler : public GameObject
{
public:
	virtual void update(float dt);
    
    void openLid();
    
    PhysicalBody* getPhysicalBody()
    {
        return m_pTumblerBody;
    }
private:
	Tumbler(TumblerInfo tInfo, FirstGame* pGame);
    	
	virtual ~Tumbler();
	
	friend class ObjectFactory;
private:
	
    PhysicalBody* m_pTumblerBody;
	PhysicalBody* m_pTumblerLidBody;
	
    HingeConstraint* m_pLidConstraint;
    
    int m_NumObjects;
    
	static FirstGame* m_pGame; 
	
public:
	static int m_TumblerCount;
};

#endif