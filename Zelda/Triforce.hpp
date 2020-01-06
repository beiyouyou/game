//
//  Triforce.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/8/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Triforce_hpp
#define Triforce_hpp

#include <stdio.h>
#include "Actor.h"
class Triforce: public Actor{
public:
    Triforce(class Game* game);
    class AnimatedSprite* ASprite;
    class CollisionComponent* mCollision;
    class Collectible* collectible;
};
#endif /* Triforce_hpp */
