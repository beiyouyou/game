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
#include <vector>
class Player: public Actor{
public:
    Player(class Game* game, Actor* parent);
    class PlayerMove* pMove;
    class CameraComponent* mCamera;
    class CollisionComponent* mCollision;
    Vector3 Respawn;
    void SetRespawnPos(Vector3 pos){Respawn = pos;}
    void RespwanPlayer();
    class Arrow* arrow;
    class HUD* mText;
};
#endif /* Player_hpp */
