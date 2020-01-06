#pragma once
#include "Component.h"
#include "Math.h"
#include <vector>

struct Info{
    bool closed = false;
    float g = 0.0f;
    float f = 0.0f;
    float h = 0.0f;
    class PathNode* parent = nullptr;
};
class GhostAI : public Component
{
public:
	// Used to track the four different GhostAI states
	enum State
	{
		Scatter,
		Chase,
		Frightened,
		Dead
	};
	GhostAI(class Actor* owner);

	void Update(float deltaTime) override;
	
	// Called when the Ghost starts at the beginning
	// (or when the ghosts should respawn)
	void Start(class PathNode* startNode);
	
	// Get the current state
	State GetState() const { return mState; }
	
	// Called when the ghost should switch to the "Frightened" state
	void Frighten();
	
	// Called when the ghost should switch to the "Dead" state
	void Die();

	//  Helper function to draw GhostAI's current path
	void DebugDrawPath(struct SDL_Renderer* render);
private:
	// Member data for pathfinding
    float scatter_speed = 90.0f;
    float frighten_speed = 65.0f;
    float frighten_second = 0.0f;
    float dead_speed = 125.0f;
    float scatter_second = 0.0f;
    float chase_second = 0.0f;
	// TargetNode is our current goal node
	class PathNode* mTargetNode = nullptr;
	// PrevNode is the last node we intersected
	// with prior to the current position
	class PathNode* mPrevNode = nullptr;
	// NextNode is the next node we're trying
	// to get to
	class PathNode* mNextNode = nullptr;

	// This vector always contains the path
	// from "next node" to "target node"
	// (if there is still such a path)
	std::vector<class PathNode*> mPath;

	// Current state of the Ghost AI
	State mState = Scatter;
	// Save the owning actor (cast to a Ghost*)
	class Ghost* mGhost;
	// TODO: Add any member data/helper functions here!
    bool arrive_End = false;
    bool CalculatePath(class PathNode* startNode, class PathNode* scatterNode);
    double EuclideanDistance(class PathNode* startNode, class PathNode* endNode);
    void ChaseAlgorithm(float deltatime);
    void failsafe();
    void GhostPath(bool collision);
};
