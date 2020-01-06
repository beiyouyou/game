//
//  Spawner.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 9/19/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Spawner_hpp
#define Spawner_hpp

#include <stdio.h>
#include "Actor.h"
#include <string>
class Spawner: public Actor{
public:
    Spawner(class Game* game);
    Game* game;
    std::string type;
    void Trigger();
};
#endif /* Spawner_hpp */
