//
//  SecurityCamera.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 12/3/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef SecurityCamera_hpp
#define SecurityCamera_hpp

#include <stdio.h>
#include "Actor.h"
class SecurityCamera: public Actor{
public:
    SecurityCamera(class Game* game, Actor* parent);
    ~SecurityCamera();
    class MeshComponent* mc;
    class SecurityCone* sc;
    void OnUpdate(float deltaTime);
    Quaternion startQ;
    Quaternion endQ;
    float interpTime;
    float pauseTime;
    float moveTime = 0.0f;
    float pauseTimeCounter = 0.0f;
    bool StartToEnd = true;
    bool pause = false;
    int rotateSound;
    int volume;
    int stopSound;
};
#endif /* SecurityCamera_hpp */
