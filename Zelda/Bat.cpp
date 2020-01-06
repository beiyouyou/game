//
//  Bat.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/7/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Bat.hpp"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "EnemyComponent.hpp"
#include "Random.h"
#include "Collider.hpp"
#include "Math.h"
#include "Game.h"
Bat::Bat(Game* game):Actor(game){
    mCollision = new CollisionComponent(this);
    mCollision->SetSize(25, 25);
    EComponent = new EnemyComponent(this);
    ASprite = new AnimatedSprite(this, 200);
    std::vector<SDL_Texture*> move{
        GetGame()->GetTexture("Assets/Bat0.png"),
        GetGame()->GetTexture("Assets/Bat1.png")
    };
    ASprite->AddAnimation("move", move);
    ASprite->SetAnimation("move");
    direction = Random::GetVector(Vector2(1,1), Vector2(-1,-1));
}
void Bat::OnUpdate(float deltaTime){
    if(time_elapse >= 3.0f){
        time_elapse -= 3.0f;
        direction = Random::GetVector(Vector2(1,1), Vector2(-1,-1));
    }
    direction.Normalize();
    //update position
    this->SetPosition(GetPosition() + direction*speed*deltaTime);
    for(Collider* collider: GetGame()->mCollider){
        Vector2 offset = Vector2(0,0);
        CollSide cSide = this->mCollision->GetMinOverlap(collider->mCollision, offset);
        if(cSide == CollSide::None)continue;
        else{
            this->SetPosition(GetPosition()+offset);
            direction = Random::GetVector(Vector2(1,1), Vector2(-1,-1));
            time_elapse = 0;
        }
    }
    time_elapse+=deltaTime;
}
