//
//  SimpleVehicle.h
//  Dirac2D
//
//  Created by Prashant Sharma on 27/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Vehicle.h"

#ifndef _SIMPLE_VEHICLE_H_
#define _SIMPLE_VEHICLE_H_

class SimpleVehicle : public Vehicle
{
public:
    SimpleVehicle();
    
    virtual void init();
    
    virtual void initFromFile(const char* fileName);
    
    virtual void update(float dt);
    
    virtual void render();
    
private:
    
    
};

#endif
