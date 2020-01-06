//
//  TiledBGComponent.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 9/27/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "TiledBGComponent.hpp"
#include "Actor.h"
#include "CSVHelper.h"
#include <fstream>
#include <iostream>
#include <string>
TiledBGComponent::TiledBGComponent(Actor* owner):
SpriteComponent(owner, 50)
{
    tileWidth = 0;
    tileHeight = 0;
}
void TiledBGComponent::Draw(SDL_Renderer* renderer, Vector2 Camera)
{
    int numCol = GetTexWidth()/tileWidth;
    int originx = (int)(mOwner->GetPosition().x- Camera.x);
    int originy = (int)(mOwner->GetPosition().y - Camera.y);
    if (mTexture)
    {
        for(size_t i = 0; i < CSVVector.size(); i++){
            int y = originy + (int)i*tileHeight;
            int x = originx;
            for(size_t j = 0; j < CSVVector[i].size(); j++){
                int tileNum = stoi(CSVVector[i][j]);
                if(tileNum == -1){
                    x+= tileWidth;
                    continue;
                }
                SDL_Rect resource;
                SDL_Rect r;
                r.w = tileWidth;
                r.h = tileHeight;
                r.x = x;
                r.y = y;
                resource.w = tileWidth;
                resource.h = tileHeight;
                resource.x = (tileNum%numCol)*tileWidth;
                resource.y = (tileNum/numCol)*tileHeight;
                //std::cout<< "coresponding tile x and y" << r.x << " " << r.y << "\n";
                // Draw (have to convert angle from radians to degrees, and clockwise to counter)
                SDL_RenderCopyEx(renderer,
                                 mTexture,
                                 &resource,
                                 &r,
                                 0,
                                 nullptr,
                                 SDL_FLIP_NONE);
                x+= tileWidth;
            }
        }
    }
}
void TiledBGComponent::LoadTileCSV(const std::string &fileName, int tileWidth, int tileHeight){
    this->tileHeight = tileHeight;
    this->tileWidth = tileWidth;
    std::ifstream filein(fileName);
    for (std::string line; std::getline(filein, line); )
    {
        if(line == "")continue;
        std::vector<std::string> lineData = CSVHelper::Split(line);
        this->CSVVector.push_back(lineData);
    }
}

