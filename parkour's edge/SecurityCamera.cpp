//
//  SecurityCamera.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 12/3/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "SecurityCamera.hpp"
#include "MeshComponent.h"
#include "Renderer.h"
#include "SecurityCone.hpp"
#include "Game.h"
#include "Player.hpp"
#include <iostream>
SecurityCamera::SecurityCamera(Game* game,  Actor* parent):Actor(game, parent){
    mc = new MeshComponent(this);
    mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Camera.gpmesh"));
    sc = new SecurityCone(game, this);
    SetQuaternion(startQ);
    game->AddCamera(this);
}
SecurityCamera::~SecurityCamera(){
    if(rotateSound != -1){
        Mix_HaltChannel(rotateSound);
    }
    GetGame()->RemoveCamera(this);
}
void SecurityCamera::OnUpdate(float deltaTime){
    //calculate the sounds;
    Vector3 dist = GetPosition() - GetGame()->player->GetPosition();
    float dis =dist.Length();
    if(dis > 1500.0f){
        volume = 0;
    }
    else if(dis < 500.0f){
        volume = 128;
    }
    else{
        volume = 128 - 128*((dis-500)/1000);
    }
    if(rotateSound!=-1){
        Mix_Volume(rotateSound, volume);
    }
    if(pause){
        return;
    }
    if(pauseTimeCounter>0.0f){
        pauseTimeCounter+= deltaTime;
        if(pauseTimeCounter > pauseTime){
            pauseTimeCounter = 0.0f;
            StartToEnd^= 1;
            moveTime = 0.0f;
            //rotateSound = Mix_PlayChannel(Mix_GroupAvailable(1), GetGame()->GetSound("Assets/Sounds/CameraMotor.wav"), 0);
        }
        return;
    }
    if(moveTime == 0.0f){
        rotateSound = Mix_PlayChannel(Mix_GroupAvailable(1), GetGame()->GetSound("Assets/Sounds/CameraMotor.wav"), 0);
    }
    moveTime+= deltaTime;
    float ratio = moveTime/interpTime;
    if(ratio > 1.0f){
        if(pauseTimeCounter == 0.0f){
            if(rotateSound!=-1){
                Mix_HaltChannel(rotateSound);
                rotateSound = -1;
            }
            Mix_Volume(Mix_PlayChannel(Mix_GroupAvailable(1), GetGame()->GetSound("Assets/Sounds/CameraMotorStop.wav"), 0), volume);
        }
        moveTime = 0.0f;
        pauseTimeCounter+= deltaTime;
        if(StartToEnd){
            SetQuaternion(endQ);
        }
        else{
            SetQuaternion(startQ);
        }
    }
    else{
        Quaternion newQ;
        if(StartToEnd){
            newQ = Quaternion::Slerp(startQ, endQ, ratio);
        }
        else{
            newQ = Quaternion::Slerp(endQ, startQ, ratio);
        }
        SetQuaternion(newQ);
    }
}
