//
//  Player.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/20/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Player.hpp"
#include "Game.h"
#include "HUD.hpp"
#include "CollisionComponent.h"
#include "Renderer.h"
#include "PlayerMove.hpp"
#include "CameraComponent.hpp"
Player::Player(Game* game, Actor* parent):Actor(game, parent){
    pMove = new PlayerMove(this);
    mCamera = new CameraComponent(this);
    this->mCollision = new CollisionComponent(this);
    mCollision->SetSize(50, 175, 50);
    mText = new HUD(this);
}
void Player::RespwanPlayer(){
    SetPosition(Respawn);
    SetRotation(0.0f);
    pMove->mVelocity = Vector3::Zero;
    pMove->mPendingForces = Vector3::Zero;
    pMove->mCurrentState = Falling;
}
