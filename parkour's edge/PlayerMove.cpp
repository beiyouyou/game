//
//  PlayerMove.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/20/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "PlayerMove.hpp"
#include "Actor.h"
#include "Player.hpp"
#include "Game.h"
#include "Block.hpp"
#include "CameraComponent.hpp"
#include "CameraComponent.hpp"
#include <iostream>
PlayerMove::PlayerMove(Player* owner):MoveComponent(owner){
    ChangeState(Falling);
    mRunningSFX = Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Running.wav"), -1);
    Mix_Pause(mRunningSFX);
}
void PlayerMove::ProcessInput(const Uint8 *keyState){
    if(keyState[SDL_SCANCODE_SPACE] && mCurrentState == OnGround && !SpacePressed){
        AddForce(mJumpForce);
        mCurrentState = Jump;
        Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Jump.wav"), 0);
        SpacePressed = true;
    }
    if(!keyState[SDL_SCANCODE_SPACE]){
        SpacePressed = false;
    }
    if(keyState[SDL_SCANCODE_W] && keyState[SDL_SCANCODE_S]){
        //mVelocity = Vector3::Zero;
    }
    else if(keyState[SDL_SCANCODE_S]){
        AddForce(mOwner->GetFoward()*-700);
    }
    else if(keyState[SDL_SCANCODE_W]){
        AddForce(mOwner->GetFoward()*700);
    }
    else{
       //mVelocity = Vector3::Zero;
    }
    if(keyState[SDL_SCANCODE_A] && keyState[SDL_SCANCODE_D]){
        //mVelocity = Vector3::Zero;
    }
    else if(keyState[SDL_SCANCODE_A]){
        AddForce(mOwner->GetRight()*-700);
    }
    else if(keyState[SDL_SCANCODE_D]){
        AddForce(mOwner->GetRight()*700);
    }
    else{
        //mVelocity = Vector3::Zero;
    }
    int x, y;
    SDL_GetRelativeMouseState(&x, &y);
    SetAngularSpeed((x/500.0f)*Math::Pi*10.0f);
    mOwner->GetComponent<CameraComponent>()->SetPitchSpeed((y/500.0f)*Math::Pi*10.0f);
}
void PlayerMove::Update(float deltaTime){
    if(mOwner->GetPosition().z < -750.0f){
        Player* p = (Player*)mOwner;
        mOwner->SetPosition(p->Respawn);
        mOwner->SetRotation(0.0f);
        mVelocity = Vector3::Zero;
        mPendingForces = Vector3::Zero;
        mCurrentState = Falling;
    }
    if(mCurrentState == Falling){
        //std::cout<<"falling \n";
        Mix_Pause(mRunningSFX);
        UpdateFalling(deltaTime);
    }
    else if(mCurrentState == Jump){
        //std::cout<<"jump \n";
        Mix_Pause(mRunningSFX);
        UpdateJump(deltaTime);
    }
    else if (mCurrentState == WallClimb){
        Mix_Resume(mRunningSFX);
        UpdateWallClimb(deltaTime);
    }
    else if(mCurrentState == WallRun){
        Mix_Resume(mRunningSFX);
        UpdateWallRun(deltaTime);
    }
    else{
        //std::cout<<"onGround \n";
        UpdateOnGround(deltaTime);
    }
}
void PlayerMove::ChangeState(MoveState mState){
    mCurrentState = mState;
}
void PlayerMove::UpdateJump(float deltaTime){
    AddForce(mGravity);
    PhysicsUpdate(deltaTime);
    bool hit_head = false;
    for(Block * b: mOwner->GetGame()->mBlocks){
        CollSide cside = FixCollision(mOwner->GetComponent<CollisionComponent>(), b->mCollision);
        if(cside== CollSide::Bottom)hit_head = true;
        if(cside== CollSide::SideX1 || cside== CollSide::SideX2 ||cside== CollSide::SideY1 ||cside== CollSide::SideY2){
            if(CanWallClimb(cside)){
                mCurrentState = WallClimb;
                 mWallClimbTimer = 0.0f;
                //std::cout<<"wall climb\n";
                return;
            }
            else if (CanWallRun(cside)){
                mCurrentState = WallRun;
                mWallRunTimer = 0.0f;
                //std::cout<<"wall run\n";
                if(cside == CollSide::SideY1){
                    mOwner->GetComponent<CameraComponent>()->SetRotation(CameraRotation::Left);
                }
                else if (cside == CollSide::SideY2){
                    mOwner->GetComponent<CameraComponent>()->SetRotation(CameraRotation::Right);
                }
                else if(cside == CollSide::SideX2){
                    mOwner->GetComponent<CameraComponent>()->SetRotation(CameraRotation::Front);
                }
                else if(cside == CollSide::SideX1){
                    mOwner->GetComponent<CameraComponent>()->SetRotation(CameraRotation::Back);
                }
                return;
            }
        }
    }
    if(hit_head)mVelocity.z = 0.0f;
    if(mVelocity.z <= 0.0f){
        mCurrentState = Falling;
    }
}
void PlayerMove::UpdateOnGround(float deltaTime){
    PhysicsUpdate(deltaTime);
    bool no_conflict = true;
    if(mVelocity.Length()>50.0f){
        Mix_Resume(mRunningSFX);
    }
    else{
        Mix_Pause(mRunningSFX);
    }
    for(Block * b: mOwner->GetGame()->mBlocks){
        CollSide cside = FixCollision(mOwner->GetComponent<CollisionComponent>(), b->mCollision);
        if(cside== CollSide::Top)no_conflict = false;
        if(cside== CollSide::SideX1 || cside== CollSide::SideX2 ||cside== CollSide::SideY1 ||cside== CollSide::SideY2){
            if(CanWallClimb(cside)){
                mCurrentState = WallClimb;
                mWallClimbTimer = 0.0f;
                return;
            }
        }
    }
    if(no_conflict) mCurrentState = Falling;
}
void PlayerMove::UpdateFalling(float deltaTime){
    //std::cout<< "postion" << mOwner->GetPosition().x << " " <<mOwner->GetPosition().y <<" "<<mOwner->GetPosition().z <<"\n";
    AddForce(mGravity);
    PhysicsUpdate(deltaTime);
    bool onGround = false;
    for(Block * b: mOwner->GetGame()->mBlocks){
        CollSide cside = FixCollision(mOwner->GetComponent<CollisionComponent>(), b->mCollision);
        if(cside == CollSide::Top){
            onGround = true;
        }
    }
    if(onGround){
        mVelocity.z = 0.0f;
        Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Land.wav"), 0);
        ChangeState(OnGround);
    }
}
void PlayerMove::UpdateWallClimb(float deltaTime){
    AddForce(mGravity);
    if(mWallClimbTimer < 0.4f){
        AddForce(WallClimbForce);
    }
    mWallClimbTimer+=deltaTime;
    PhysicsUpdate(deltaTime);
    bool noCollision = true;
    for(Block * b: mOwner->GetGame()->mBlocks){
        CollSide cside = FixCollision(mOwner->GetComponent<CollisionComponent>(), b->mCollision);
        if(cside!= CollSide::None)noCollision =false;
    }
    if(noCollision || mVelocity.z <= 0.0f){
        mVelocity.z = 0.0f;
        mCurrentState = Falling;
    }
}
void PlayerMove::UpdateWallRun(float deltaTime){
    AddForce(mGravity);
    if(mWallRunTimer < 0.4f){
        AddForce(mWallRunForce);
    }
    PhysicsUpdate(deltaTime);
    mWallRunTimer+=deltaTime;
    for(Block * b: mOwner->GetGame()->mBlocks){
        FixCollision(mOwner->GetComponent<CollisionComponent>(), b->mCollision);
    }
    if( mVelocity.z <= 0.0f){
        mVelocity.z = 0.0f;
        mCurrentState = Falling;
        mOwner->GetComponent<CameraComponent>()->SetRotation(CameraRotation::NullRotation);
    }
}
CollSide PlayerMove::FixCollision( CollisionComponent *self, CollisionComponent *block){
    if(!self->Intersect(block)){
        return CollSide::None;
    }
    Vector3 offset = Vector3(0,0,0);
    CollSide cside = self->GetMinOverlap(block, offset);
    mOwner->SetPosition(mOwner->GetPosition()+offset);
    if(cside == CollSide::SideX1){
         AddForce(Vector3(-700.0f,0,0));
    }
    else if(cside == CollSide::SideX2){
        AddForce(Vector3(700.0f,0,0));
    }
    else if(cside == CollSide::SideY1){
       AddForce(Vector3(0,-700.0f,0));
    }
    else if(cside == CollSide::SideY2){
        AddForce(Vector3(0,700.0f,0));
    }
    return cside;
}
void PlayerMove::PhysicsUpdate(float deltaTime){
    mAcceleration = mPendingForces*(1.0f/mMass);
    mVelocity+= mAcceleration*deltaTime;
    FixXYVelocity();
    mOwner->SetPosition(mOwner->GetPosition() + mVelocity*deltaTime);
    mOwner->SetRotation(mOwner->GetRotation() + GetAngularSpeed()*deltaTime);
    mPendingForces = Vector3::Zero;
}
void PlayerMove::FixXYVelocity(){
    Vector2 tempVe = Vector2(mVelocity.x, mVelocity.y);
    if(tempVe.Length() > 400.0f){
        tempVe.Normalize();
        tempVe*= 400.0f;
    }
    if(mCurrentState == OnGround || mCurrentState == WallClimb){
        if((mAcceleration.x)*tempVe.x < 0 ){
            tempVe.x *= 0.9f;
        }
        if((mAcceleration.y)*tempVe.y < 0){
             tempVe.y *= 0.9f;
        }
        if(Math::NearZero(mAcceleration.x)){
            tempVe.x *= 0.9f;
        }
        if(Math::NearZero(mAcceleration.y)){
            tempVe.y *=0.9f;
        }
    }
    mVelocity.x = tempVe.x;
    mVelocity.y = tempVe.y;
}
bool PlayerMove::CanWallClimb(CollSide cSide){
    Vector3 WallNorm;
    Vector3 xyVelocity;
    xyVelocity.x = mVelocity.x;
    xyVelocity.y = mVelocity.y;
    xyVelocity.z = 0;
    if(cSide == CollSide::SideX1){
         WallNorm = Vector3(1,0,0);
    }
    else if(cSide == CollSide::SideX2){
        WallNorm = Vector3(-1,0,0);
    }
    else if(cSide == CollSide::SideY1){
        WallNorm = Vector3(0,1,0);
    }
    else{
        WallNorm = Vector3(0,-1,0);
    }
    float dotProduct  =Vector3::Dot(WallNorm, mOwner->GetFoward());
    if(dotProduct > 0.8f && dotProduct<1.2f && Vector3::Dot(WallNorm, xyVelocity)>=0 && xyVelocity.Length()>350.0f){
        return true;
    }
    return false;
}
bool PlayerMove::CanWallRun(CollSide cSide){
    Vector3 WallNorm;
    Vector3 xyVelocity;
    xyVelocity.x = mVelocity.x;
    xyVelocity.y = mVelocity.y;
    xyVelocity.z = 0;
    if(cSide == CollSide::SideX1){
        WallNorm = Vector3(1,0,0);
    }
    else if(cSide == CollSide::SideX2){
        WallNorm = Vector3(-1,0,0);
    }
    else if(cSide == CollSide::SideY1){
        WallNorm = Vector3(0,1,0);
    }
    else{
        WallNorm = Vector3(0,-1,0);
    }
    float dotProduct  = Vector3::Dot(WallNorm, mOwner->GetFoward());
    if(dotProduct >=0 && dotProduct<=0.5f && Vector3::Dot(mOwner->GetFoward(), xyVelocity)>=0 && xyVelocity.Length()>350.0f){
        return true;
    }
    return false;
}

