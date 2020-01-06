//
//  Key.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/6/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Key.hpp"
#include "Game.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Collectible.hpp"
#include <iostream>
Key::Key(Game* game):Actor(game){
    mSprite = new SpriteComponent(this);
    mSprite->SetTexture(GetGame()->GetTexture("Assets/Key.png"));
    mCollision = new CollisionComponent(this);
    mCollision->SetSize(16.0f, 32.0f);
    collectible = new Collectible(this);
    collectible->setOnCollect([this]{
        this->GetGame()->key_count++;
    });
}
