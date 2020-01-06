//
//  Triforce.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/8/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Triforce.hpp"
#include "Game.h"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
#include "Collectible.hpp"
#include "Player.hpp"
Triforce::Triforce(Game* game):Actor(game){
    std::vector<SDL_Texture*> anim{
        GetGame()->GetTexture("Assets/Triforce0.png"),
        GetGame()->GetTexture("Assets/Triforce1.png")
    };
    ASprite = new AnimatedSprite(this);
    ASprite->AddAnimation("anim", anim);
    ASprite->SetAnimation("anim");
    mCollision = new CollisionComponent(this);
    mCollision->SetSize(20.0f, 20.0f);
    collectible = new Collectible(this);
    collectible->setOnCollect([this]{
        Mix_HaltChannel( this->GetGame()->channel);
        Mix_PlayChannel(-1,  this->GetGame()->GetSound("Assets/Sounds/Triforce.ogg"), 0);
        this->GetGame()->player->SetState(ActorState::Paused);
    });
}
