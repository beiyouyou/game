//
//  SecurityCone.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 12/3/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "SecurityCone.hpp"
#include "MeshComponent.h"
#include "Game.h"
#include "Player.hpp"
#include "Renderer.h"
#include "SecurityCamera.hpp"
#include <iostream>
SecurityCone::SecurityCone(Game* game, Actor* parent):Actor(game, parent){
    mc = new MeshComponent(this, true);
    mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Cone.gpmesh"));
    SetPosition(Vector3(30,0,0));
    SetScale(3.0f);
}

void SecurityCone::OnUpdate(float deltaTime){
    SecurityCamera* parent = (SecurityCamera*)mParent;
    if(kickOut){
        kickOutTime+= deltaTime;
        if(kickOutTime > 0.5f){
            GetGame()->player->RespwanPlayer();
            kickOutTime = 0.0f;
            mc->SetTextureIndex(0);
            kickOut = false;
            parent->pause = false;
        }
        return;
    }
    Vector3 start = GetWorldPosition();
    Vector3 dir = GetWorldForward();
    Vector3 playerPos = GetGame()->player->GetPosition();
    Vector3 camera_to_player = playerPos-start;
    float length = Vector3::Dot(camera_to_player, dir);
    camera_to_player.Normalize();
    float angle = Math::Acos(Vector3::Dot(dir, camera_to_player));
    //std::cout<< "angle is " << angle*180/Math::Pi<< "length is " <<length<<"\n";
    if(angle*180/Math::Pi < 30.0 && length < 300.0f){
        //first pause
        if(parent->pause == false){
            if(parent->rotateSound!=-1){
                Mix_HaltChannel(parent->rotateSound);
            }
            parent->rotateSound = -1;
            Mix_Volume(Mix_PlayChannel(Mix_GroupAvailable(1), GetGame()->GetSound("Assets/Sounds/CameraMotorStop.wav"), 0), parent->volume);
            securityDetect = Mix_PlayChannel(Mix_GroupAvailable(1), GetGame()->GetSound("Assets/Sounds/SecurityDetected.wav"), 0);
        }
        parent->pause = true;
        mc->SetTextureIndex(1);
        warningSecond+= deltaTime;
    }
    else{
        parent->pause = false;
        mc->SetTextureIndex(0);
        warningSecond = 0.0f;
        if(securityDetect!= -1){
            Mix_HaltChannel(securityDetect);
            securityDetect = -1;
        }
    }
    if(warningSecond > 2.0f){
        kickOut = true;
        mc->SetTextureIndex(2);
        warningSecond = 0.0f;
        kickOutTime+=deltaTime;
    }
}
