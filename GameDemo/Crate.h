/*
 *  Crate.h
 *  Dirac2D
 *
 *  Created by Prashant on 07/12/12.
 *
 */

#ifndef _CRATE_H_
#define _CRATE_H_

#include "GameObject.h"

BEGIN_NAMESPACE_DIRAC2D
class PhysicalBody;
END_NAMESPACE_DIRAC2D

USE_NAMESPACE_DIRAC2D

class FirstGame;

enum CRATE_SHAPE { ECS_BOX, ECS_CIRCLE, ECS_REGULAR_POLY, ECS_POLY };

struct CrateInfo : public GameObjectInfo
{
    CrateInfo()
    {
        m_ObjectType = EOT_CRATE;
		m_ShapeType = ECS_BOX;
		m_Width = 0.2f;
		m_Height = 0.2f;
		m_Radius = 0.1f;
		m_NumVertices = 0;
    }
	
	CRATE_SHAPE m_ShapeType;
	Vector2f m_Vertices[8];
	int m_NumVertices;
	float m_Radius, m_Width, m_Height;
};

class Crate : public GameObject
{
public:
	virtual void update(float dt);
    
    PhysicalBody* getPhysicalBody()
    {
        return m_pBody;
    }
private:
    Crate(CrateInfo cInfo, FirstGame* pGame);
    
	Crate(FirstGame* pGame);
	
	virtual ~Crate();
	
    void onCollisionEnter( GameObject* pObject, ContactManifold& manifold );
    
    void onCollisionExit( GameObject* pObject, ContactManifold& manifold );
    
	friend class ObjectFactory;
private:
	
    PhysicalBody* m_pBody;
	static FirstGame* m_pGame;
	
public:
	static int m_CrateCount;
};


#endif