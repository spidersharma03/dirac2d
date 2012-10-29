//
//  IUpdatable.h
//  Dirac2D
//
//  Created by Prashant Sharma on 27/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef _IUPDATABLE_H_
#define _IUPDATABLE_H_

class IUpdatable
{
public:
    virtual void update(float dt) = 0;
};

#endif
