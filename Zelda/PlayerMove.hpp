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
enum class direction
{
    Up,
    Down,
    Left,
    Right
};
class PlayerMove: public MoveComponent
{
public:
    direction dir;
    class Player* player;
    class Sword* sword;
    PlayerMove(class Player* owner);
    Vector2 Velocity;
    int direction;
    float mYSpeed;
    bool mSpacePressed, move;
    void Update(float deltaTime);
    void ProcessInput(const Uint8* keyState);
    void SetAnimation();
    void OpenDoorHelper(class Door* door);
    void UpdateSword();
    float attack_timer;
};
