//
//  Block.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/21/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Block_hpp
#define Block_hpp

#include <stdio.h>
#include "Actor.h"
class Block: public Actor{
public:
    Block(class Game* game);
    ~Block();
    class MeshComponent* mc;
    class CollisionComponent* mcollision;
    void OnUpdate(float deltaTime)override;
    bool explode = false;
    void SetExplode(bool explode);
};
#endif /* Block_hpp */
