#pragma once
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();
	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
    void AddBlock();
    void RemoveBlock();
    void AddBlock(class Block* block);
    void RemoveBlock(class Block* block);
	Mix_Chunk* GetSound(const std::string& fileName);
    void LoadBlock();
	class Renderer* GetRenderer() {	return mRenderer; }
    class Player* player;
    std::string fileInput = "Assets/Blocks/1.txt";
    int current_file_index = 1;
    int current_file_previous = 0;
    std::string changeFileToRead(int index, int previous);
    bool random_Create = false;
    std::vector<class Block*> mBlock;
    std::unordered_map<std::string, class Block*> bMap;
    int channel = 0;
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
    int Game_block_position = 1000;
	std::unordered_map<std::string, Mix_Chunk*> mSounds;

	// All the actors in the game
	std::vector<class Actor*> mActors;
    std::vector<class SideBlock*> LBlock;
    std::vector<class SideBlock*> RBlock;
    int color_pattern[4] = {1,2,4,2};
    int current_color_index = 0;
    class Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
    float block_width = 25.0f;
};
