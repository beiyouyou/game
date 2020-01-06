//
//  GoombaMove.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 9/19/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "GoombaMove.hpp"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
#include "Actor.h"
#include "Game.h"
#include "Block.hpp"
#include "Goomba.hpp"
#include <cmath>
GoombaMove::GoombaMove(Goomba* owner)
:MoveComponent(owner)
{
    this->seconds = 0.0f;
    this->goomba = owner;
    mYSpeed = 0.0f;
    SetForwardSpeed(-100.0f);
}
void GoombaMove::Update(float deltaTime){
    if(goomba->Stomped){
        goomba->AComponent->SetAnimation("dead");
        seconds += deltaTime;
        if(seconds >= 0.25f){
            goomba->SetState(ActorState::Destroy);
        }
        return;
    }
    float xDirection = GetForwardSpeed()*deltaTime;
    float yDirection = mYSpeed*deltaTime;
    goomba->SetPosition(Vector2(goomba->GetPosition().x+xDirection, goomba->GetPosition().y + yDirection));
    mYSpeed += deltaTime*2000.0f;
    //detect block conflict
    for(Block* block: mOwner->GetGame()->mBlock){
        Vector2 offset = Vector2(0,0);
        CollSide cSide= goomba->mCollision->GetMinOverlap(block->mCollision, offset);
        if(cSide == CollSide::None){
            continue;
        }
        if(cSide == CollSide::Top && mYSpeed>=0.0f){
            inAir = false;
            mYSpeed = 0.0f;
        }
        else if(cSide == CollSide::Left || cSide == CollSide::Right){
            if(!inAir){
            SetForwardSpeed(GetForwardSpeed()*-1);
            }
        }
        goomba->SetPosition(goomba->GetPosition() + offset);
    }
    if(mYSpeed!=0.0f){
        inAir = true;
    }
    for(Goomba* g: mOwner->GetGame()->mEnermy){
        if(goomba == g)continue;
        Vector2 offset2 = Vector2(0,0);
        CollSide cSide= goomba->mCollision->GetMinOverlap(g->mCollision, offset2);
        if(cSide == CollSide::None){
            continue;
        }
        if(cSide == CollSide::Left || cSide == CollSide::Right){
            SetForwardSpeed(GetForwardSpeed()*-1);
            goomba->SetPosition(goomba->GetPosition() + offset2);
        }
    }
    if(goomba->GetPosition().y >= 448.0f){
        goomba->SetState(ActorState::Destroy);
    }
}
