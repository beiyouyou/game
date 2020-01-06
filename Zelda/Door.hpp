//
//  Door.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/6/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Door_hpp
#define Door_hpp

#include <stdio.h>
#include "Actor.h"
#include <string>
enum class Direction
{
    Up,
    Down,
    Left,
    Right
};
enum class State
{
    Open,
    Closed,
    Locked
};
class Door:public Actor{
public:
    Door(class Game* game);
    class SpriteComponent* mSprite;
    class CollisionComponent* mCollision;
    std::string destination;
    std::string state;
    std::string direction;
    void Setup(std::string destination, std::string state, std::string direction);
    void UpdateTexture();
};
#endif /* Door_hpp */
