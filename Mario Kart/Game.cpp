//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include <algorithm>
#include "Actor.h"
#include <fstream>
#include "Renderer.h"
#include "Random.h"
#include "Player.hpp"
#include "SideBlock.hpp"
#include "Block.hpp"
#include <iostream>
#include "MeshComponent.h"
#include "Random.h"
Game::Game()
:mIsRunning(true)
{
	
}

bool Game::Initialize()
{
	Random::Init();
    
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// TODO: Create renderer
    mRenderer = new Renderer(this);
    if(!mRenderer->Initialize(1024.0f, 768.0f)){
        SDL_Log("Unable to initialize Renderer: %s", SDL_GetError());
        return false;
    }
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	LoadData();

	mTicksCount = SDL_GetTicks();
	
	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	for (auto actor : mActors)
	{
		actor->ProcessInput(state);
	}
}

void Game::UpdateGame()
{
	// Compute delta time
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.01667f)
	{
		deltaTime = 0.01667f;
	}
	mTicksCount = SDL_GetTicks();
	
	// Make copy of actor vector
	// (iterate over this in case new actors are created)
	std::vector<Actor*> copy = mActors;
	// Update all actors
	for (auto actor : copy)
	{
		actor->Update(deltaTime);
	}

	// Add any actors to destroy to a temp vector
	std::vector<Actor*> destroyActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == ActorState::Destroy)
		{
			destroyActors.emplace_back(actor);
		}
	}

	// Delete the destroyed actors (which will
	// remove them from mActors)
	for (auto actor : destroyActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	// TODO: tell renderer to draw
    mRenderer->Draw();
}

void Game::LoadData()
{
    player = new Player(this);
    Matrix4 projectionMatrix = Matrix4::CreatePerspectiveFOV(1.22f, 1024.0f, 768.0f, 10.0f, 10000.0f);
    mRenderer->SetProjectionMatrix(projectionMatrix);
    Matrix4 viewMatrix = Matrix4::CreateLookAt(Vector3(-300, 0, 100), Vector3(20,0,0), Vector3::UnitZ);
    mRenderer->SetViewMatrix(viewMatrix);
    size_t x = 0;
    for(size_t i = 0; i <7; i++){
        x=500*i;
        current_color_index = current_color_index%4;
        SideBlock* sb1 = new SideBlock(this);
        SideBlock* sb2 = new SideBlock(this);
        sb1->SetPosition(Vector3(x,-500,0));
        sb2->SetPosition(Vector3(x, 500, 0));
        sb1->mc->SetTextureIndex(color_pattern[current_color_index]);
        sb2->mc->SetTextureIndex(color_pattern[current_color_index]);
        LBlock.push_back(sb1);
        RBlock.push_back(sb2);
        current_color_index++;
    }
    LoadBlock();
    LoadBlock();
    channel = Mix_PlayChannel(-1, GetSound("Assets/Sounds/ShipLoop.wav"), -1);
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy sounds
	for (auto s : mSounds)
	{
		Mix_FreeChunk(s.second);
	}
	mSounds.clear();
}

Mix_Chunk* Game::GetSound(const std::string& fileName)
{
	Mix_Chunk* chunk = nullptr;
	auto iter = mSounds.find(fileName);
	if (iter != mSounds.end())
	{
		chunk = iter->second;
	}
	else
	{
		chunk = Mix_LoadWAV(fileName.c_str());
		if (!chunk)
		{
			SDL_Log("Failed to load sound file %s", fileName.c_str());
			return nullptr;
		}

		mSounds.emplace(fileName, chunk);
	}
	return chunk;
}

void Game::Shutdown()
{
	UnloadData();
	Mix_CloseAudio();
	// TODO: Delete renderer
    mRenderer->Shutdown();
    delete mRenderer;
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	mActors.emplace_back(actor);
}
void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = mActors.end() - 1;
		std::iter_swap(iter, iter2);
		mActors.pop_back();
	}
}
void Game::RemoveBlock(){
    SideBlock*lb = LBlock.front();
    SideBlock*rb = RBlock.front();
    LBlock.erase(LBlock.begin());
    RBlock.erase(RBlock.begin());
    lb->SetState(ActorState::Destroy);
    rb->SetState(ActorState::Destroy);
}
void Game::AddBlock(){
    SideBlock* sb1 = new SideBlock(this);
    SideBlock* sb2 = new SideBlock(this);
    sb1->SetPosition(Vector3(LBlock.back()->GetPosition().x + 500,-500,0));
    sb2->SetPosition(Vector3(RBlock.back()->GetPosition().x + 500, 500, 0));
    current_color_index = current_color_index%4;
    sb1->mc->SetTextureIndex(color_pattern[current_color_index]);
    sb2->mc->SetTextureIndex(color_pattern[current_color_index]);
    current_color_index++;
    LBlock.push_back(sb1);
    RBlock.push_back(sb2);
}
void Game::AddBlock(Block* block){
    mBlock.push_back(block);
}
void Game::RemoveBlock(Block* block){
    std::vector<Block*>::iterator it = std::find(mBlock.begin(), mBlock.end(), block);
    if (it != mBlock.end())
    {
        mBlock.erase(it);
    }
    Vector3 position = block->GetPosition();
    std::string key = std::to_string((int)position.x) + std::to_string((int)position.y)+std::to_string((int)position.z);
    if(bMap.find(key)!=bMap.end()){
        bMap.erase(bMap.find(key));
    }
}
void Game::LoadBlock(){
    if(random_Create){
        current_file_previous = current_file_index;
        current_file_index = Random::GetIntRange(0, 20);
    }
    if(current_file_index>20){
        random_Create = true;
        current_file_index = Random::GetIntRange(0, 20);
    }
    changeFileToRead(current_file_index, current_file_previous);
    std::ifstream filein(fileInput);
    float y = -237.5f;
    float z = 237.5f;
    for (std::string line; std::getline(filein, line); )
    {
        for(char& c: line){
            if(c =='B'){
                Block* b = new Block(this);
                b->SetPosition(Vector3(Game_block_position,y,z));
                b->SetExplode(true);
                std::string key = std::to_string((int)Game_block_position) + std::to_string((int)y)+std::to_string((int)z);
                bMap[key] = b;
                AddBlock(b);
            }
            else if(c=='A'){
                Block* b = new Block(this);
                b->SetPosition(Vector3(Game_block_position,y,z));
                std::string key = std::to_string((int)Game_block_position) + std::to_string((int)y)+std::to_string((int)z);
                bMap[key] = b;
                AddBlock(b);
            }
            y+=block_width;
        }
        y = -237.5f;
        z-=block_width;
    }
    Game_block_position+=1000;
    if(!random_Create){
        current_file_previous = current_file_index;
        current_file_index++;
    }
}
std::string Game::changeFileToRead(int index, int previous){
    if(previous>=10){
        fileInput.replace(fileInput.size()-6, 2, std::to_string(index));
    }
    else{
        fileInput.replace(fileInput.size()-5, 1, std::to_string(index));
    }
    return fileInput;
}
