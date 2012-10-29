//
//  FirstGame.h
//  Dirac2D
//
//  Created by Prashant Sharma on 27/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Game.h"

#ifndef _FIRST_GAME_H_
#define _FIRST_GAME_H_

class SimpleVehicle;

class FirstGame : public Game
{
public:
    FirstGame();
    
    virtual void initGame();
    
    virtual void initGameFromFile(const char* fileName);

    virtual void gameLoop();
    
    virtual void render();
    
    virtual void update(float dt);

public:
    virtual void keyProcessor(unsigned char key, int x, int y);
    
private:
    SimpleVehicle* m_pVehicle;
};

#endif
