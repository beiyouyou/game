#include "CollisionComponent.h"
#include "Actor.h"

CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f)
{
	
}

CollisionComponent::~CollisionComponent()
{
	
}

bool CollisionComponent::Intersect(const CollisionComponent* other)
{
	return !((GetMax().x < other->GetMin().x)||(GetMin().x > other->GetMax().x)
             ||(GetMax().y < other->GetMin().y)||(GetMin().y >other->GetMax().y));
}

Vector2 CollisionComponent::GetMin() const
{
	// TODO: Implement
    float minx = mOwner->GetPosition().x - (mWidth*mOwner->GetScale())/2.0f;
    float miny =mOwner->GetPosition().y - (mHeight*mOwner->GetScale())/2.0f;
	return Vector2(minx, miny);
}

Vector2 CollisionComponent::GetMax() const
{
    float maxx = mOwner->GetPosition().x + (mWidth*mOwner->GetScale())/2.0f;
    float maxy =mOwner->GetPosition().y + (mHeight*mOwner->GetScale())/2.0f;
    return Vector2(maxx, maxy);
}

const Vector2& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(
	const CollisionComponent* other, Vector2& offset)
{
    if(!Intersect(other)){
        return CollSide::None;
    }
    float dy1 = other->GetMin().y - GetMax().y;
    float dy2 = other->GetMax().y - GetMin().y;
    float dx1 = other->GetMin().x - GetMax().x;
    float dx2 = other->GetMax().x - GetMin().x;
    float minY = abs(dy1) > abs(dy2)? dy2: dy1;
    float minX = abs(dx1) > abs(dx2)? dx2: dx1;
    float minMove = abs(minY) > abs(minX)? minX: minY;
    if(minMove == dy1){
        offset += Vector2(0, minMove);
        return CollSide::Top;
    }
    else if(minMove == dy2){
        offset += Vector2(0, minMove);
        return CollSide::Bottom;
    }
    else if(minMove == dx1){
        offset += Vector2(minMove, 0);
        return CollSide::Left;
    }
    else if(minMove == dx2){
        offset += Vector2(minMove, 0);
        return CollSide::Right;
    }
    return CollSide::None;
}
