//
//  Coin.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 11/27/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Coin_hpp
#define Coin_hpp

#include <stdio.h>
#include "Actor.h"
class Coin: public Actor{
public:
    Coin(class Game* game, Actor* parent);
    class MeshComponent* mc;
    class CollisionComponent* mCollision;
    void OnUpdate(float deltaTime);
};
#endif /* Coin_hpp */
