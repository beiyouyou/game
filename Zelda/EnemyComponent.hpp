//
//  EnemyComponent.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 10/7/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef EnemyComponent_hpp
#define EnemyComponent_hpp

#include <stdio.h>
#include "Component.h"
#include <functional>
class EnemyComponent: public Component{
public:
    class Actor* owner;
    EnemyComponent(class Actor* owner);
    ~EnemyComponent();
    int hit_point;
    int damage;
    float invulnerability_timer;
    std::function<void()> mOnDeath;
    std::function<void()> mOnTakeDamage;
    void setOnDeath(std::function<void()> onCollect);
    void setOnTakeDamage(std::function<void()> onCollect);
    void TakeDamage(int damage);
};
#endif /* EnemyComponent_hpp */
