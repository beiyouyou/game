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
#include "Block.hpp"
#include "Block.hpp"
#include "Player.hpp"
#include "Goomba.hpp"
#include "Spawner.hpp"
#include "CollisionComponent.h"
#include <iostream>
#include <set>
#include <fstream>
Game::Game(){
    renderer = nullptr;
    window = nullptr;
    Camera = Vector2(0,0);
    player = nullptr;
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
    Delta_Time =currentTime-PreviousTick > 33? 0.033: (float)(currentTime - PreviousTick)/1000;
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
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
            SDL_Log("fail to load");
        }
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
        SDL_FreeSurface(image);
        Map.insert({file, texture});
    }
    return Map.find(file)->second;
}
void Game::LoadData(){
    //set the default background
    channel = Mix_PlayChannel(-1, GetSound("Assets/Sounds/Music.ogg"), -1);
    Actor* background = new Actor(this);
    background->SetPosition(Vector2(3392, 224));
    SpriteComponent* backgroundSprite = new SpriteComponent(background, 90);
    backgroundSprite->SetTexture(GetTexture("Assets/Background.png"));
    //form the map;
    std::ifstream filein("Assets/Level1.txt");
    int x = TopLeftX;
    int y = TopLeftY;
    std::set<int> invisible_block;
    for (std::string line; std::getline(filein, line); )
    {
        for(char& c: line){
            if(c=='.'){
                x+=BlockWidth;
                continue;
            }
            if(c =='Y'){
                Spawner * spawner = new Spawner(this);
                spawner->SetPosition(Vector2(x,y));
            }
            if(c=='A'){
                Block* block = new Block(this);
                block->SetPosition(Vector2(x,y));
                block->ChangeTexture(GetTexture("Assets/BlockA.png"));
            }
            if(c=='B'){
                Block* block = new Block(this);
                block->SetPosition(Vector2(x,y));
                block->ChangeTexture(GetTexture("Assets/BlockB.png"));
            }
            if(c=='C'){
                Block* block = new Block(this);
                block->SetPosition(Vector2(x,y));
                block->ChangeTexture(GetTexture("Assets/BlockC.png"));
            }
            if(c=='D'){
                Block* block = new Block(this);
                block->SetPosition(Vector2(x,y));
                block->ChangeTexture(GetTexture("Assets/BlockD.png"));
                block->mCollision->SetSize(0.0f, 0.0f);
                if(invisible_block.find(x)==invisible_block.end()){
                    invisible_block.insert(x);
                    Block* newBlock = new Block(this);
                    newBlock->SetPosition(Vector2(x, (Height_resolution-WallWidth+y-BlockHeight/2.0f)/2.0f));
                    newBlock->mCollision->SetSize(BlockWidth, Height_resolution-WallWidth-(y-BlockHeight/2.0f));
                }
            }
            if(c=='E'){
                Block* block = new Block(this);
                block->SetPosition(Vector2(x,y));
                block->ChangeTexture(GetTexture("Assets/BlockE.png"));
            }
            if(c=='F'){
                Block* block = new Block(this);
                block->SetPosition(Vector2(x,y));
                block->mCollision->SetSize(0.0f, 0.0f);
                block->ChangeTexture(GetTexture("Assets/BlockF.png"));
                //invisible block is not created yet
                if(invisible_block.find(x)==invisible_block.end()){
                    invisible_block.insert(x);
                    Block* newBlock = new Block(this);
                    newBlock->SetPosition(Vector2(x+BlockWidth/2.0f, (Height_resolution-WallWidth+y-BlockHeight/2.0f)/2.0f));
                    newBlock->mCollision->SetSize(BlockWidth*2, Height_resolution-WallWidth-(y-BlockHeight/2.0f));
                }
            }
            if(c=='G'){
                Block* block = new Block(this);
                block->SetPosition(Vector2(x,y));
                block->ChangeTexture(GetTexture("Assets/BlockG.png"));
                block->mCollision->SetSize(0.0f, 0.0f);
            }
            if(c=='H'){
                Block* block = new Block(this);
                block->SetPosition(Vector2(x,y));
                block->ChangeTexture(GetTexture("Assets/BlockH.png"));
                block->mCollision->SetSize(0.0f, 0.0f);
            }
            if(c=='I'){
                Block* block = new Block(this);
                block->SetPosition(Vector2(x,y));
                block->ChangeTexture(GetTexture("Assets/BlockI.png"));
                block->mCollision->SetSize(0.0f, 0.0f);
            }
            if(c=='P'){
                player = new Player(this);
                player->SetPosition(Vector2(x,y));
            }
            x+=BlockWidth;
        }
        y+= BlockHeight;
        x = TopLeftX;
    }
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
void Game::AddBlock(Block *block){
    mBlock.push_back(block);
}
void Game::RemoveBlock(Block *block){
    std::vector<Block*>::iterator it;
    it = std::find(mBlock.begin(), mBlock.end(), block);
    if(it != mBlock.end() ){
        mBlock.erase(it);
    }
}
void Game::AddEnermy(Goomba *enermy){
    mEnermy.push_back(enermy);
}
void Game::RemoveEnermy(Goomba *enermy){
    std::vector<Goomba*>::iterator it;
    it = std::find(mEnermy.begin(), mEnermy.end(), enermy);
    if(it != mEnermy.end() ){
        mEnermy.erase(it);
    }
}
Mix_Chunk* Game::GetSound(const std::string& filename){
    const char* file = filename.c_str();
    if(SoundMap.find(file) == SoundMap.end()){
        Mix_Chunk *sound;
        sound = Mix_LoadWAV(file);
        if(sound == nullptr){
            SDL_Log("fail to load");
        }
        SoundMap.insert({file, sound});
    }
    return SoundMap.at(file);
}
// TODO
