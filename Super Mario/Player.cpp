//
//  Ball.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 9/12/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Player.hpp"
#include "PlayerMove.hpp"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "Game.h"
Player::Player(Game* game):Actor(game){
    pMove = new PlayerMove(this);
    ASprite = new AnimatedSprite(this, 200);
    std::vector<SDL_Texture*> idleAnim{
        GetGame()->GetTexture("Assets/Mario/Idle.png"),
    };
    std::vector<SDL_Texture*> deadAnim{
        GetGame()->GetTexture("Assets/Mario/Dead.png"),
    };
    std::vector<SDL_Texture*> jumpLeftAnim{
        GetGame()->GetTexture("Assets/Mario/JumpLeft.png"),
    };
    std::vector<SDL_Texture*> jumpRightAnim{
        GetGame()->GetTexture("Assets/Mario/JumpRight.png"),
    };
    std::vector<SDL_Texture*> runLeftAnim{
        GetGame()->GetTexture("Assets/Mario/RunLeft0.png"),
        GetGame()->GetTexture("Assets/Mario/RunLeft1.png"),
        GetGame()->GetTexture("Assets/Mario/RunLeft2.png")
    };
    std::vector<SDL_Texture*> runRightAnim{
        GetGame()->GetTexture("Assets/Mario/RunRight0.png"),
        GetGame()->GetTexture("Assets/Mario/RunRight1.png"),
        GetGame()->GetTexture("Assets/Mario/RunRight2.png")
    };
    ASprite->AddAnimation("idle", idleAnim);
    ASprite->AddAnimation("dead", deadAnim);
    ASprite->AddAnimation("jumpLeft", jumpLeftAnim);
    ASprite->AddAnimation("jumpRight", jumpRightAnim);
    ASprite->AddAnimation("runLeft", runLeftAnim);
    ASprite->AddAnimation("runRight", runRightAnim);
    ASprite->SetAnimation("idle");
    mCollision = new CollisionComponent(this);
    mCollision->SetSize(32.0f, 32.0f);
}
