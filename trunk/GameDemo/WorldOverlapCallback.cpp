/*
 *  WorldOverlapCallback.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 13/12/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "WorldOverlapCallback.h"


WorldOverlapCallBack::WorldOverlapCallBack(FirstGame* pGame) : m_pGame(pGame)
{
}

// This Function will be called for any AABB overlap query against the world. 
void WorldOverlapCallBack::overlapCallBack(dint32 overlapNodeID, void* userData)
{
}
