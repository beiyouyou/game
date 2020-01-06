//
//  Bullet.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/21/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Bullet_hpp
#define Bullet_hpp

#include <stdio.h>
#include "Actor.h"
class Bullet: public Actor{
public:
    Bullet(class Game* game);
    class MeshComponent* mc;
    class MoveComponent* mMove;
    class CollisionComponent* mCollision;
    void OnUpdate(float deltaTime)override;
    float time_elaspe = 0.0f;
    void explode(Vector3 b);
};
#endif /* Bullet_hpp */
