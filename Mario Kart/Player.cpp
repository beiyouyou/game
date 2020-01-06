//
//  Player.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/20/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Player.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Mesh.h"
#include "Renderer.h"
#include "PlayerMove.hpp"
Player::Player(Game* game):Actor(game){
    mc = new MeshComponent(this);
    mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Ship.gpmesh"));
    mcollision= new CollisionComponent(this);
    mcollision->SetSize(100, 40, 60);
    pMove = new PlayerMove(this);
}
