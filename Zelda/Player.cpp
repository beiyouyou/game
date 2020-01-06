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
#include "Heart.hpp"
#include "Game.h"
Player::Player(Game* game):Actor(game){
    pMove = new PlayerMove(this);
    ASprite = new AnimatedSprite(this, 200);
    player_hit_point = 20;
    for(int i = 0; i < player_hit_point/2; i++){
        game->AddHeart(new Heart(game));
    }
    std::vector<SDL_Texture*> attackUp{
        GetGame()->GetTexture("Assets/Link/AttackUp.png"),
    };
    std::vector<SDL_Texture*> attackDown{
        GetGame()->GetTexture("Assets/Link/AttackDown.png"),
    };
    std::vector<SDL_Texture*> attackLeft{
        GetGame()->GetTexture("Assets/Link/AttackLeft.png"),
    };
    std::vector<SDL_Texture*> attackRight{
        GetGame()->GetTexture("Assets/Link/AttackRight.png"),
    };
    std::vector<SDL_Texture*> WalkLeftAnim{
        GetGame()->GetTexture("Assets/Link/WalkLeft0.png"),
        GetGame()->GetTexture("Assets/Link/WalkLeft1.png")
    };
    std::vector<SDL_Texture*> WalkDownAnim{
        GetGame()->GetTexture("Assets/Link/WalkDown0.png"),
        GetGame()->GetTexture("Assets/Link/WalkDown1.png")
    };    std::vector<SDL_Texture*> WalkUpAnim{
        GetGame()->GetTexture("Assets/Link/WalkUp0.png"),
        GetGame()->GetTexture("Assets/Link/WalkUp1.png")
    };    std::vector<SDL_Texture*> WalkRightAnim{
        GetGame()->GetTexture("Assets/Link/WalkRight0.png"),
        GetGame()->GetTexture("Assets/Link/WalkRight1.png")
    };
    ASprite->AddAnimation("walkup", WalkUpAnim);
    ASprite->AddAnimation("walkdown", WalkDownAnim);
    ASprite->AddAnimation("walkleft", WalkLeftAnim);
    ASprite->AddAnimation("walkright", WalkRightAnim);
    ASprite->AddAnimation("attackUp", attackUp);
    ASprite->AddAnimation("attackDown", attackDown);
    ASprite->AddAnimation("attackLeft", attackLeft);
    ASprite->AddAnimation("attackRight", attackRight);
    ASprite->SetAnimation("walkup");
    ASprite->SetIsPaused(true);
    mCollision = new CollisionComponent(this);
    mCollision->SetSize(20.0f, 20.0f);
}
void Player::TakeDamage(int amount){
    if(invulnerability_timer <= 0){
        player_hit_point -= amount;
        if(player_hit_point<= 0){
            this->SetState(ActorState::Paused);
            Mix_HaltChannel(GetGame()->channel);
            Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/LinkDie.wav"), 0);
        }
        else{
            Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/LinkHit.wav"), 0);
        }
        long index = GetGame()->mHeart.size()-1;
        Heart* heart = GetGame()->mHeart.at(index);
        if(heart->Full){
            if(amount == 1){
                GetGame()->mHeart.at(index)->mSprite->SetTexture(GetGame()->GetTexture("Assets/HeartHalf.png"));
                GetGame()->mHeart.at(index)->Full = false;
            }
            else{
                GetGame()->mHeart.at(index)->SetState(ActorState::Destroy);
                GetGame()->RemoveHeart();
            }
        }
        else{
            if(amount == 1){
                GetGame()->mHeart.at(index)->SetState(ActorState::Destroy);
                GetGame()->RemoveHeart();
            }
            else{
                GetGame()->mHeart.at(index)->SetState(ActorState::Destroy);
                GetGame()->RemoveHeart();
                index--;
                if(index>=0){
                    GetGame()->mHeart.at(index)->mSprite->SetTexture(GetGame()->GetTexture("Assets/HeartHalf.png"));
                    GetGame()->mHeart.at(index)->Full = false;
                }
            }
        }
        invulnerability_timer = 0.5f;
    }
}
