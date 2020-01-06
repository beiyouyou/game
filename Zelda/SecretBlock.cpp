//
//  SecretBlock.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/6/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "SecretBlock.hpp"
#include "CollisionComponent.h"
#include "SpriteComponent.h"
#include "string"
#include "Game.h"
SecretBlock::SecretBlock(Game* game):Actor(game){
    mSprite = new SpriteComponent(this);
    mSprite->SetTexture(GetGame()->GetTexture("Assets/SecretBlock.png"));
    mCollision = new CollisionComponent(this);
    mCollision->SetSize(32.0f, 32.0f);
}
void SecretBlock::SetDirection(std::string direction){
    this->Movedirection = direction;
    if(direction == "Up"){
        destination = GetPosition().y + 32.0f;
    }
    else if(direction == "Down"){
        destination = GetPosition().y - 32.0f;
    }
    else if (direction =="Left"){
        destination = GetPosition().x + 32.0f;
    }
    else if(direction == "Right"){
        destination = GetPosition().x - 32.0f;
    }
}
