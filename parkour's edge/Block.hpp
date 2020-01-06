//
//  Block.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/31/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Block_hpp
#define Block_hpp

#include <stdio.h>
#include "Actor.h"
class Block: public Actor{
public:
    Block(class Game* game, Actor* parent);
    ~Block();
    bool rotate = false;
    void SetRotate(bool rot){rotate = rot;}
    bool GetRotate(){return rotate;}
    class MeshComponent* mc;
    class CollisionComponent* mCollision;
    void OnUpdate(float deltaTime);
    bool Mirror = false;
    bool GetMirror(){return Mirror;}
    void SetMirror(bool isMirror){Mirror = isMirror;}
};
#endif /* Block_hpp */
