//
//  Collectible.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/6/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Collectible.hpp"
#include "CollisionComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Player.hpp"
#include <iostream>
Collectible::Collectible(Actor* owner)
:Component(owner)
{
    this->owner = owner;
    mOnCollect = []() { SDL_Log("call back initialized"); };
}
void Collectible::setOnCollect(std::function<void ()> onCollect){
    mOnCollect = onCollect;
}
void Collectible::Update(float deltatime){
    if(owner->GetComponent<CollisionComponent>()->Intersect(owner->GetGame()->player->mCollision)){
        owner->SetState(ActorState::Destroy);
        Mix_PlayChannel(-1, owner->GetGame()->GetSound("Assets/Sounds/Key.wav"), 0);
        if(mOnCollect){
            mOnCollect();
        }
    }
}
