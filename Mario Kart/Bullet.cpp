//
//  Bullet.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/21/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Bullet.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "MoveComponent.h"
#include "Renderer.h"
#include "Block.hpp"
#include "Math.h"
Bullet::Bullet(Game* game):Actor(game){
    mScale = 0.5f;
    mc = new MeshComponent(this);
    mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Sphere.gpmesh"));
    mCollision = new CollisionComponent(this);
    mCollision->SetSize(10, 10, 10);
    mMove = new MoveComponent(this);
}
void Bullet::OnUpdate(float deltaTime){
    if(time_elaspe > 1.0f){
        this->SetState(ActorState::Destroy);
    }
    SetPosition(GetPosition() + Vector3( mMove->GetForwardSpeed(),0,0)*deltaTime);
    for(Block* b: GetGame()->mBlock){
        if(b->mcollision->Intersect(mCollision)){
            if(!b->explode){
                this->SetState(ActorState::Destroy);
            }
            else{
                //chain explode
                Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/BlockExplode.wav"), 0);
                explode(b->GetPosition());
                this->SetState(ActorState::Destroy);
            }
            break;
        }
    }
    time_elaspe+=deltaTime;
}
void Bullet::explode(Vector3 Position ){
    std::string key = std::to_string((int)Position.x) + std::to_string((int)Position.y) + std::to_string((int)Position.z);
    if(GetGame()->bMap.find(key) == GetGame()->bMap.end()){
        return;
    }
    Block* b =GetGame()->bMap.at(key);
    if(b->GetState() ==ActorState::Destroy)return;
    if(!b->explode){
        b->SetState(ActorState::Destroy);
        return;
    }
    b->SetState(ActorState::Destroy);
    explode(Position + Vector3(0, -25, 0));
    explode(Position + Vector3(0, -50, 0));
    explode(Position + Vector3(0, 25, 0));
    explode(Position + Vector3(0, 50, 0));
    explode(Position + Vector3(0, 0, 25));
    explode(Position + Vector3(0, 0, 50));
    explode(Position + Vector3(0, 0, -25));
    explode(Position + Vector3(0, 0, -50));
    explode(Position + Vector3(0, -25, -25));
    explode(Position + Vector3(0, -25, 25));
    explode(Position + Vector3(0, 25, -25));
    explode(Position + Vector3(0, 25, 25));
}
