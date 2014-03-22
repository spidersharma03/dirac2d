/*
 *  PathHurdle.h
 *  Dirac2D
 *
 *  Created by Prashant on 30/01/13.
 *
 */

#ifndef _PATH_HURDLE_H_
#define _PATH_HURDLE_H_


#include "GameObject.h"

BEGIN_NAMESPACE_DIRAC2D
class PhysicalBody;
END_NAMESPACE_DIRAC2D

USE_NAMESPACE_DIRAC2D

class FirstGame;


struct PathHurdleInfo : public GameObjectInfo
{
    PathHurdleInfo()
    {
        m_ObjectType = EOT_PATH_HURDLE;
		m_NumVertices = 4;
		m_Radius = 0.63f;
    }
	
	Vector2f m_Vertices[8];
	float m_Radius;
	int m_NumVertices;
};

class PathHurdle : public GameObject
{
public:
	virtual void update(float dt);
    
    PhysicalBody* getPhysicalBody()
    {
        return m_pBody;
    }
	
	virtual void onCollisionEnter( GameObject* pObject, ContactManifold& manifold );
	
	virtual void onCollisionExit( GameObject* pObject, ContactManifold& manifold );
	
private:
    PathHurdle(PathHurdleInfo pInfo, FirstGame* pGame);
    
	PathHurdle(FirstGame* pGame);
	
	virtual ~PathHurdle();
	
	friend class ObjectFactory;
private:
	
    PhysicalBody* m_pBody;
	static FirstGame* m_pGame;
	
public:
	static int m_PathHurdleCount;
};


#endif