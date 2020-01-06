//
//  LaserMine.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 11/17/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef LaserMine_hpp
#define LaserMine_hpp

#include <stdio.h>
#include "Actor.h"
class LaserMine: public Actor{
public:
    LaserMine(class Game* game, Actor* parent);
    class MeshComponent* mMesh;
    class LaserComponent* mLaser;
};
#endif /* LaserMine_hpp */
