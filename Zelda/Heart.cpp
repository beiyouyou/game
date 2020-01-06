//
//  Heart.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/8/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Heart.hpp"
#include "Game.h"
#include "SpriteComponent.h"
Heart::Heart(Game* game):Actor(game){
    mSprite = new SpriteComponent(this);
    mSprite->SetTexture(game->GetTexture("Assets/HeartFull.png"));
}
