//
//  Goomba.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 9/19/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Goomba_hpp
#define Goomba_hpp
#include "Actor.h"
#include <stdio.h>
#include "SDL/SDL.h"
class Goomba: public Actor
{
public:
    class Game* game;
    Goomba(class Game* game);
    ~Goomba();
    class AnimatedSprite* AComponent;
    class CollisionComponent* mCollision;
    class GoombaMove* GMove;
    bool Stomped;
};
#endif /* Goomba_hpp */
