//
//  Block.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/31/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Block.hpp"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Renderer.h"
#include "Game.h"
Block::Block(Game* game, Actor* parent):Actor(game, parent){
    mc = new MeshComponent(this);
    mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
    mCollision = new CollisionComponent(this);
    mCollision->SetSize(1, 1, 1);
    game->AddBlock(this);
}
Block::~Block(){
    GetGame()->RemoveBlock(this);
}
void Block::OnUpdate(float deltaTime){
    if(rotate){
        SetRotation(GetRotation() + Math::Pi/4*deltaTime);
    }
}
