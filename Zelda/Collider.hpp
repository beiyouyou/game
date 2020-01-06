//
//  Collider.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 9/29/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Collider_hpp
#define Collider_hpp
#include "Actor.h"
class Collider: public Actor
{
public:
    Collider(class Game* game);
    class CollisionComponent* mCollision;
    void setWidthHeight(float width, float height);
};
#include <stdio.h>

#endif /* Collider_hpp */
