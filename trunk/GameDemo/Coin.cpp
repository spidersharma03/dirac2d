//
//  Coin.cpp
//  Dirac2D
//
//  Created by Prashant Sharma on 17/11/12.
//

#include "Coin.h"

Coin::Coin(CoinInfo cInfo) : GameObject(cInfo)
{
    m_ObjectInfo.m_ObjectType = EOT_COIN;
}

void Coin::update(float dt)
{
    
}