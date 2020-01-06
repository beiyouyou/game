#include "GhostAI.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "PathNode.h"
#include "AnimatedSprite.h"
#include <SDL/SDL.h>
#include <unordered_map>
#include <map>
#include "Ghost.h"
#include "Math.h"
#include "PacMan.h"
#include "Random.h"
#include <iostream>

GhostAI::GhostAI(class Actor* owner)
:Component(owner, 50)
{
	mGhost = static_cast<Ghost*>(owner);
}

void GhostAI::Update(float deltaTime)
{
    //std::cout << "prenode is" << mPrevNode->GetPosition().x <<" " <<mPrevNode->GetPosition().y<<"\n";
    //std::cout << "nextnode is" << mNextNode->GetPosition().x <<" " <<mNextNode->GetPosition().y<<"\n";
    //std::cout<< "startnode is"<< mGhost->GetPosition().x <<" " <<mGhost->GetPosition().y<<"\n";
    //std::cout<< "direction is"<< (mNextNode->GetPosition() - mGhost->GetPosition()).x <<" " <<(mNextNode->GetPosition() - mGhost->GetPosition()).y<<"\n";
    //move if the state is scatter
    if(mState == Scatter){
        scatter_second+=deltaTime;
        if(arrive_End){
            if(!CalculatePath(mNextNode, mTargetNode)){
                double minVal = 10000.0f;
                PathNode* shortestNode = nullptr;
                for(PathNode* node: mNextNode->mAdjacent){
                    if(node == mPrevNode)continue;
                    //find the shortest node from current node
                    double val = (node->GetPosition() - mNextNode->GetPosition()).Length();
                    if(val < minVal){
                        minVal = val;
                        shortestNode = node;
                    }
                }
                mPrevNode = mNextNode;
                mNextNode = shortestNode;
            }
            arrive_End = false;
        }
        //std::cout << "nextnode is" << mNextNode->GetPosition().x <<" " <<mNextNode->GetPosition().y<<"\n";
        //std::cout<< "current postion ghost"<< mGhost->GetPosition().x <<" " <<mGhost->GetPosition().y<<"\n";
        Vector2 dir = (mNextNode->GetPosition() - mGhost->GetPosition());
        dir.Normalize();
        if(dir.x == 0){
            if(dir.y == -1){
                mGhost->asc->SetAnimation("up");
            }
            else{
                mGhost->asc->SetAnimation("down");
            }
        }
        else{
            if(dir.x == -1){
                mGhost->asc->SetAnimation("left");
            }
            else{
                mGhost->asc->SetAnimation("right");
            }
        }
        mGhost->SetPosition(mGhost->GetPosition() + dir*scatter_speed*deltaTime);
        if(mGhost->mCollision->Intersect(mNextNode->mCollision)){
            //handle turnal cases
            if(mNextNode == mOwner->GetGame()->mTunnelLeft || mNextNode == mOwner->GetGame()->mTunnelRight){
                mGhost->SetPosition(mPath.back()->GetPosition());
                mPrevNode = mPath.back();
                mPath.erase(mPath.end()-1);
                mNextNode = mPath.back();
            }
            else{
                mGhost->SetPosition(mNextNode->GetPosition());
                if(mPath.size() == 0)arrive_End = true;
                mPrevNode = arrive_End? mPrevNode:mNextNode;
                mNextNode = arrive_End? mNextNode: mPath.back();
                if(mPath.size()>0){
                    mPath.erase(mPath.end()-1);
                }
            }
        }
        if(scatter_second> 5.0f){
            mState = Chase;
            scatter_second = 0.0f;
            mPath.clear();
            GhostPath(false);
        }
    }
    else if(mState == Frightened){
        if(frighten_second <= 5.0f){
            mGhost->asc->SetAnimation("scared0");
        }
        else if(frighten_second >5.0f && frighten_second <= 7.0f){
            mGhost->asc->SetAnimation("scared1");
        }
        else{
            frighten_second = 0.0f;
            mState = Scatter;
            CalculatePath(mNextNode, mTargetNode);
            mPath.push_back(mNextNode);
            mNextNode=mPrevNode;
            mPrevNode = nullptr;
        }
        //change direction.
        Vector2 dir = mNextNode->GetPosition() - mGhost->GetPosition();
        dir.Normalize();
        mGhost->SetPosition(mGhost->GetPosition() + dir*frighten_speed*deltaTime);
        if(mGhost->mCollision->Intersect(mNextNode->mCollision)){
            //handle turnal cases
            if(mNextNode == mOwner->GetGame()->mTunnelLeft){
                mGhost->SetPosition(mOwner->GetGame()->mTunnelRight->GetPosition());
                mNextNode = mOwner->GetGame()->mTunnelRight;
                mPrevNode = mOwner->GetGame()->mTunnelLeft;
            }
            else if (mNextNode == mOwner->GetGame()->mTunnelRight){
                mGhost->SetPosition(mOwner->GetGame()->mTunnelLeft->GetPosition());
                mNextNode = mOwner->GetGame()->mTunnelLeft;
                mPrevNode = mOwner->GetGame()->mTunnelRight;
                
            }
            int index = Random::GetIntRange(0, (int)mNextNode->mAdjacent.size()-1);
            while(mNextNode->mAdjacent.at(index) == mPrevNode && mNextNode->mAdjacent.size()>1){
                index = Random::GetIntRange(0, (int)mNextNode->mAdjacent.size()-1);
            }
            mPrevNode = mNextNode;
            mNextNode = mNextNode->mAdjacent.at(index);
        }
        frighten_second+=deltaTime;
    }
    else if(mState == Dead){
        if(arrive_End){
            Start(mNextNode);
            arrive_End = false;
        }
        Vector2 dir = (mNextNode->GetPosition() - mGhost->GetPosition());
        dir.Normalize();
        if(dir.x == 0){
            if(dir.y == -1){
                mGhost->asc->SetAnimation("deadup");
            }
            else{
                mGhost->asc->SetAnimation("deaddown");
            }
        }
        else{
            if(dir.x == -1){
                mGhost->asc->SetAnimation("deadleft");
            }
            else{
                mGhost->asc->SetAnimation("deadright");
            }
        }
        mGhost->SetPosition(mGhost->GetPosition() + dir*dead_speed*deltaTime);
        if(mGhost->mCollision->Intersect(mNextNode->mCollision)){
            //handle turnal cases
            if(mNextNode == mOwner->GetGame()->mTunnelLeft || mNextNode == mOwner->GetGame()->mTunnelRight){
                mGhost->SetPosition(mPath.back()->GetPosition());
                mPrevNode = mPath.back();
                mPath.erase(mPath.end()-1);
                mNextNode = mPath.back();
            }
            else{
                mGhost->SetPosition(mNextNode->GetPosition());
                if(mPath.size() == 0)arrive_End = true;
                mPrevNode = arrive_End? mPrevNode:mNextNode;
                mNextNode = arrive_End? mNextNode: mPath.back();
                if(mPath.size()>0){
                    mPath.erase(mPath.end()-1);
                }
            }
        }
    }
    else if (mState == Chase){
        chase_second+= deltaTime;
        ChaseAlgorithm(deltaTime);
        if(chase_second>20.0f){
            chase_second = 0.0f;
            mState = Scatter;
            mPath.clear();
        }
    }
	// TODO: Implement
}


