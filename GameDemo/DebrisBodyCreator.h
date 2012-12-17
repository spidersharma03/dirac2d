//
//  DebrisBodyCreator.h
//  Dirac2D
//
//  Created by Prashant Sharma on 15/12/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef _DEBRIS_BODY_CREATOR_H_
#define _DEBRIS_BODY_CREATOR_H_

#include "../Dirac2D/Dirac2D.h"

USE_NAMESPACE_DIRAC2D

class FirstGame;
struct GameObjectInfo;

class DebrisBodyCreator
{
public:
    DebrisBodyCreator(FirstGame* pGame);
    
    void createDebrisBodies( const GameObjectInfo& gInfo, const Vector2f& position, int numBodies );
    
private:
    void createCrateDebris(const Vector2f& position, int numBodies );
    
private:
    FirstGame* m_pGame;
};

#endif
