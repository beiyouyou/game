//
//  Spawner.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 9/19/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Spawner.hpp"
#include "Game.h"
#include "Key.hpp"
#include "Bat.hpp"
#include "Math.h"
#include "Boss.hpp"
#include "Skeleton.hpp"
#include "Player.hpp"
#include "Triforce.hpp"
Spawner::Spawner(Game* game):Actor(game){
    this->game = game;
}
void Spawner::Trigger(){
    if(type == "Key"){
        Key* key = new Key(game);
        key->SetPosition(Vector2(GetPosition().x, GetPosition().y));
    }
    if(type == "Bat"){
        Bat* bat = new Bat(game);
        bat->SetPosition(Vector2(GetPosition().x, GetPosition().y));
    }
    if(type == "Skeleton"){
        Skeleton* skeleton = new Skeleton(game);
        skeleton->SetPosition(Vector2(GetPosition().x, GetPosition().y));
    }
    if(type == "Boss"){
        Boss* boss = new Boss(game);
        boss->SetPosition(GetPosition());
    }
    if(type == "Triforce"){
        Triforce* triforce = new Triforce(game);
        triforce->SetPosition(GetPosition());
    }
}