void GhostAI::Frighten()
{
    if(mState == Dead)return;
    frighten_second = 0.0f;
    //reverse direction, change state to frightened
    mState = Frightened;
    mPath.clear();
    //change direction
    PathNode* temp = mNextNode;
    mNextNode = mPrevNode;
    mPrevNode = temp;
    if(mNextNode == nullptr){
        Vector2 dir = mGhost->GetPosition() - mPrevNode->GetPosition();
        dir.Normalize();
        Vector2 new_point = mPrevNode->GetPosition() + dir;
        float minimal = 10000.0f;
        PathNode* node = nullptr;
        for(PathNode* n: mPrevNode->mAdjacent){
            if((n->GetPosition() - new_point).Length() < minimal){
                minimal =(n->GetPosition() - new_point).Length();
                node = n;
            }
        }
        mNextNode = node;
    }
}

void GhostAI::Start(PathNode* startNode)
{
    mPath.clear();
    mGhost->SetPosition(startNode->GetPosition());
    mState = Scatter;
    mPrevNode = nullptr;
    mNextNode = nullptr;
    mTargetNode = nullptr;
    CalculatePath(startNode, mGhost->GetScatterNode());
	// TODO: Implement
}

void GhostAI::Die()
{
    mState = Dead;
    while(!CalculatePath(mNextNode, mOwner->GetGame()->mGhostPen)){
        double minVal = 10000.0f;
        PathNode* shortestNode = nullptr;
        for(PathNode* node: mNextNode->mAdjacent){
            if(node == mPrevNode)continue;
            //find the shortest node from current node
            double val = (node->GetPosition() - mNextNode->GetPosition()).Length();
            if(val < minVal){
                minVal = val;
                shortestNode = node;
            }
        }
        mPrevNode = mNextNode;
        mNextNode = shortestNode;
    }
    mPath.push_back(mNextNode);
    mNextNode = mPrevNode;
    mPrevNode = nullptr;
	// TODO: Implement
}

