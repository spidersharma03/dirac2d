//
//  TerrainGenerator.h
//  Dirac2D
//
//  Created by Prashant Sharma on 28/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include "IUpdatable.h"
#include "IRenderable.h"

#ifndef _TERRAIN_GENERATOR_H_
#define _TERRAIN_GENERATOR_H_

// B Spline Based Terrain Generator.

class TerrainGenerator : public IUpdatable, IRenderable
{
public:
    TerrainGenerator();
    
    virtual void update(float dt);
    
    virtual void render();
};

#endif
