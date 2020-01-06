#include "AnimatedSprite.h"
#include "Actor.h"
#include "Game.h"
#include <iostream>

AnimatedSprite::AnimatedSprite(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
{
}

void AnimatedSprite::Update(float deltaTime)
{
    if(mAnimName.empty())return;
    if(!mIsPaused){
        mAnimTimer += deltaTime*mAnimFPS;
        while(mAnimTimer >= mAnims.at(mAnimName).size()){
            mAnimTimer -= mAnims.at(mAnimName).size();
        }
    }
    SetTexture(mAnims.at(mAnimName).at(floor(mAnimTimer)));
}
void AnimatedSprite::SetAnimation(const std::string& name, bool resetTimer)
{
	if (mAnimName != name)
	{
		mAnimName = name;
	}
	
	if (resetTimer)
	{
		mAnimTimer = 0.0f;
	}
}

void AnimatedSprite::AddAnimation(const std::string& name,
								  const std::vector<SDL_Texture*>& images)
{
	mAnims.emplace(name, images);
}
