//
//  CameraComponent.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/24/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef CameraComponent_hpp
#define CameraComponent_hpp

#include <stdio.h>
#include "Component.h"
#include "Math.h"
enum CameraRotation{
    Left, Right, Front, Back, NullRotation
};
class CameraComponent: public Component{
public:
    CameraComponent(class Player* owner);
    void Update(float deltaTime)override;
    float GetPitchSpeed() const { return mPitchSpeed; }
    void SetPitchSpeed(float speed) { mPitchSpeed= speed; }
    void SetRotation(CameraRotation rside);
    class Player* player;
    float HDistance = 60.0f;
    float TargetOffset = 50.0f;
    float mPitchAngle;
    float mPitchSpeed;
    float mRollAngle;
    CameraRotation rotationSide;
    CameraRotation preRotation;
    Vector3 Velocity;
    Vector3 position;
};
#endif /* CameraComponent_hpp */
