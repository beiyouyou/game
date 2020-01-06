#include "CollisionComponent.h"
#include "Actor.h"
#include <iostream>

CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f)
,mDepth(0.0f)
{
	
}

CollisionComponent::~CollisionComponent()
{
	
}

bool CollisionComponent::Intersect(const CollisionComponent* other)
{
	Vector3 aMin = GetMin();
	Vector3 aMax = GetMax();
	Vector3 bMin = other->GetMin();
	Vector3 bMax = other->GetMax();

	bool no = aMax.x < bMin.x ||
		aMax.y < bMin.y ||
		aMax.z < bMin.z ||
		bMax.x < aMin.x ||
		bMax.y < aMin.y ||
		bMax.z < aMin.z;

	return !no;
}

Vector3 CollisionComponent::GetMin() const
{
	Vector3 v = mOwner->GetPosition();
	v.x -= mDepth * mOwner->GetScale() / 2.0f;
	v.y -= mWidth * mOwner->GetScale() / 2.0f;
	v.z -= mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

Vector3 CollisionComponent::GetMax() const
{
	Vector3 v = mOwner->GetPosition();
	v.x += mDepth * mOwner->GetScale() / 2.0f;
	v.y += mWidth * mOwner->GetScale() / 2.0f;
	v.z += mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

const Vector3& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(
	const CollisionComponent* other, Vector3& offset)
{
	//offset = Vector3::Zero;
	// TODO: Implement
    if(!Intersect(other)){
        return CollSide::None;
    }
    float dy1 = other->GetMin().y - GetMax().y;
    float dy2 = other->GetMax().y - GetMin().y;
    float dx1 = other->GetMin().x - GetMax().x;
    float dx2 = other->GetMax().x - GetMin().x;
    float dz1 = other->GetMin().z - GetMax().z;
    float dz2 = other->GetMax().z - GetMin().z;
    float minY = abs(dy1) > abs(dy2)? dy2: dy1;
    float minX = abs(dx1) > abs(dx2)? dx2: dx1;
    float minZ = abs(dz1) > abs(dz2)? dz2: dz1;
    float minMove = abs(minY) > abs(minX)? minX: minY;
    minMove = abs(minZ) > abs(minMove)? minMove: minZ;
    if(minMove == dz1){
        offset += Vector3(0, 0, minMove);
        return CollSide::Bottom;
    }
    else if(minMove == dz2 ){
        offset += Vector3(0, 0, minMove);
        //std::cout<<"collision top side\n";
        return CollSide::Top;
    }
    else if(minMove == minY){
        //std::cout<<"collision y side\n";
        offset+= Vector3(0,minMove,0);
        if(minMove == dy1){
            return CollSide::SideY1;
        }
        else{
            return CollSide::SideY2;
        }
    }
    else{
        //std::cout<<"collision x side\n";
        offset+= Vector3(minMove, 0,0);
        if(minMove == dx1){
            return CollSide::SideX1;
        }
        else{
            return CollSide::SideX2;
        }
    }
}
