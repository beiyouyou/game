//
//  Spawner.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 9/19/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Spawner.hpp"
#include "Game.h"
#include "Math.h"
#include "Player.hpp"
#include "Goomba.hpp"
Spawner::Spawner(Game* game):Actor(game){
    this->game = game;
}
void Spawner::OnUpdate(float deltaTime){
    if(GetPosition().x - game->player->GetPosition().x <= 300.0f){
        Goomba* goomba = new Goomba(game);
        goomba->SetPosition(Vector2(GetPosition().x, GetPosition().y));
        mState = ActorState::Destroy;
    }
}
