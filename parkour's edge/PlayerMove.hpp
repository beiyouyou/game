//
//  PlayerMove.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/20/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef PlayerMove_hpp
#define PlayerMove_hpp

#include <stdio.h>
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "Math.h"
enum MoveState{
  OnGround, Jump, Falling, WallClimb, WallRun
};
class PlayerMove: public MoveComponent{
public:
    PlayerMove(class Player* owner);
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    void ChangeState(MoveState mState);
    MoveState mCurrentState;
    Vector3 mVelocity, mAcceleration, mPendingForces;
    float mMass = 1.0f;
    Vector3 mGravity = Vector3(0.0f, 0.0f, -980.0f);
    Vector3 mJumpForce = Vector3(0.0f, 0.0f, 35000.0f);
    Vector3 WallClimbForce = Vector3(0.0f, 0.0f, 1800.0f);
    Vector3 mWallRunForce = Vector3(0.0f, 0.0f, 1200.0f);
    float mWallRunTimer = 0.0f;
    float mWallClimbTimer = 0.0f;
    bool SpacePressed = false;
    void PhysicsUpdate(float deltaTime);
    void AddForce(const Vector3& force){mPendingForces+= force;};
    void FixXYVelocity();
    bool CanWallClimb(CollSide cSide);
    bool CanWallRun(CollSide cSide);
    int mRunningSFX;
protected:
    void UpdateOnGround(float deltaTime);
    void UpdateJump(float deltaTime);
    void UpdateFalling(float deltaTime);
    void UpdateWallClimb(float deltaTime);
    void UpdateWallRun(float deltaTime);
    CollSide FixCollision(class CollisionComponent* self, class CollisionComponent* block);
};
#endif /* PlayerMove_hpp */
