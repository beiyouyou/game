//
//  Heart.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/8/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Heart_hpp
#define Heart_hpp

#include <stdio.h>
#include "Actor.h"
class Heart: public Actor{
public:
    Heart(class Game* game);
    class SpriteComponent* mSprite;
    bool Full = true;
};
#endif /* Heart_hpp */