void GhostAI::DebugDrawPath(SDL_Renderer* render)
{
	// Draw a rectangle at the target node
	if (mTargetNode != nullptr)
	{
		const int SIZE = 16;
		SDL_Rect r;
		r.x = static_cast<int>(mTargetNode->GetPosition().x) - SIZE / 2;
		r.y = static_cast<int>(mTargetNode->GetPosition().y) - SIZE / 2;
		r.w = SIZE;
		r.h = SIZE;
		SDL_RenderDrawRect(render, &r);
	}

	// Line from ghost to next node
	if (mNextNode != nullptr)
	{
		SDL_RenderDrawLine(render,
			static_cast<int>(mOwner->GetPosition().x),
			static_cast<int>(mOwner->GetPosition().y),
			static_cast<int>(mNextNode->GetPosition().x),
			static_cast<int>(mNextNode->GetPosition().y));
	}

	// Exit if no path
	if (mPath.empty())
	{
		return;
	}

	// Line from next node to subsequent on path
	SDL_RenderDrawLine(render,
		static_cast<int>(mNextNode->GetPosition().x),
		static_cast<int>(mNextNode->GetPosition().y),
		static_cast<int>(mPath.back()->GetPosition().x),
		static_cast<int>(mPath.back()->GetPosition().y));

	// Lines for rest of path
	for (size_t i = 0; i < mPath.size() - 1; i++)
	{
		SDL_RenderDrawLine(render,
			static_cast<int>(mPath[i]->GetPosition().x),
			static_cast<int>(mPath[i]->GetPosition().y),
			static_cast<int>(mPath[i + 1]->GetPosition().x),
			static_cast<int>(mPath[i + 1]->GetPosition().y));
	}
}
//calculate the path and generate mpath
bool GhostAI::CalculatePath(PathNode *startNode, PathNode *scatterNode){
    if(startNode == scatterNode)return false;
    std::unordered_map<PathNode*, Info> OpenSet;
    std::unordered_map<PathNode*, Info> CloseSet;
    PathNode* currentNode = startNode;
    Info startInfo;
    startInfo.closed = true;
    CloseSet[currentNode] =startInfo;
    bool first = true;
    while(currentNode!= scatterNode){
        for(PathNode* node: currentNode->mAdjacent){
            if(CloseSet.find(node) != CloseSet.end() || (node == mPrevNode && first))continue;
            if(OpenSet.find(node) != OpenSet.end()){
                double new_g = CloseSet[currentNode].g + (node->GetPosition()-currentNode->GetPosition()).Length();
                if(new_g < OpenSet[node].g){
                    OpenSet[node].parent = currentNode;
                    OpenSet[node].g = new_g;
                    OpenSet[node].f = new_g + OpenSet[node].h;
                }
            }
            else{
                Info new_info;
                new_info.parent = currentNode;
                new_info.g = CloseSet[currentNode].g + (node->GetPosition()-currentNode->GetPosition()).Length();
                new_info.h = EuclideanDistance(node, scatterNode);
                new_info.f = new_info.g + new_info.f;
                OpenSet[node] = new_info;
            }
        }
        if(OpenSet.size() == 0){
            break;
        }
        //find the lowest cost openset;
        double lowest = 10000.0;
        std::unordered_map<PathNode*, Info>::iterator open_node_remove;
        for(std::unordered_map<PathNode*, Info>::iterator it = OpenSet.begin(); it!= OpenSet.end(); it++){
            if(it->second.f < lowest){
                lowest = it->second.f;
                open_node_remove = it;
            }
        }
        currentNode = open_node_remove->first;
        Info updated_info = open_node_remove->second;
        updated_info.closed = true;
        CloseSet[open_node_remove->first]= updated_info;
        OpenSet.erase(open_node_remove);
        first = false;
    }
    if(currentNode != scatterNode){
        std::cout<<"strange break here" <<"\n";
        mTargetNode = scatterNode;
        return false;
    }
    else{
        mTargetNode = scatterNode;
        mPrevNode = startNode;
        PathNode* backtrack_node = scatterNode;
        while(backtrack_node!= startNode){
            //std::cout<<"node position from target to start" << backtrack_node->GetPosition().x <<" " <<
            //backtrack_node->GetPosition().x <<"\n";
            mPath.push_back(backtrack_node);
            backtrack_node = CloseSet[backtrack_node].parent;
        }
        mNextNode = mPath.back();
        mPath.erase(mPath.end()-1);
        return true;
    }
}
double GhostAI::EuclideanDistance(PathNode *startNode, PathNode *endNode){
    Vector2 directPath = endNode->GetPosition() - startNode->GetPosition();
    double directDistance = directPath.Length();
    double LeftDistance = (mOwner->GetGame()->mTunnelLeft->GetPosition() - startNode->GetPosition()).Length()
    + (endNode->GetPosition() - mOwner->GetGame()->mTunnelRight->GetPosition()).Length();
    double RightDistance = (mOwner->GetGame()->mTunnelRight->GetPosition() - startNode->GetPosition()).Length()
    + (endNode->GetPosition() - mOwner->GetGame()->mTunnelLeft->GetPosition()).Length();
    //return the minimum distance;
    double smaller_turnal_distance = LeftDistance < RightDistance? LeftDistance: RightDistance;
    return directDistance < smaller_turnal_distance? directDistance: smaller_turnal_distance;
}
void GhostAI::ChaseAlgorithm(float deltatime){
    //calculate the path if there is no  current path
    if(mGhost->mCollision->Intersect(mNextNode->mCollision)){
        mPath.clear();
        GhostPath(true);
    }
    Vector2 dir = (mNextNode->GetPosition() - mGhost->GetPosition());
    dir.Normalize();
    if(dir.x == 0){
        if(dir.y == -1){
            mGhost->asc->SetAnimation("up");
        }
        else{
            mGhost->asc->SetAnimation("down");
        }
    }
    else{
        if(dir.x == -1){
            mGhost->asc->SetAnimation("left");
        }
        else{
            mGhost->asc->SetAnimation("right");
        }
    }
    mGhost->SetPosition(mGhost->GetPosition() + dir*scatter_speed*deltatime);
}
void GhostAI::failsafe(){
    int index = Random::GetIntRange(0, (int)mNextNode->mAdjacent.size() -1);
    while(mNextNode->mAdjacent.at(index) == mPrevNode || mNextNode->mAdjacent.at(index) == mOwner->GetGame()->mGhostPen){
        index = Random::GetIntRange(0, (int)mNextNode->mAdjacent.size()-1);
    }
    mPrevNode = mNextNode;
    mNextNode = mNextNode->mAdjacent.at(index);
}
void GhostAI::GhostPath(bool collision){
    if(mGhost->GetType() == Ghost::Type::Blinky){
        while(!CalculatePath(mNextNode, mOwner->GetGame()->mPlayer->GetPrevNode())){
            failsafe();
        }
    }
    else if(mGhost->GetType() == Ghost::Type::Pinky){
        Vector2 new_point = mOwner->GetGame()->mPlayer->GetPointInFrontOf(80.0f);
        float shortest = 10000.0f;
        PathNode* node = nullptr;
        for(PathNode* n: mOwner->GetGame()->mPathNodes){
            if((n->GetPosition() - new_point).Length() < shortest && n!= mOwner->GetGame()->mPlayer->GetPrevNode()){
                shortest =(n->GetPosition() - new_point).Length();
                node = n;
            }
        }
        while(!CalculatePath(mNextNode, node)){
            failsafe();
        }
    }
    else if(mGhost->GetType() == Ghost::Type::Inky){
        Vector2 point_p =mOwner->GetGame()->mPlayer->GetPointInFrontOf(40.0f);
        Vector2 new_point = 2*(point_p - mOwner->GetGame()->mGhosts[0]->GetPosition()) + mOwner->GetGame()->mGhosts[0]->GetPosition();
        float shortest = 10000.0f;
        PathNode* node = nullptr;
        for(PathNode* n: mOwner->GetGame()->mPathNodes){
            if((n->GetPosition() - new_point).Length() < shortest && n!= mOwner->GetGame()->mPlayer->GetPrevNode()){
                shortest =(n->GetPosition() - new_point).Length();
                node = n;
            }
        }
        while(!CalculatePath(mNextNode, node)){
            failsafe();
        }
    }
    else if(mGhost->GetType() == Ghost::Type::Clyde){
        if((mGhost->GetPosition() - mOwner->GetGame()->mPlayer->GetPosition()).Length() > 150.0f){
            while(!CalculatePath(mNextNode, mOwner->GetGame()->mPlayer->GetPrevNode())){
                failsafe();
            }
        }
        else{
            while(!CalculatePath(mNextNode, mTargetNode)){
                failsafe();
            }
        }
    }
    if(!collision){
        mPath.push_back(mNextNode);
        mNextNode=mPrevNode;
        mPrevNode= nullptr;
    }
}

