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
#include "Math.h"
class PlayerMove: public MoveComponent{
public:
    PlayerMove(class Player* owner);
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    Vector3 velocity;
    Vector3 direction;
    class Player* player;
    size_t block_Create = 0;
    size_t block_destroy = 500;
    size_t game_block_Create = 0;
    float second = 0.0f;
    float multiplier = 1.0f;
    float base_Speed = 400.0f;
    bool leading_edge = false;
};
#endif /* PlayerMove_hpp */
