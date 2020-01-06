//
//  Ball.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 9/12/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Actor.h"
#include <stdio.h>
class Player: public Actor{
public:
    Player(class Game* game);
    class AnimatedSprite* ASprite;
    class CollisionComponent* mCollision;
private:
    class PlayerMove* pMove;
    // Any actor-specific update code (overridable)
};
