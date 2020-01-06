//
//  GoombaMove.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 9/19/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef GoombaMove_hpp
#define GoombaMove_hpp

#include <stdio.h>
#include "MoveComponent.h"
#include "Math.h"
#include <stdio.h>
class GoombaMove: public MoveComponent
{
public:
    bool inAir = true;
    float seconds;
    class Goomba* goomba;
    GoombaMove(class Goomba* owner);
    float mYSpeed;
    void Update(float deltaTime);
};
#endif /* GoombaMove_hpp */
