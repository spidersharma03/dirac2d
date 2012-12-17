//
//  IGameCollisionListener.h
//  Dirac2D
//
//  Created by Prashant Sharma on 14/12/12.
//

#ifndef _IGAME_COLLISION_LISTENER_H_
#define _IGAME_COLLISION_LISTENER_H_

BEGIN_NAMESPACE_DIRAC2D
struct ContactManifold;
END_NAMESPACE_DIRAC2D

USE_NAMESPACE_DIRAC2D

class GameObject;

class IGameCollisionListener
{
public:
    virtual void onCollisionEnter( GameObject* pObject, ContactManifold& manifold ) = 0;
    
    virtual void onCollisionExit( GameObject* pObject, ContactManifold& manifold ) = 0;
};

#endif
