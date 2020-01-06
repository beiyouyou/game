//
//  Boss.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/8/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Boss_hpp
#define Boss_hpp
#define PI 3.14159265
#include <stdio.h>
#include "Actor.h"
class Boss: public Actor{
public:
    Boss(class Game* game);
    class AnimatedSprite* ASprite;
    class CollisionComponent* mCollision;
    class EnemyComponent* EComponent;
    void OnUpdate(float deltaTime) override;
    float attack_period = 2.0f;
};
#endif /* Boss_hpp */
