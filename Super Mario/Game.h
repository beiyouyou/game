#pragma once
#include "SDL/SDL.h"
#include "Math.h"
#include "SDL/SDL_mixer.h" 
#include <cstdio>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <string>
class Game{
public:
    std::vector<class Actor*> mActors;
    std::vector<class SpriteComponent*> SComponents;
    std::vector<class Block*> mBlock;
    std::vector<class Goomba*> mEnermy;
    std::unordered_map<std::string, SDL_Texture* >Map;
    std::unordered_map<std::string, Mix_Chunk* >SoundMap;
    int channel;
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    int Height_resolution = 448;
    int Width_resolution = 600;
    int PreviousTick = 0;
    float Delta_Time = 0;
    int WallWidth = 32;
    int TopLeftX = 16;
    int TopLeftY = 16;
    int BlockWidth = 32;
    int BlockHeight = 32;
    class Player* player;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Vector2 Camera;
    bool GameOver = false;
    Game();
    SDL_Texture* GetTexture(std::string filename);
    Mix_Chunk* GetSound(const std::string& filename);
    void LoadData();
    void UnLoadData();
    bool Initialize();
    void Shutdown();
    void RunLoop();
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    void addActor(class Actor* actor);
    void removeActor(class Actor* actor);
    void AddSprite(class SpriteComponent* sprite);
    void RemoveSprite(class SpriteComponent* sprite);
    void AddEnermy(class Goomba* enermy);
    void RemoveEnermy(class Goomba * enermy);
    void AddBlock(class Block* block);
    void RemoveBlock(class Block * block);
};
// TODO
