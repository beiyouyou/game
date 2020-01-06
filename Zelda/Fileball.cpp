//
//  Fileball.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/7/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Fileball.hpp"
#include "CollisionComponent.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "Player.hpp"
Fireball::Fireball(Game * game):Actor(game){
    mCollision = new CollisionComponent(this);
    mCollision->SetSize(16.0f, 16.0f);
    mSprite = new SpriteComponent(this);
    mSprite->SetTexture(game->GetTexture("Assets/FireballRed.png"));
    MComponent = new MoveComponent(this);
    MComponent->SetForwardSpeed(200.0f);
}
void Fireball::OnUpdate(float deltaTime){
    if(this->mCollision->Intersect(GetGame()->player->mCollision)){
        GetGame()->player->TakeDamage(1);
        this->SetState(ActorState::Destroy);
    }
    if(time_elaspe >= 1.5f){
        this->SetState(ActorState::Destroy);
    }
    this->SetPosition(GetPosition() + GetFoward()*MComponent->GetForwardSpeed()*deltaTime);
    time_elaspe+=deltaTime;
}

