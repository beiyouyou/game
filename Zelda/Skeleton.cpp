//
//  Skeleton.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/7/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Skeleton.hpp"
#include "CollisionComponent.h"
#include "EnemyComponent.hpp"
#include "AnimatedSprite.h"
#include "Fileball.hpp"
#include "Game.h"
#include "Random.h"
#include "Player.hpp"
#include "Math.h"
Skeleton::Skeleton(Game* game):Actor(game){
    mCollision = new CollisionComponent(this);
    mCollision->SetSize(25.0f, 25.0f);
    ASprite = new AnimatedSprite(this);
    std::vector<SDL_Texture*> move{
        GetGame()->GetTexture("Assets/Skeleton0.png"),
        GetGame()->GetTexture("Assets/Skeleton1.png")
    };
    ASprite->AddAnimation("move", move);
    ASprite->SetAnimation("move");
    EComponent = new EnemyComponent(this);
    cool_down = Random::GetFloatRange(0.0f, 2.0f);
}
void Skeleton::OnUpdate(float deltaTime){
    //fire the ball every 4 seconds
    if(cool_down>= 0){
        cool_down-=deltaTime;
        return;
    }
    if(time_elaspe >= 4){
        time_elaspe-= 4.0f;
        Fireball * fball = new Fireball(GetGame());
        fball->SetPosition(GetPosition());
        Vector2 distance = GetGame()->player->GetPosition() - GetPosition();
        fball->SetRotation(Math::Atan2(-distance.y, distance.x));
        Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/Fireball.wav"), 0);
    }
    time_elaspe+=deltaTime;
}
