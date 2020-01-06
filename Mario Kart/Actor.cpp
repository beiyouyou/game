#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
	:mGame(game)
	,mState(ActorState::Active)
,mPosition(Vector3::Zero)
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
    Matrix4 scaleMatrix = Matrix4::CreateScale(mScale);
    Matrix4 rotationMatrix = Matrix4::CreateRotationZ(mRotation);
    Matrix4 postionMatrix = Matrix4::CreateTranslation(mPosition);
    mWorldTransform = scaleMatrix*rotationMatrix*postionMatrix;
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
Vector3 Actor::GetFoward(){
    return Vector3(cos(mRotation), sin(mRotation), 0.0f);
}
void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
	});
}
