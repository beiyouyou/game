//
//  Collider.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 9/29/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Collider.hpp"
#include "CollisionComponent.h"
#include "Game.h"
Collider::Collider(Game* game):Actor(game)
{
    this->mCollision = new CollisionComponent(this);
}
void Collider::setWidthHeight(float width, float height){
    this->mCollision->SetSize(width, height);
}
