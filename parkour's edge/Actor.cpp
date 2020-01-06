#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game, Actor* parent)
	:mGame(game)
    ,mParent(parent)
	,mState(ActorState::Active)
,mPosition(Vector3::Zero)
	,mScale(1.0f)
	,mRotation(0.0f)
{
    if(parent == nullptr){
        mGame->AddActor(this);
    }
    else{
        parent->AddChildren(this);
    }
	// TODO
}

Actor::~Actor()
{
	// TODO
    if(mParent == nullptr){
        mGame->RemoveActor(this);
    }
    else{
        mParent->RemoveChildren(this);
    }
    while(!mChildren.empty()){
        delete mChildren.back();
    }
    for(int i = 0; i < mComponents.size(); i++){
        delete mComponents[i];
    }
    mComponents.clear();
}

void Actor::Update(float deltaTime)
{
    CalcWorldTransform();
    if(mState == ActorState::Active){
        for(Component* c: mComponents){
            c->Update(deltaTime);
        }
        OnUpdate(deltaTime);
    }
    CalcWorldTransform();
    for(Actor* child: mChildren){
        child->Update(deltaTime);
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
Vector3 Actor::GetFoward(){
    return Vector3(cos(mRotation), sin(mRotation), 0.0f);
}
Vector3 Actor::GetRight(){
    float rot = mRotation + Math::PiOver2;
    return Vector3(cos(rot), sin(rot), 0.0f);
}
void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
	});
}
Vector3 Actor::GetQuatForward(){
    Vector3 foward = Vector3::Transform(Vector3::UnitX, mQuaternion);
    foward.Normalize();
    return foward;
}
void Actor::CalcWorldTransform(){
    Matrix4 scaleMatrix = Matrix4::CreateScale(mScale);
    Matrix4 rotationMatrix = Matrix4::CreateRotationZ(mRotation);
    Matrix4 postionMatrix = Matrix4::CreateTranslation(mPosition);
    Matrix4 rotation = Matrix4::CreateFromQuaternion(mQuaternion);
    mWorldTransform = scaleMatrix*rotationMatrix*rotation*postionMatrix;
    if(mParent!=nullptr){
        if(mInheritScale){
            mWorldTransform*=mParent->mWorldTransform;
        }else{
            mWorldTransform*=mParent->CalcWorldRotTrans();
        }
    }
}
Matrix4 Actor::CalcWorldRotTrans(){
    Matrix4 rotationMatrix = Matrix4::CreateRotationZ(mRotation);
    Matrix4 postionMatrix = Matrix4::CreateTranslation(mPosition);
    Matrix4 rotation = Matrix4::CreateFromQuaternion(mQuaternion);
    Matrix4 Transform = rotationMatrix*rotation*postionMatrix;
    if(mParent!=nullptr){
        Transform*= mParent->CalcWorldRotTrans();
    }
    return Transform;
}
void Actor::AddChildren(Actor *child){
    mChildren.push_back(child);
}
void Actor::RemoveChildren(Actor *child){
    auto iter = std::find(mChildren.begin(), mChildren.end(), child);
    if (iter != mChildren.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        auto iter2 = mChildren.end() - 1;
        std::iter_swap(iter, iter2);
        mChildren.pop_back();
    }
}
