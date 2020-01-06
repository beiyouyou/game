//
//  BallMove.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 9/12/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Math.h"
#include "MoveComponent.h"
#include <stdio.h>
class PlayerMove: public MoveComponent
{
public:
    class Player* player;
    PlayerMove(class Player* owner);
    Vector2 Velocity;
    int direction;
    float mYSpeed;
    bool mSpacePressed, mInAir;
    void Update(float deltaTime);
    void ProcessInput(const Uint8* keyState);
    void SetAnimation();
};
