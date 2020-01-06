//
//  BallMove.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 9/12/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "PlayerMove.hpp"
#include "CollisionComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Player.hpp"
#include "Block.hpp"
#include "AnimatedSprite.h"
#include "Goomba.hpp"
#include <iostream>
PlayerMove::PlayerMove(Player* owner)
:MoveComponent(owner)
{
    mYSpeed = 0.0f;
    Velocity = Vector2(0.0f, 0.0f);
    player = owner;
    mSpacePressed = false;
    mInAir = false;
    direction = 0;
}
void PlayerMove::Update(float deltaTime){
    if(player->GetPosition().y >=448.0f){
        player->ASprite->SetAnimation("dead");
        player->SetState(ActorState::Paused);
        Mix_HaltChannel(player->GetGame()->channel);
        Mix_PlayChannel(-1, player->GetGame()->GetSound("Assets/Sounds/Dead.wav"), 0);
        return;
    }
    if(player->GetPosition().x >=6368.0f){
        player->SetState(ActorState::Paused);
        Mix_HaltChannel(player->GetGame()->channel);
        Mix_PlayChannel(-1, player->GetGame()->GetSound("Assets/Sounds/StageClear.wav"), 0);
        return;
    }
    float xDirection = GetForwardSpeed()*deltaTime;
    float yDirection = mYSpeed*deltaTime;
    player->SetPosition(Vector2(player->GetPosition().x+xDirection, player->GetPosition().y + yDirection));
    mYSpeed += deltaTime*2000.0f;
    bool collision = false;
    //check if mario collid with walls.
    for(Block* block: mOwner->GetGame()->mBlock){
        Vector2 offset = Vector2(0,0);
        CollSide cSide= player->mCollision->GetMinOverlap(block->mCollision, offset);
        if(cSide == CollSide::None){
            continue;
        }
        else if(cSide == CollSide::Top && mYSpeed>=0.0f){
            mYSpeed = 0.0f;
            collision = true;
        }
        else if(cSide == CollSide::Bottom && mYSpeed<=0.0f){
            mYSpeed = 0.0f;
            Mix_PlayChannel(-1, player->GetGame()->GetSound("Assets/Sounds/Bump.wav"), 0);
            collision = true;
        }
        player->SetPosition(player->GetPosition() + offset);
    }
    //check if mario collids with enermy
    for(Goomba* goomba: player->GetGame()->mEnermy){
        Vector2 offset = Vector2(0,0);
        CollSide cSide= player->mCollision->GetMinOverlap(goomba->mCollision, offset);
        if(cSide == CollSide::None || goomba->Stomped){
            continue;
        }
        if(cSide == CollSide::Top){
            goomba->Stomped = true;
            mYSpeed = -350.0f;
            mInAir = true;
            Mix_PlayChannel(-1, player->GetGame()->GetSound("Assets/Sounds/Stomp.wav"), 0);
        }
        else if(cSide == CollSide::Left || cSide == CollSide::Right){
            if(mInAir){
                goomba->Stomped = true;
                mYSpeed = -350.0f;
                mInAir = true;
                Mix_PlayChannel(-1, player->GetGame()->GetSound("Assets/Sounds/Stomp.wav"), 0);
            }
            else{
                player->ASprite->SetAnimation("dead");
                player->SetState(ActorState::Paused);
                Mix_HaltChannel(player->GetGame()->channel);
                Mix_PlayChannel(-1, player->GetGame()->GetSound("Assets/Sounds/Dead.wav"), 0);
                return;
            }
        }
    }
    if(collision){
        mInAir = false;
    }
    else{
        mInAir = true;
    }
    SetForwardSpeed(0.0f);
    if(player->GetPosition().x <= player->GetGame()->Camera.x){
        player->SetPosition(Vector2(player->GetGame()->Camera.x, player->GetPosition().y));
    }
    player->GetGame()->Camera.x = player->GetPosition().x -300.0f > player->GetGame()->Camera.x?player->GetPosition().x -300.0f: player->GetGame()->Camera.x;
    //setup animation
    SetAnimation();
}
void PlayerMove::ProcessInput(const Uint8 *keyState){
    if(keyState[SDL_SCANCODE_LEFT]){
        SetForwardSpeed(-300.0f);
        direction = -1;
    }
    else if(keyState[SDL_SCANCODE_RIGHT]){
        SetForwardSpeed(300.0f);
        direction = 1;
    }
    else{
        direction = 0;
    }
    if(keyState[SDL_SCANCODE_SPACE]){
        if(!mSpacePressed && !mInAir){
            mYSpeed = -700.0f;
            Mix_PlayChannel(-1, player->GetGame()->GetSound("Assets/Sounds/Jump.wav"), 0);
        }
        mInAir = true;
        mSpacePressed = true;
    }
    if(!keyState[SDL_SCANCODE_SPACE]){
        mSpacePressed = false;
    }
}
void PlayerMove::SetAnimation(){
    if(!mInAir){
        if(direction == 1){
            player->ASprite->SetAnimation("runRight");
        }
        else if(direction == -1){
            player->ASprite->SetAnimation("runLeft");
        }
        else{
            player->ASprite->SetAnimation("idle");
        }
    }
    else{
        if(direction == 1){
            player->ASprite->SetAnimation("jumpRight");
        }
        else if(direction == -1){
            player->ASprite->SetAnimation("jumpLeft");
        }
        else{
            if(player->ASprite->GetAnimName().compare("jumpRight") == 0||
               player->ASprite->GetAnimName().compare("runRight")==0 ||
               player->ASprite->GetAnimName().compare("idle")== 0){
                player->ASprite->SetAnimation("jumpRight");
            }
            else{
                 player->ASprite->SetAnimation("jumpLeft");
            }
        }
    }
}


