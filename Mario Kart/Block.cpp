//
//  Block.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/21/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Block.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Renderer.h"
#include "Player.hpp"
Block::Block(Game* game): Actor(game){
    mScale = 25.0f;
    mc = new MeshComponent(this);
    mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
    mc->SetTextureIndex(2);
    mcollision = new CollisionComponent(this);
    mcollision->SetSize(1, 1, 1);
}
Block::~Block(){
    GetGame()->RemoveBlock(this);
}
void Block::OnUpdate(float deltaTime){
    if(GetGame()->player->GetPosition().x - GetPosition().x >2000.0f){
        this->SetState(ActorState::Destroy);
    }
}
void Block::SetExplode(bool explode){
    this->explode = explode;
    if(explode){
        mc->SetTextureIndex(4);
    }
}
