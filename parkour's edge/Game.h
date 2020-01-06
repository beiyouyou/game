#pragma once
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include <queue>
#include <SDL/SDL_ttf.h>
class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();
	void AddActor(class Actor* actor);
    void AddBlock(class Block* block);
	void RemoveActor(class Actor* actor);
    void RemoveBlock(class Block* block);
    void AddCheck(class Checkpoint* check);
    void AddCamera(class SecurityCamera* camera);
    void RemoveCamera(class SecurityCamera* camera);
    class Checkpoint* removeCheck();
	Mix_Chunk* GetSound(const std::string& fileName);
	class Renderer* GetRenderer() {	return mRenderer; }
    class Player* player;
    int channel = 0;
    // All the actors in the game
    std::vector<class Actor*> mActors;
    std::vector<class Block*> mBlocks;
    std::vector<class SecurityCamera*> mSecurityCameras;
    std::queue<class Checkpoint*> mChecks;
    void SetNextLevel(std::string level){mNextLevel = level;}
    void LoadNextLevel();
    float timer = 0.0f;
    int coin = 0;
    float Text_timer = 0.0f;
    std::string Screen_text;
    bool hasText = false;
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
    std::string mNextLevel;
	std::unordered_map<std::string, Mix_Chunk*> mSounds;
    class Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
};
