//
//  HUD.hpp
//  Game-mac
//
//  Created by Beiyou Zhu on 12/4/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef HUD_hpp
#define HUD_hpp

#include <stdio.h>
#include "UIComponent.h"
#include "Game.h"
#include "Font.h"
class HUD: public UIComponent{
public:
    HUD(class Actor* mowner);
    ~HUD();
    void Draw(class Shader* shader);
    void Update(float deltaTime);
    Font* mFont;
    Texture* mTimerText;
    Texture* mCoin;
    Texture* Text;
    Texture* RadarBackground;
    Texture* RadarArrow;
    Texture* Blip;
    Vector2 RadarCenter;
    bool hasText = false;
    float timer = 0.0f;
    void addCoin();
    void updateCheckPoint(std::string text);
};
#endif /* HUD_hpp */
