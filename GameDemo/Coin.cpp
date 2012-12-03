//
//  Coin.cpp
//  Dirac2D
//
//  Created by Prashant Sharma on 17/11/12.
//

#include "Coin.h"

Coin::Coin()
{
	m_ObjectInfo.m_ObjectType = EOT_COIN;
	m_Value = 0;
	m_pBody = 0;	
}

Coin::Coin(CoinInfo cInfo) : GameObject(cInfo)
{
    m_ObjectInfo.m_ObjectType = EOT_COIN;
	m_Value = 0;
	m_pBody = 0;
}

void Coin::update(float dt)
{
    
}