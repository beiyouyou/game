//
//  HUD.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 12/4/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "HUD.hpp"
#include "Actor.h"
#include "Shader.h"
#include "Texture.h"
#include "Player.hpp"
#include "Game.h"
#include "Renderer.h"
#include <string>
#include <iostream>
#include <sstream>
#include "SecurityCamera.hpp"
#include <iomanip>
HUD::HUD(Actor* mowner):UIComponent(mowner){
    mFont = new Font();
    mFont->Load("Assets/Inconsolata-Regular.ttf");
    mTimerText = mFont->RenderText("00:00.00");
    addCoin();
    Text = mFont->RenderText("");
    RadarBackground = mowner->GetGame()->GetRenderer()->GetTexture("Assets/Radar.png");
    RadarArrow = mowner->GetGame()->GetRenderer()->GetTexture("Assets/RadarArrow.png");
    Blip= mowner->GetGame()->GetRenderer()->GetTexture("Assets/Blip.png");
    RadarCenter = Vector2(400.0f, -280.0f);
}
HUD::~HUD(){
    mFont->Unload();
    //?
    delete mFont;
}
void HUD::Draw(Shader *shader){
    DrawTexture(shader, mTimerText, Vector2(-420.0f, -325.0f));
    DrawTexture(shader, mCoin, Vector2(-443.0f, -300.0f));
    if(mOwner->GetGame()->hasText){
        if(Text == nullptr){
            Text = mFont->RenderText(mOwner->GetGame()->Screen_text);
        }
        DrawTexture(shader, Text, Vector2::Zero);
    }
    DrawTexture(shader, RadarBackground, RadarCenter);
    DrawTexture(shader, RadarArrow, RadarCenter);
    //calculate relative position of cameras and draw them 1500 to 92 is size;
    Vector2 playerPos = Vector2(mOwner->GetGame()->player->GetPosition().y, mOwner->GetGame()->player->GetPosition().x);
    Vector2 playerFoward = Vector2(mOwner->GetGame()->player->GetFoward().x, mOwner->GetGame()->player->GetFoward().y);
    float angle = Math::Atan2(playerFoward.y, playerFoward.x);
    Matrix3 rotationMatrix = Matrix3::CreateRotation(angle);
    for(SecurityCamera* camera: mOwner->GetGame()->mSecurityCameras){
        Vector2 cameraPos = Vector2(camera->GetPosition().y, camera->GetPosition().x);
        Vector2 relativeVec = cameraPos - playerPos;
        if(relativeVec.Length() > 1500.0f){
            continue;
        }
        //convert to radar vector;
        relativeVec*= 1/1500.0f;
        relativeVec*= 92.0f;
        relativeVec = Vector2::Transform(relativeVec, rotationMatrix);
        Vector2 finalPos = RadarCenter + relativeVec;
        Vector2 CameraFoward = Vector2(camera->GetWorldForward().y, camera->GetWorldForward().x);
        CameraFoward = Vector2::Transform(CameraFoward, rotationMatrix);
        float cameraAngle = Math::Atan2(CameraFoward.y, CameraFoward.x);
        DrawTexture(shader, Blip, finalPos, 1.0f, cameraAngle);
    }
}
void HUD::Update(float deltaTime){
    mOwner->GetGame()->timer+=deltaTime;
    mTimerText->Unload();
    delete mTimerText;
    int minute = mOwner->GetGame()->timer/60;
    float ParseSeconds = mOwner->GetGame()->timer;
    while(ParseSeconds>= 60.0f){
        ParseSeconds -= 60.0f;
    }
    std::stringstream buffer;
    buffer << std::fixed << std::setprecision(2) <<ParseSeconds;
    std::string seconds = buffer.str();
    buffer.str("");
    buffer << std::setw(2) << std::setfill('0') << minute << ":"
    << std::setw(5) << std::setfill('0') << seconds;
    mTimerText = mFont->RenderText(buffer.str());
}
void HUD::addCoin(){
    std::stringstream buffer;
    buffer << std::setw(2) << std::setfill('0') << mOwner->GetGame()->coin <<"/55";
    std::string coinText = buffer.str();
    mCoin = mFont->RenderText(coinText);
}
void HUD::updateCheckPoint(std::string text){
    mOwner->GetGame()->hasText = true;
    mOwner->GetGame()->Text_timer = 0.0f;
    mOwner->GetGame()->Screen_text = text;
    Text = mFont->RenderText(text);
}
