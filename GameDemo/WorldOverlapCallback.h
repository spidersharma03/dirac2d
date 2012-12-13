/*
 *  WorldOverlapCallback.h
 *  Dirac2D
 *
 *  Created by Prashant on 13/12/12.
 *
 */

#ifndef _WORLD_OVERLAP_CALL_BACK_H_
#define _WORLD_OVERLAP_CALL_BACK_H_

#include "../Dirac2D/Dirac2D.h"

USE_NAMESPACE_DIRAC2D

class FirstGame;


class WorldOverlapCallBack : public OverlapCallBackClass
{
public:
	WorldOverlapCallBack(FirstGame* pGame);

	virtual void overlapCallBack(dint32 overlapNodeID, void* userData);

protected:
	FirstGame* m_pGame;
};

#endif