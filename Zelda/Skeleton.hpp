//
//  Skeleton.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/7/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Skeleton_hpp
#define Skeleton_hpp

#include <stdio.h>
#include "Actor.h"
class Skeleton:public Actor{
public:
    Skeleton(class Game* game);
    class AnimatedSprite* ASprite;
    class CollisionComponent* mCollision;
    class EnemyComponent* EComponent;
    void OnUpdate(float deltaTime) override;
    float time_elaspe = 4.0f;
    float cool_down = 0;
};
#endif /* Skeleton_hpp */
