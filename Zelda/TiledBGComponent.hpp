//
//  TiledBGComponent.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 9/27/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef TiledBGComponent_hpp
#define TiledBGComponent_hpp

#include <stdio.h>
#include "SpriteComponent.h"
#include <vector>
class TiledBGComponent : public SpriteComponent
{
public:
    TiledBGComponent(class Actor* owner);
    std::vector<std::vector<std::string>> CSVVector;
    int tileWidth, tileHeight;
    void Draw(SDL_Renderer* renderer, Vector2 Camera);
    void LoadTileCSV(const std::string& fileName, int tileWidth, int tileHeight);
};
#endif /* TiledBGComponent_hpp */
