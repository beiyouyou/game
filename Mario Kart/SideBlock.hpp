//
//  SideBlock.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/21/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef SideBlock_hpp
#define SideBlock_hpp

#include <stdio.h>
#include "Actor.h"
class SideBlock: public Actor{
public:
    SideBlock(class Game* game);
    class MeshComponent* mc;
    void OnUpdate(float deltaTime)override;
};
#endif /* SideBlock_hpp */
