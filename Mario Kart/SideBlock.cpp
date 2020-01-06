//
//  SideBlock.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/21/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "SideBlock.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
SideBlock::SideBlock(Game* game):Actor(game){
    mScale = 500;
    mc = new MeshComponent(this);
    mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
    mc->SetTextureIndex(1);
}
void SideBlock::OnUpdate(float deltaTime){
    
}
