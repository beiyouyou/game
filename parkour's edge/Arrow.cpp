//
//  Arrow.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 11/21/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Arrow.hpp"
#include "MeshComponent.h"
#include "Game.h"
#include "Checkpoint.hpp"
#include "Renderer.h"
#include "Player.hpp"
Arrow::Arrow(Game* game, Actor* parent):Actor(game, parent){
    mMesh = new MeshComponent(this);
    mMesh->SetMesh(game->GetRenderer()->GetMesh("Assets/Arrow.gpmesh"));
    SetScale(0.15f);
}
void Arrow::OnUpdate(float deltaTime){
    Checkpoint* cp = GetGame()->mChecks.size()==0? nullptr: GetGame()->mChecks.front();
    if(cp == nullptr){
        SetQuaternion(Quaternion::Identity);
    }
    else{
        Vector3 foward = cp->GetPosition() - GetGame()->player->GetPosition();
        foward.Normalize();
        float dotResult = Vector3::Dot(foward, Vector3::UnitX);
        Matrix4 rotationMatrix;
        if(dotResult == 1.0f){
            SetQuaternion(Quaternion::Identity);
        }
        else if(dotResult == -1.0f){
            Quaternion *q =new Quaternion(Vector3::UnitZ, Math::Pi);
            SetQuaternion(*q);
        }
        else{
            float angle = Math::Acos(dotResult);
            Vector3 crossProduct = Vector3::Cross(Vector3::UnitX, foward);
            Vector3 axis = crossProduct*(1/crossProduct.Length());
            axis.Normalize();
            Quaternion *q =new Quaternion(axis, angle);
            SetQuaternion(*q);
        }
    }
    SetPosition(GetGame()->GetRenderer()->Unproject(Vector3(0.0f, 250.0f, 0.1f)));
}
