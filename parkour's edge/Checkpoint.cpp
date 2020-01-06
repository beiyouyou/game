//
//  Checkpoint.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 11/21/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Checkpoint.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Player.hpp"
#include "HUD.hpp"
#include <iostream>
#include "CollisionComponent.h"
Checkpoint::Checkpoint(Game* game, Actor* parent):Actor(game, parent){
    mMesh = new MeshComponent(this);
    mMesh->SetMesh(game->GetRenderer()->GetMesh("Assets/Checkpoint.gpmesh"));
    mCollision = new CollisionComponent(this);
    mCollision->SetSize(25, 25, 25);
}
void Checkpoint::OnUpdate(float deltaTime){
    if(active){
        mMesh->SetTextureIndex(0);
        if(mCollision->Intersect(GetGame()->player->mCollision)){
            GetGame()->player->Respawn = GetPosition();
            GetGame()->player->mText->updateCheckPoint(GetGame()->mChecks.front()->mText);
            GetGame()->removeCheck();
            Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/Checkpoint.wav"), 0);
            if(!mLevelString.empty()){
                GetGame()->SetNextLevel(mLevelString);
                //std::cout<<"next level is"<<mLevelString<<"\n";
            }
            this->SetState(ActorState::Destroy);
            if(GetGame()->mChecks.size()>0){
                GetGame()->mChecks.front()->active = true;
            }
        }
    }
    else{
        mMesh->SetTextureIndex(1);
    }
    
}
