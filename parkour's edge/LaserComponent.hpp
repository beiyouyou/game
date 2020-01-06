//
//  LaserComponent.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 11/17/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef LaserComponent_hpp
#define LaserComponent_hpp

#include <stdio.h>
#include <vector>
#include "MeshComponent.h"
#include "SegmentCast.h"
class LaserComponent: public MeshComponent{
public:
    LaserComponent(class LaserMine* mowner);
    void Draw(class Shader* shader);
    void Update(float deltaTime);
    Matrix4 createTransform(LineSegment mLine);
    std::vector<LineSegment> mLines;
};
#endif /* LaserComponent_hpp */
