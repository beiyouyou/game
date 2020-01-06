//
//  Sword.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/7/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Sword.hpp"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Actor.h"
Sword::Sword(Game* game):Actor(game){
    mCollision = new CollisionComponent(this);
    mCollision->SetSize(25.0f, 25.0f);
    mSprite = new SpriteComponent(this);
    mSprite->SetTexture(game->GetTexture("Assets/Sword/Up.png"));
}
