//
//  Block.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 9/12/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Block_hpp
#define Block_hpp
#include "Actor.h"
#include <stdio.h>
#include "SDL/SDL.h"
class Block: public Actor
{
public:
    class Game* game;
    Block(class Game* game);
    ~Block();
    class SpriteComponent* sComponent;
    class CollisionComponent* mCollision;
    void ChangeTexture(SDL_Texture* texture);
};
#endif /* Block_hpp */
