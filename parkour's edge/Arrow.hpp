//
//  Arrow.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 11/21/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Arrow_hpp
#define Arrow_hpp

#include <stdio.h>
#include "Actor.h"
class Arrow: public Actor{
public:
    Arrow(class Game* game, Actor* parent);
    class MeshComponent* mMesh;
    void OnUpdate(float deltaTime);
};
#endif /* Arrow_hpp */
