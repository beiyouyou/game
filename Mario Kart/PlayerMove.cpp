//
//  PlayerMove.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/20/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "PlayerMove.hpp"
#include "Actor.h"
#include "Player.hpp"
#include "Game.h"
#include "Renderer.h"
#include "Bullet.hpp"
#include "Block.hpp"
#include "CollisionComponent.h"
#include <iostream>
PlayerMove::PlayerMove(Player* owner):MoveComponent(owner){
    velocity = Vector3(400,0,0);
    player = owner;
}
void PlayerMove::ProcessInput(const Uint8 *keyState){
    if((keyState[SDL_SCANCODE_W] && keyState[SDL_SCANCODE_S]) || (keyState[SDL_SCANCODE_A]&&keyState[SDL_SCANCODE_D])){
        direction = Vector3(0,0,0);
    }
    else if(keyState[SDL_SCANCODE_W]){
        direction = Vector3(0,0,300);
    }
    else if(keyState[SDL_SCANCODE_S]){
        direction = Vector3(0,0,-300);
    }
    else if(keyState[SDL_SCANCODE_A]){
        direction = Vector3(0,-300,0);
    }
    else if(keyState[SDL_SCANCODE_D]){
        direction = Vector3(0,300,0);
    }
    else{
        direction = Vector3(0,0,0);
    }
    if(keyState[SDL_SCANCODE_SPACE]){
        if(!leading_edge){
            Bullet* bullet = new Bullet(player->GetGame());
            bullet->SetPosition(player->GetPosition());
            bullet->mMove->SetForwardSpeed(500.0f + velocity.x);
            leading_edge = true;
             Mix_PlayChannel(-1, player->GetGame()->GetSound("Assets/Sounds/Shoot.wav"), 0);
        }
    }
    else{
        leading_edge = false;
    }
}
void PlayerMove::Update(float deltaTime){
    if(second > 10.0f){
        second-= 10.0f;
        multiplier+= 0.15f;
        velocity= Vector3(multiplier*base_Speed,0,0);
    }
    player->SetPosition(player->GetPosition() + velocity*deltaTime);
    Vector3 newPo =player->GetPosition() + direction*deltaTime;
    if(newPo.y > 195.0f){
        newPo.y = 195.0f;
    }
    if(newPo.y < -195.0f){
        newPo.y = -195.0f;
    }
    if(newPo.z < -225.0f){
        newPo.z = -225.0f;
    }
    if(newPo.z > 225.0f){
        newPo.z = 225.0f;
    }
    player->SetPosition(newPo);
    for(Block* b: player->GetGame()->mBlock){
        if(player->mcollision->Intersect(b->mcollision)){
            player->SetState(ActorState::Paused);
            Mix_HaltChannel(player->GetGame()->channel);
            Mix_PlayChannel(-1, player->GetGame()->GetSound("Assets/Sounds/ShipDie.wav"), 0);
        }
    }
    if(player->GetPosition().x > block_Create){
        block_Create+=500;
        player->GetGame()->AddBlock();
    }
    if(player->GetPosition().x > game_block_Create){
        game_block_Create+=1000;
        player->GetGame()->LoadBlock();
    }
    if(player->GetPosition().x > block_destroy){
        block_destroy+=500;
        player->GetGame()->RemoveBlock();
    }
    //calculate camera position
    Vector3 targetPos = player->GetPosition() + Vector3(20, 0, 0);
    Vector3 eyePos = player->GetPosition() - Vector3(300, 0 ,0) + Vector3(0, 0, 100);
    player->GetGame()->GetRenderer()->SetViewMatrix(Matrix4::CreateLookAt(eyePos, targetPos, Vector3::UnitZ));
    second+= deltaTime;
}
