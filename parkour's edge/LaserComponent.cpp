//
//  LaserComponent.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 11/17/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "LaserComponent.hpp"
#include "LaserMine.hpp"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Player.hpp"
#include "Renderer.h"
#include "Texture.h"
#include "Block.hpp"
#include "SegmentCast.h"
#include "VertexArray.h"
#include <iostream>
#include <vector>
LaserComponent::LaserComponent(LaserMine* mowner):MeshComponent(mowner, true){
    SetMesh(mowner->GetGame()->GetRenderer()->GetMesh("Assets/Laser.gpmesh"));
}
void LaserComponent::Draw(class Shader* shader){
    if (mMesh)
    {
        for(LineSegment l: mLines){
            // Set the world transform
            shader->SetMatrixUniform("uWorldTransform",
                                     createTransform(l));
            // Set the active texture
            Texture* t = mMesh->GetTexture(mTextureIndex);
            if (t)
            {
                t->SetActive();
            }
            // Set the mesh's vertex array as active
            VertexArray* va = mMesh->GetVertexArray();
            va->SetActive();
            // Draw
            glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
        }
    }
}
void LaserComponent::Update(float deltaTime){
    mLines.clear();
    LineSegment mLine;
    mLine.mStart = mOwner->GetWorldPosition();
    mLine.mEnd = mOwner->GetWorldPosition() + 500.0f*mOwner->GetWorldForward();
    Vector3 forward = mOwner->GetWorldForward();
    Block* lastBlock = nullptr;
    CastInfo info;
    bool firstparent = true;
    while(true){
        if(SegmentCast(mOwner->GetGame()->player, mLine, info)){
            mLine.mEnd = info.mPoint;
            mLines.push_back(mLine);
            mOwner->GetGame()->player->RespwanPlayer();
            Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Laser.wav"), 0);
            return;
        }
        bool blockCollision = false;
        if(firstparent){
            blockCollision = mOwner->mParent==nullptr?SegmentCast(mOwner->GetGame()->mActors, mLine, info): SegmentCast(mOwner->GetGame()->mActors, mLine, info, mOwner->mParent);
            firstparent = false;
        }
        else{
            blockCollision = lastBlock==nullptr?SegmentCast(mOwner->GetGame()->mActors, mLine, info): SegmentCast(mOwner->GetGame()->mActors, mLine, info, lastBlock);
        }
        if(blockCollision){
            mLine.mEnd = info.mPoint;
            mLines.push_back(mLine);
            Block* b = (Block*)info.mActor;
            if(b->GetMirror() == true){
                mLine.mStart = info.mPoint;
                forward = Vector3::Reflect(forward, info.mNormal);
                mLine.mEnd = mLine.mStart + 500.0f*forward;
                    lastBlock = b;
            }
            else{
                break;
            }
        }
        else{
            mLines.push_back(mLine);
            return;
        }
    }
}
Matrix4 LaserComponent::createTransform(LineSegment mLine){
    Matrix4 scaleMatrix = Matrix4::CreateScale(mLine.Length(), 1.0f, 1.0f);
    Vector3 foward = mLine.mEnd - mLine.mStart;
    foward.Normalize();
    float dotResult = Vector3::Dot(foward, Vector3::UnitX);
    Matrix4 rotationMatrix;
    if(dotResult == 1.0f){
        rotationMatrix = Matrix4::CreateFromQuaternion(Quaternion::Identity);
    }
    else if(dotResult == -1.0f){
        Quaternion q =Quaternion(Vector3::UnitZ, Math::Pi);
        rotationMatrix = Matrix4::CreateFromQuaternion(q);
    }
    else{
        float angle = Math::Acos(dotResult);
        Vector3 crossProduct = Vector3::Cross(Vector3::UnitX, foward);
        Vector3 axis = crossProduct*(1/crossProduct.Length());
        axis.Normalize();
        Quaternion q =Quaternion(axis, angle);
        rotationMatrix = Matrix4::CreateFromQuaternion(q);
    }
    Vector3 midpoint = mLine.PointOnSegment(0.5f);
    Matrix4 postionMatrix = Matrix4::CreateTranslation(midpoint);
    Matrix4 Transform = scaleMatrix*rotationMatrix*postionMatrix;
    return Transform;
}
