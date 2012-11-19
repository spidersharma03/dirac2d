//
//  Coin.h
//  Dirac2D
//
//  Created by Prashant Sharma on 17/11/12.
//

#include "GameObject.h"
#include "../Dirac2D/definitions.h"

#ifndef _COIN_H_
#define _COIN_H_

BEGIN_NAMESPACE_DIRAC2D
class PhysicalBody;
END_NAMESPACE_DIRAC2D

class Coin : public GameObject
{
public:
    Coin();
    
    virtual void update(float dt);
    
    PhysicalBody* getPhysicalBody()
    {
        return m_pBody;
    }
private:
    PhysicalBody* m_pBody;
};

#endif
