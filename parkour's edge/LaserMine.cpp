//
//  LaserMine.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 11/17/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "LaserMine.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "LaserComponent.hpp"
#include "Renderer.h"
LaserMine::LaserMine(Game* game, Actor* parent):Actor(game, parent){
    mMesh = new MeshComponent(this);
    mMesh->SetMesh(game->GetRenderer()->GetMesh("Assets/LaserMine.gpmesh"));
    mLaser = new LaserComponent(this);
}
