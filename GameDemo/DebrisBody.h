//
//  DebrisBody.h
//  Dirac2D
//
//  Created by Prashant Sharma on 14/12/12.
//

#ifndef _DEBRIS_BODY_H_
#define _DEBRIS_BODY_H_

#include "../Dirac2D/definitions.h"

BEGIN_NAMESPACE_DIRAC2D
class PhysicalBody;
END_NAMESPACE_DIRAC2D

class FirstGame;

#include "GameObject.h"

enum { MAX_DEBRIS_VERTICS = 8 };

struct DebrisInfo : public GameObjectInfo
{
    DebrisInfo()
    {
        m_ObjectType = EOT_DEBRIS;
    }
    Vector2f vertices[MAX_DEBRIS_VERTICS]; 
    int numVertices;
};

class DebrisBody : public GameObject
{
public:
	virtual void update(float dt);
    
    PhysicalBody* getPhysicalBody()
    {
        return m_pBody;
    }
private:
    DebrisBody(DebrisInfo dInfo, FirstGame* pGame);
    	
    virtual void onCollisionEnter( GameObject* pObject, ContactManifold& manifold );
    
    virtual void onCollisionExit( GameObject* pObject, ContactManifold& manifold );
    
	virtual ~DebrisBody();
	
	friend class ObjectFactory;
    
private:
    int m_BounceCount;
private:
    PhysicalBody* m_pBody;
	static FirstGame* m_pGame;
public:
	static int m_DebrisCount;
};

#endif
