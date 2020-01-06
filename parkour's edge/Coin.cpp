//
//  Coin.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 11/27/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Coin.hpp"
#include "Game.h"
#include "CollisionComponent.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "HUD.hpp"
#include "Player.hpp"
Coin::Coin(Game* game, Actor* parent):Actor(game, parent){
    mc = new MeshComponent(this);
    mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Coin.gpmesh"));
    SetScale(0.15f);
    mCollision = new CollisionComponent(this);
    mCollision->SetSize(100, 100, 100);
}
void Coin::OnUpdate(float deltaTime){
    mRotation += deltaTime*Math::Pi;
    if(mCollision->Intersect(GetGame()->player->mCollision)){
        this->SetState(ActorState::Destroy);
        Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/Coin.wav"), 0);
        GetGame()->coin++;
        GetGame()->player->mText->addCoin();
    }
}
