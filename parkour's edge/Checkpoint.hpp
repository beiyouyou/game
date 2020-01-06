//
//  Checkpoint.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 11/21/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Checkpoint_hpp
#define Checkpoint_hpp

#include <stdio.h>
#include "Actor.h"
#include <string>
class Checkpoint: public Actor{
public:
    Checkpoint(class Game* game, Actor* parent);
    class MeshComponent* mMesh;
    class CollisionComponent* mCollision;
    bool active = false;
    void OnUpdate(float deltaTime);
    std::string mLevelString;
    std::string mText;
    void setLevel(std::string level){mLevelString = level;}
};
#endif /* Checkpoint_hpp */
