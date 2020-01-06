#pragma once
#include "SDL/SDL.h"
#include "Math.h"
#include "SDL/SDL_mixer.h" 
#include <cstdio>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <string>
#include <set>
class Game{
public:
    std::vector<class Actor*> mActors;
    std::vector<class SpriteComponent*> SComponents;
    std::vector<class Collider*> mCollider;
    std::vector<class SecretBlock*> mSecret;
    std::vector<class Heart*> mHeart;
    std::unordered_map<std::string, SDL_Texture* >Map;
    std::unordered_map<std::string, std::vector<class Door*> >RoomDoors;
    std::set<std::string> RoomVisited;
    std::unordered_map<std::string, std::vector<class Spawner*> >RoomSpawners;
    std::unordered_map<std::string, std::vector<class Actor*> >RoomEnemies;
    std::unordered_map<std::string, Mix_Chunk* >SoundMap;
    std::string room;
    int channel;
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    int Height_resolution = 448;
    int key_count = 0;
    int Width_resolution = 600;
    int PreviousTick = 0;
    float Delta_Time = 0;
    int WallWidth = 32;
    int TopLeftX = 16;
    int TopLeftY = 16;
    int BlockWidth = 32;
    int BlockHeight = 32;
    float TileWidth = 32.0f;
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
    void addDoor(class Door*door);
    void addSpawner(class Spawner* spawner);
    void addActor(class Actor* actor);
    void removeActor(class Actor* actor);
    void AddSprite(class SpriteComponent* sprite);
    void AddSecret(class SecretBlock* sBlock);
    void RemoveSprite(class SpriteComponent* sprite);
    void AddEnemy(class Actor* enemy);
    void RemoveEnemy(class Actor* enemy);
    void AddHeart(class Heart* heart);
    void RemoveHeart();
    void ActivateEnemy(std::string room);
    void DeactivateEnemy(std::string room);
};
// TODO
