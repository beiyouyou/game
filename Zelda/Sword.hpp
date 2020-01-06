//
//  Sword.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/7/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Sword_hpp
#define Sword_hpp
#include "Actor.h"
#include <stdio.h>
class Sword:public Actor{
public:
    Sword(class Game* game);
    class SpriteComponent* mSprite;
    class CollisionComponent* mCollision;
};
#endif /* Sword_hpp */
