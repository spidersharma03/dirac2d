//
//  Game.h
//  Dirac2D
//
//  Created by Prashant Sharma on 27/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "IUpdatable.h"

#ifndef _GAME_H_
#define _GAME_H_

class Game
{
public:
    Game()
    {
        
    }
    
    virtual void initGame() = 0;

    virtual void initGameFromFile(const char* fileName) = 0;

    virtual void gameLoop() = 0;
    
    virtual void render() = 0;
        
public:
    
    virtual void keyProcessor(unsigned char key, int x, int y){};
    
    
};

#endif
