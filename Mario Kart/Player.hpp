//
//  Player.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/20/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include "Actor.h"
class Player: public Actor{
public:
    Player(class Game* game);
    class MeshComponent* mc;
    class CollisionComponent* mcollision;
    class PlayerMove* pMove;
};
#endif /* Player_hpp */
