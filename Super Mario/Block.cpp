//
//  Block.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 9/12/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//
#include "Game.h"
#include "Block.hpp"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
Block::Block(Game* game)
    :Actor(game)
    {
        this->game = game;
        game->AddBlock(this);
        sComponent = new SpriteComponent(this);
        mCollision = new CollisionComponent(this);
        mCollision->SetSize(32.0f, 32.0f);
    }
Block::~Block(){
    game->RemoveBlock(this);
}
void Block::ChangeTexture(SDL_Texture *texture){
    sComponent->SetTexture(texture);
}
