#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
	:mGame(game)
	,mState(ActorState::Active)
	,mPosition(Vector2::Zero)
	,mScale(1.0f)
	,mRotation(0.0f)
{
    mGame->AddActor(this);
	// TODO
}

Actor::~Actor()
{
	// TODO
    mGame->RemoveActor(this);
    for(int i = 0; i < mComponents.size(); i++){
        delete mComponents[i];
    }
    mComponents.clear();
}

void Actor::Update(float deltaTime)
{
    if(mState == ActorState::Active){
        for(Component* c: mComponents){
            c->Update(deltaTime);
        }
        OnUpdate(deltaTime);
    }
	// TODO
}

void Actor::OnUpdate(float deltaTime)
{
    
}

void Actor::ProcessInput(const Uint8* keyState)
{
    if(mState == ActorState::Active){
        for(Component* c: mComponents){
            c->ProcessInput(keyState);
        }
        OnProcessInput(keyState);
    }
	// TODO
}

void Actor::OnProcessInput(const Uint8* keyState)
{
}
Vector2 Actor::GetFoward(){
    return Vector2(cos(mRotation), -1*sin(mRotation));
}
void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
	});
}
