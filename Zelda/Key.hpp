//
//  Key.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/6/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Key_hpp
#define Key_hpp

#include <stdio.h>
#include "Actor.h"
class Key:public Actor{
public:
    Key(class Game* game);
    class Collectible* collectible;
    class SpriteComponent* mSprite;
    class CollisionComponent* mCollision;
};
#endif /* Key_hpp */
