//
//  SecretBlock.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/6/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef SecretBlock_hpp
#define SecretBlock_hpp

#include <stdio.h>
#include "Actor.h"
#include <string>
class SecretBlock: public Actor{
public:
    SecretBlock(class Game*game);
    class SpriteComponent* mSprite;
    class CollisionComponent* mCollision;
    std::string Movedirection;
    float resistance = 50.0f;
    float destination;
    bool movable = true;
    bool open_door = false;
    void SetDirection(std::string direction);
};
#endif /* SecretBlock_hpp */
