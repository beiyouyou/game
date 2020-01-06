#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner)
:Component(owner, 50)
,mAngularSpeed(0.0f)
,mForwardSpeed(0.0f)
,mStrafeSpeed(0.0f)
{
	
}

void MoveComponent::Update(float deltaTime)
{
    mOwner->SetRotation(mOwner->GetRotation() + mAngularSpeed*deltaTime);
    Vector3 velocity = mOwner->GetFoward()*mForwardSpeed*deltaTime;
    mOwner->SetPosition(mOwner->GetPosition() + velocity);
    mOwner->SetPosition(mOwner->GetPosition() + mOwner->GetRight()*GetStrafeSpeed()*deltaTime);
}
