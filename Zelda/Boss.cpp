//
//  Boss.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/8/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Boss.hpp"
#include "AnimatedSprite.h"
#include "EnemyComponent.hpp"
#include "Game.h"
#include "Door.hpp"
#include "Fileball.hpp"
#include "CollisionComponent.h"
#include <iostream>
Boss::Boss(Game* game):Actor(game){
    mCollision = new CollisionComponent(this);
    mCollision->SetSize(50.0f, 64.0f);
    ASprite = new AnimatedSprite(this);
    std::vector<SDL_Texture*> idle{
        GetGame()->GetTexture("Assets/Dragon/Idle0.png"),
        GetGame()->GetTexture("Assets/Dragon/Idle1.png")
    };
    std::vector<SDL_Texture*> attack{
        GetGame()->GetTexture("Assets/Dragon/Attack0.png"),
        GetGame()->GetTexture("Assets/Dragon/Attack1.png")
    };
    std::vector<SDL_Texture*> enraged{
        GetGame()->GetTexture("Assets/Dragon/Enraged0.png"),
        GetGame()->GetTexture("Assets/Dragon/Enraged1.png"),
        GetGame()->GetTexture("Assets/Dragon/Enraged2.png"),
        GetGame()->GetTexture("Assets/Dragon/Enraged3.png")
    };
    ASprite->AddAnimation("idle",idle);
    ASprite->AddAnimation("attack", attack);
    ASprite->AddAnimation("enraged", enraged);
    ASprite->SetAnimation("idle");
    EComponent = new EnemyComponent(this);
    EComponent->hit_point = 5;
    EComponent->damage = 2;
    EComponent->setOnTakeDamage([this]{
        Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/BossHit.wav"), 0);
    });
    EComponent->setOnDeath([this]{
        Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/BossDie.wav"), 0);
        Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/DoorOpen.wav"), 0);
        for(Door* door: GetGame()->RoomDoors.at(GetGame()->room)){
            if(door->state == "Closed"){
                door->Setup(door->destination, door->direction, "Open");
            }
            for(Door* door2: GetGame()->RoomDoors.at(door->destination)){
                door2->Setup(door2->destination, door2->direction, "Open");
            }
        }
    });
}
void Boss::OnUpdate(float deltaTime){
    EComponent->invulnerability_timer -= deltaTime;
    if(EComponent->hit_point>2){
        if(attack_period + 0.25f >= 2.0f){
            ASprite->SetAnimation("attack");
        }
        if(attack_period>2.0f){
            attack_period = fmod(attack_period, 2.0f);
            Fireball* fireBall1 = new Fireball(GetGame());
            Fireball* fireBall2 = new Fireball(GetGame());
            Fireball* fireBall3 = new Fireball(GetGame());
            fireBall1->mSprite->SetTexture(GetGame()->GetTexture("Assets/FireballGreen.png"));
            fireBall2->mSprite->SetTexture(GetGame()->GetTexture("Assets/FireballGreen.png"));
            fireBall3->mSprite->SetTexture(GetGame()->GetTexture("Assets/FireballGreen.png"));
            fireBall1->SetPosition(GetPosition());
            fireBall2->SetPosition(GetPosition());
            fireBall3->SetPosition(GetPosition());
            fireBall1->SetRotation(Math::Atan2(-1, -2));
            fireBall2->SetRotation(Math::Atan2(0, -1));
            fireBall3->SetRotation(Math::Atan2(1, -2));
            Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/Fireball.wav"), 0);
            ASprite->SetAnimation("idle");
        }
    }
    else{
        ASprite->SetAnimation("enraged");
        if(attack_period>1.0f){
            attack_period = fmod(attack_period, 1.0f);
            float degree = 90;
            for(int i = 0; i < 9; i++){
                Fireball* fireBall = new Fireball(GetGame());
                fireBall->mSprite->SetTexture(GetGame()->GetTexture("Assets/FireballGreen.png"));
                fireBall->SetPosition(GetPosition());
                fireBall->SetRotation(degree*PI/180.0f);
                degree += 20;
            }
            Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/Fireball.wav"), 0);
        }
    }
    attack_period+= deltaTime;
}
