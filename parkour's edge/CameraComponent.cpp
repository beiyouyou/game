
//
//  CameraComponent.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/24/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "CameraComponent.hpp"
#include "Player.hpp"
#include "Game.h"
#include "Math.h"
#include "Renderer.h"
CameraComponent::CameraComponent(Player* mowner):Component(mowner){
    player = mowner;
    mPitchAngle = 0.0f;
    mPitchSpeed = 0.0f;
    this->position = Vector3(0,0,0);
    this->Velocity = Vector3(0,0,0);
    mRollAngle = 0.0F;
    rotationSide = NullRotation;
    preRotation = NullRotation;
}
void CameraComponent::Update(float deltaTime){
    Vector3 eyePos = player->GetPosition();
    mPitchAngle += mPitchSpeed*deltaTime;
    if(mPitchAngle > Math::PiOver2/2){
        mPitchAngle = Math::PiOver2/2;
    }
    if(mPitchAngle < -Math::PiOver2/2){
        mPitchAngle = -Math::PiOver2/2;
    }
    Matrix4 roll;
    if(rotationSide == NullRotation){
        if(mRollAngle > 0){
            mRollAngle -= (deltaTime/0.2f)*mRollAngle;
            if(mRollAngle <0){
                mRollAngle = 0;
            }
        }
        else if(mRollAngle<0){
             mRollAngle -= (deltaTime/0.2f)*mRollAngle;
            if(mRollAngle >0){
                mRollAngle = 0;
            }
        }
        if(preRotation == Left || preRotation == Right){
            roll = Matrix4::CreateRotationX(mRollAngle);
        }
        else{
            roll = Matrix4::CreateRotationY(mRollAngle);
        }
    }
    else if(rotationSide == Left){
        mRollAngle += (deltaTime/0.2f)*Math::PiOver2/2;
        if(mRollAngle > Math::PiOver2/2){
            mRollAngle = Math::PiOver2/2;
        }
        roll = Matrix4::CreateRotationX(mRollAngle);
    }
    else if(rotationSide == Right){
        mRollAngle += (deltaTime/0.2f)*-Math::PiOver2/2;
        if(mRollAngle < -Math::PiOver2/2){
            mRollAngle = -Math::PiOver2/2;
        }
        roll = Matrix4::CreateRotationX(mRollAngle);
    }
    else if(rotationSide == Front){
        mRollAngle += (deltaTime/0.2f)*Math::PiOver2/2;
        if(mRollAngle > Math::PiOver2/2){
            mRollAngle = Math::PiOver2/2;
        }
        roll = Matrix4::CreateRotationY(mRollAngle);
    }
    else{
        mRollAngle += (deltaTime/0.2f)*-Math::PiOver2/2;
        if(mRollAngle < -Math::PiOver2/2){
            mRollAngle = -Math::PiOver2/2;
        }
        roll = Matrix4::CreateRotationY(mRollAngle);
    }
    Vector3 newUp = Vector3::Transform(Vector3::UnitZ, roll);
    Matrix4 pitch = Matrix4::CreateRotationY(mPitchAngle);
    Matrix4 yaw = Matrix4::CreateRotationZ(mOwner->GetRotation());
    Vector3 target = Vector3::Transform(Vector3(1,0,0), pitch*yaw);
    Vector3 targetPos = target*TargetOffset + player->GetPosition();
    player->GetGame()->GetRenderer()->SetViewMatrix(Matrix4::CreateLookAt(eyePos, targetPos, newUp));
}
void CameraComponent::SetRotation(CameraRotation rside){
    preRotation = rotationSide;
    rotationSide = rside;
}
