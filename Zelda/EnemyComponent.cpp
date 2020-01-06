//
//  EnemyComponent.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/7/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "EnemyComponent.hpp"
#include "Actor.h"
#include "Game.h"
#include <iostream>
EnemyComponent::EnemyComponent(Actor* owner)
:Component(owner){
    this->owner = owner;
    owner->GetGame()->AddEnemy(owner);
    hit_point = 1;
    damage = 1;
    invulnerability_timer = 0;
    mOnDeath = []() { SDL_Log("call back initialized"); };
    mOnTakeDamage = []() { SDL_Log("call back initialized"); };
}
EnemyComponent::~EnemyComponent(){
    owner->GetGame()->RemoveEnemy(owner);
}
void EnemyComponent::setOnDeath(std::function<void ()> onCollect){
    mOnDeath = onCollect;
}
void EnemyComponent::setOnTakeDamage(std::function<void ()> onCollect){
    mOnTakeDamage = onCollect;
}
void EnemyComponent::TakeDamage(int damage){
    std::cout<<"invilnerability timer "<< invulnerability_timer <<"\n";
    if(this->invulnerability_timer <= 0){
        hit_point-= damage;
        if(hit_point > 0){
            mOnTakeDamage();
        }
        else{
            mOnDeath();
            owner->SetState(ActorState::Destroy);
            Mix_PlayChannel(-1,owner->GetGame()->GetSound("Assets/Sounds/EnemyDie.wav"), 0);
        }
        this->invulnerability_timer = 1.0f;
    }
}
