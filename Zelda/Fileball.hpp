//
//  Fileball.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/7/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Fileball_hpp
#define Fileball_hpp

#include <stdio.h>
#include "Actor.h"
class Fireball: public Actor{
public:
    Fireball(class Game* game);
    class SpriteComponent* mSprite;
    class CollisionComponent* mCollision;
    class MoveComponent* MComponent;
    void OnUpdate(float deltaTime) override;
    float time_elaspe = 0;
};
#endif /* Fileball_hpp */
