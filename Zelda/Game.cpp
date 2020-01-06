//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include <SDL/SDL_image.h>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Random.h"
#include "Player.hpp"
#include "Spawner.hpp"
#include "CSVHelper.h"
#include "TiledBGComponent.hpp"
#include "Collider.hpp"
#include "Door.hpp"
#include "SecretBlock.hpp"
#include "Heart.hpp"
#include <iostream>
#include <fstream>
Game::Game(){
    renderer = nullptr;
    window = nullptr;
    Camera = Vector2(0,0);
    player = nullptr;
    room = "Room1";
    channel = 0;
}
bool Game::Initialize(){
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 0;
    }
    window = SDL_CreateWindow("An SDL Window", 0, 0, Width_resolution, Height_resolution, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(Game::window, -1, SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC);
    IMG_Init(IMG_INIT_PNG);
    Random::Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    LoadData();
    PreviousTick = SDL_GetTicks();
    return 1;
}
void Game::RunLoop(){
    while(!GameOver){
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}
void Game::ProcessInput(){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
            /* handle your event here */
        if(event.type == SDL_QUIT){
            GameOver = true;
            break;
        }
    }
    if(state[SDL_SCANCODE_ESCAPE]){
        GameOver = true;
    }
    std::vector<Actor* > shallowCopy = mActors;
    for(Actor* a: shallowCopy){
        a->ProcessInput(state);
    }
    /* do some other stuff here -- draw your app, etc. */
}
void Game::UpdateGame(){
    int currentTime = SDL_GetTicks();
    while(currentTime - PreviousTick < 16){
        currentTime = SDL_GetTicks();
    }
    Delta_Time =currentTime-PreviousTick > 33? 0.033f: (float)(currentTime - PreviousTick)/1000;
    PreviousTick = currentTime;
    std::vector<Actor* > shallowCopy = mActors;
    //TO MAKE A SHALLOWCOPY FOR THE CASE UPDATES CREATE NEW ACTORS TO THE VECTOR. THE SHALLOW COPY WILL NOT CHANGE
    for(Actor* a: shallowCopy){
        a->Update(Delta_Time);
    }
    std::vector<Actor* >ActorToDestroy;
    for(Actor* a: mActors){
        if(a->GetState() == ActorState::Destroy){
            ActorToDestroy.push_back(a);
        }
    }
    for(Actor* toDelete: ActorToDestroy){
        delete toDelete;
    }
}
void Game::GenerateOutput(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    //SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    for(SpriteComponent* sprite: SComponents){
        if(sprite->IsVisible()){
            sprite->Draw(renderer, Camera);
        }
    }
    SDL_RenderPresent(renderer);
}
void Game::Shutdown(){
    UnLoadData();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    Mix_CloseAudio();
}
SDL_Texture* Game::GetTexture(std::string filename){
    const char* file = filename.c_str();
    if(Map.find(file) == Map.end()){
        SDL_Surface *image;
        image=IMG_Load(file);
        if(image == nullptr){
            std::cout<<"fail to load " <<filename <<"\n";
        }
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
        SDL_FreeSurface(image);
        Map.insert({file, texture});
    }
    return Map.find(file)->second;
}
void Game::LoadData(){
    //load player
    std::string setUpFile ="Assets/Dungeon/Room0.csv";
    for(size_t i = 1; i <= 7; i++){
        room = "Room" + std::to_string(i);
        setUpFile.replace(setUpFile.size()-5, 1, std::to_string(i));;
        std::ifstream filein(setUpFile);
        for (std::string line; std::getline(filein, line); )
        {
            std::vector<std::string> lineData = CSVHelper::Split(line);
            if(lineData[0] == "Type")continue;
            if(lineData[0] == "Player"){
                player = new Player(this);
                player->SetPosition(Vector2(stof(lineData[1]), stof(lineData[2])));
            }
            else if(lineData[0] == "Collider"){
                Collider* c = new Collider(this);
                c->SetPosition(Vector2(stof(lineData[1]) + stof(lineData[3])/2,
                                       stof(lineData[2]) + stof(lineData[4])/2));
                c->setWidthHeight(stof(lineData[3]), stof(lineData[4]));
                mCollider.push_back(c);
            }
            else if (lineData[0] == "Door"){
                Door* door = new Door(this);
                door->SetPosition(Vector2(stof(lineData[1]) + stof(lineData[3])/2,
                                          stof(lineData[2]) + stof(lineData[4])/2));
                door->Setup(lineData[5], lineData[6], lineData[7]);
                addDoor(door);
            }
            else if (lineData[0] == "SecretBlock"){
                SecretBlock* sBlock = new SecretBlock(this);
                sBlock->SetPosition(Vector2(stof(lineData[1]) + stof(lineData[3])/2,
                                            stof(lineData[2]) + stof(lineData[4])/2));
                sBlock->SetDirection(lineData[6]);
                AddSecret(sBlock);
            }
            else{
                Spawner* s = new Spawner(this);
                s->SetPosition(Vector2(stof(lineData[1]) + stof(lineData[3])/2,
                                       stof(lineData[2]) + stof(lineData[4])/2));
                s->type = lineData[0];
                addSpawner(s);
            }
        }
    }
    room = "Room1";
    Actor* map = new Actor(this);
    TiledBGComponent* mapTile = new TiledBGComponent(map);
    mapTile->LoadTileCSV("Assets/Dungeon/DungeonMapBG.csv", 32, 32);
    mapTile->SetTexture(GetTexture("Assets/Dungeon/DungeonTiles.png"));
    //set the default background
    channel = Mix_PlayChannel(-1, GetSound("Assets/Sounds/Music.ogg"), -1);
}
void Game::UnLoadData(){
    while(!mActors.empty()){
        delete mActors.back();
    }
    //delete the map
    for(std::unordered_map<std::string, SDL_Texture*>::iterator it = Map.begin(); it!= Map.end(); it++){
        SDL_DestroyTexture(it->second);
    }
    //delete sound
    for(std::unordered_map<std::string, Mix_Chunk*>::iterator it = SoundMap.begin(); it!= SoundMap.end(); it++){
        Mix_FreeChunk(it->second);
    }
    Map.clear();
    SoundMap.clear();
}
void Game::addActor(Actor *actor){
    mActors.push_back(actor);
}
void Game::addDoor(Door* door){
    RoomDoors[room].push_back(door);
}
void Game::removeActor(Actor* actor){
    std::vector<Actor*>::iterator it;
    it = std::find(mActors.begin(), mActors.end(), actor);
    if(it!=mActors.end()){
        mActors.erase(it);
    }
}
void Game::AddSprite(SpriteComponent* sprite){
    SComponents.push_back(sprite);
    std::sort(SComponents.begin(), SComponents.end(),
              [](SpriteComponent* a, SpriteComponent* b) {
                  return a->GetDrawOrder() < b->GetDrawOrder();
              });
}
void Game::RemoveSprite(SpriteComponent *sprite){
    std::vector<SpriteComponent*>::iterator it;
    it = std::find(SComponents.begin(), SComponents.end(), sprite);
    if(it != SComponents.end() ){
        SComponents.erase(it);
    }
}
void Game::AddEnemy(Actor *enemy){
    RoomEnemies[room].push_back(enemy);
}
void Game::RemoveEnemy(Actor *enemy){
    std::vector<Actor*>::iterator it;
    it = std::find(RoomEnemies[room].begin(), RoomEnemies[room].end(), enemy);
    if(it!= RoomEnemies[room].end()){
        RoomEnemies[room].erase(it);
    }
}
Mix_Chunk* Game::GetSound(const std::string& filename){
    const char* file = filename.c_str();
    if(SoundMap.find(file) == SoundMap.end()){
        Mix_Chunk *sound;
        sound = Mix_LoadWAV(file);
        if(sound == nullptr){
            std::cout<< "fail to load " <<filename <<"\n";
        }
        SoundMap.insert({file, sound});
    }
    return SoundMap.at(file);
}
void Game::AddSecret( SecretBlock *sBlock){
    mSecret.push_back(sBlock);
}
void Game::addSpawner(Spawner *spawner){
    RoomSpawners[room].push_back(spawner);
}
void Game::ActivateEnemy(std::string room){
    if(RoomEnemies.find(room)!=RoomEnemies.end()){
        for(Actor* enemy: RoomEnemies.at(room)){
            enemy->SetState(ActorState::Active);
        }
    }
}
void Game::DeactivateEnemy(std::string room){
    if(RoomEnemies.find(room)!=RoomEnemies.end()){
        for(Actor* enemy: RoomEnemies.at(room)){
            enemy->SetState(ActorState::Paused);
        }
    }
}
void Game::AddHeart(Heart *heart){
    mHeart.push_back(heart);
}
void Game::RemoveHeart(){
    if(mHeart.size()>0){
        mHeart.erase(mHeart.end()-1);
    }
}

// TODO
