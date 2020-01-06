//
//  Collectible.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/6/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Collectible_hpp
#define Collectible_hpp

#include <stdio.h>
#include <functional>
#include "Component.h"
class Collectible: public Component{
public:
    class Actor* owner;
    Collectible(class Actor* actor);
    std::function<void()> mOnCollect;
    void Update(float deltaTime) override;
    void setOnCollect(std::function<void()> onCollect);
};

#endif /* Collectible_hpp */
