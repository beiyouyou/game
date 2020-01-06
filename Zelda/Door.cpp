//
//  Door.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/6/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Door.hpp"
#include "Game.h"
#include "CollisionComponent.h"
#include "SpriteComponent.h"
#include <iostream>
Door::Door(Game* game):Actor(game){
    mCollision = new CollisionComponent(this);
    mSprite = new SpriteComponent(this);
}
void Door::Setup(std::string destination, std::string direction, std::string state){
    this->destination = destination;
    this->state = state;
    this->direction = direction;
    if(direction == "Up" || direction == "Down"){
        mCollision->SetSize(30.0f, 64.0f);
    }
    else{
        mCollision->SetSize(64.0f, 30.0f);
    }
    UpdateTexture();
}
void Door::UpdateTexture(){
    std::string fileToRead = "Assets/Door/";
    fileToRead += direction + state +".png";
    mSprite->SetTexture(GetGame()->GetTexture(fileToRead));
}
