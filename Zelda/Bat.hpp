//
//  Bat.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/7/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Bat_hpp
#define Bat_hpp

#include <stdio.h>
#include "Actor.h"
class Bat: public Actor{
public:
    Bat(class Game* game);
    class AnimatedSprite* ASprite;
    class CollisionComponent* mCollision;
    class EnemyComponent* EComponent;
    void OnUpdate(float deltaTime) override;
    float speed = 50.0f;
    float time_elapse = 0.0f;
    Vector2 direction;
};

#endif /* Bat_hpp */
