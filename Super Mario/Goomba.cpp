//
//  Goomba.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 9/19/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Goomba.hpp"
#include "Game.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "GoombaMove.hpp"
Goomba::Goomba(Game* game)
:Actor(game)
{
    this->game = game;
    game->AddEnermy(this);
    AComponent = new AnimatedSprite(this, 150);
    std::vector<SDL_Texture*> walkAnim{
        GetGame()->GetTexture("Assets/Goomba/Walk0.png"),
        GetGame()->GetTexture("Assets/Goomba/Walk1.png")
    };
    std::vector<SDL_Texture*> deadAnim{
        GetGame()->GetTexture("Assets/Goomba/Dead.png"),
    };
    AComponent->AddAnimation("walk", walkAnim);
    AComponent->AddAnimation("dead", deadAnim);
    AComponent->SetAnimation("walk");
    mCollision = new CollisionComponent(this);
    mCollision->SetSize(32.0f, 32.0f);
    GMove = new GoombaMove(this);
    Stomped = false;
}
Goomba::~Goomba(){
    game->RemoveEnermy(this);
}
