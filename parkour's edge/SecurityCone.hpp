//
//  SecurityCone.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 12/3/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef SecurityCone_hpp
#define SecurityCone_hpp

#include <stdio.h>
#include "Actor.h"
class SecurityCone: public Actor{
public:
    SecurityCone(class Game* game, Actor* parent);
    class MeshComponent* mc;
    void OnUpdate(float deltaTime);
    float warningSecond = 0.0f;
    bool kickOut = false;
    float kickOutTime = 0.0f;
    int securityDetect = -1;
};
#endif /* SecurityCone_hpp */
