//
//  BallMove.cpp
//  Game-mac
//
//  Created by Beiyou Zhu on 9/12/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "PlayerMove.hpp"
#include "CollisionComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Player.hpp"
#include "AnimatedSprite.h"
#include "SecretBlock.hpp"
#include "Collider.hpp"
#include "Door.hpp"
#include "Heart.hpp"
#include "Spawner.hpp"
#include "Sword.hpp"
#include "EnemyComponent.hpp"
#include <iostream>
PlayerMove::PlayerMove(Player* owner)
:MoveComponent(owner)
{
    dir = direction::Up;
    mYSpeed = 0.0f;
    Velocity = Vector2(0.0f, 0.0f);
    player = owner;
    mSpacePressed = false;
    move = false;
    direction = 0;
    sword = new Sword(player->GetGame());
    attack_timer = 0.0f;
}
void PlayerMove::Update(float deltaTime){
    std::string room = player->GetGame()->room;
    player->invulnerability_timer-=deltaTime;
    float forwardSpeed = GetForwardSpeed();
    if(attack_timer>0){
        attack_timer-= deltaTime;
    }
    else if(move){
        if(dir == direction::Left || dir == direction::Right){
            player->SetPosition(Vector2(player->GetPosition().x + forwardSpeed*deltaTime, player->GetPosition().y));
        }
        else{
            player->SetPosition(Vector2(player->GetPosition().x, player->GetPosition().y + forwardSpeed*deltaTime));
        }
    }
    SetAnimation();
    //check collision against enemy
    for(Actor* enemy: player->GetGame()->RoomEnemies[player->GetGame()->room]){
        if(sword->mCollision->Intersect(enemy->GetComponent<CollisionComponent>()) && attack_timer>0){
            enemy->GetComponent<EnemyComponent>()->TakeDamage(1);
            break;
        }
        else{
            Vector2 offset = Vector2(0,0);
            CollSide CSide = player->mCollision->GetMinOverlap(enemy->GetComponent<CollisionComponent>(), offset);
            if(CSide == CollSide::None)continue;
            else{
                player->SetPosition(player->GetPosition() + offset);
                if(dir == direction::Up){
                    player->SetPosition(player->GetPosition() + Vector2(0, 32.0f));
                }
                else if(dir == direction::Down){
                    player->SetPosition(player->GetPosition() + Vector2(0, -32.0f));
                }
                else if(dir == direction::Left){
                    player->SetPosition(player->GetPosition() + Vector2(32.0f, 0));
                }
                else if(dir == direction::Right){
                    player->SetPosition(player->GetPosition() + Vector2(-32.0f, 0));
                }
                player->TakeDamage(enemy->GetComponent<EnemyComponent>()->damage);
            }
        }
    }
    //check if link can go to next room
    for(Door* door: player->GetGame()->RoomDoors.at(room)){
        Vector2 offset = Vector2(0,0);
        CollSide side = player->mCollision->GetMinOverlap(door->mCollision, offset);
        if(side == CollSide::None)continue;
        OpenDoorHelper(door);
    }
    //code to push secret blcok
    for(SecretBlock* b: player->GetGame()->mSecret){
        Vector2 offset = Vector2(0,0);
        CollSide side = player->mCollision->GetMinOverlap(b->mCollision, offset);
        if(side == CollSide::Left && b->Movedirection == "Left" && player->ASprite->GetAnimName() == "walkright" && b->movable){
            forwardSpeed = forwardSpeed>0? forwardSpeed-b->resistance: forwardSpeed+b->resistance;
            float newPos = b->GetPosition().x + forwardSpeed*deltaTime;
            float updatePos = newPos> b->destination? b->destination: newPos;
            if(updatePos == b->destination) b->movable = false;
            b->SetPosition(Vector2(updatePos, b->GetPosition().y ));
        }
        else if(side == CollSide::Top && b->Movedirection == "Up" && player->ASprite->GetAnimName() == "walkdown" && b->movable){
            forwardSpeed = forwardSpeed>0? forwardSpeed-b->resistance: forwardSpeed+b->resistance;
            float newPos = b->GetPosition().y + forwardSpeed*deltaTime;
            float updatePos = newPos> b->destination? b->destination: newPos;
            if(updatePos == b->destination) b->movable = false;
            b->SetPosition(Vector2(b->GetPosition().x, updatePos ));
        }
        else if(side == CollSide::Bottom && b->Movedirection == "Down" && player->ASprite->GetAnimName() == "walkup" && b->movable){
            forwardSpeed = forwardSpeed>0? forwardSpeed-b->resistance: forwardSpeed+b->resistance;
            float newPos = b->GetPosition().y + forwardSpeed*deltaTime;
            float updatePos = newPos< b->destination? b->destination: newPos;
            if(updatePos == b->destination) b->movable = false;
            b->SetPosition(Vector2(b->GetPosition().x, updatePos ));
        }
        else if(side == CollSide::Right && b->Movedirection == "Right" && player->ASprite->GetAnimName() == "walkleft" && b->movable){
            forwardSpeed = forwardSpeed>0? forwardSpeed-b->resistance: forwardSpeed+b->resistance;
            float newPos = b->GetPosition().x + forwardSpeed*deltaTime;
            float updatePos = newPos< b->destination? b->destination: newPos;
            if(updatePos == b->destination) b->movable = false;
            b->SetPosition(Vector2(updatePos, b->GetPosition().y ));
        }
        player->SetPosition(player->GetPosition() + offset);
        if(!b->movable && !b->open_door){
             for(Door* door: player->GetGame()->RoomDoors.at(player->GetGame()->room)){
                 if(door->state == "Closed"){
                     door->Setup(door->destination, door->direction, "Open");
                     Mix_PlayChannel(-1, player->GetGame()->GetSound("Assets/Sounds/Secret.wav"), 0);
                     Mix_PlayChannel(-1, player->GetGame()->GetSound("Assets/Sounds/DoorOpen.wav"), 0);
                 }
             }
            b->open_door = true;
        }
    }
    //trigger object in the new room
    if(player->GetGame()->RoomVisited.find(room) == player->GetGame()->RoomVisited.end()){
        player->GetGame()->RoomVisited.insert(room);
        if(player->GetGame()->RoomSpawners.find(room)!= player->GetGame()->RoomSpawners.end()){
            for(Spawner* s: player->GetGame()->RoomSpawners.at(room)){
                s->Trigger();
            }
        }
    }
    //check if collide with any collider
    for(Collider* c: player->GetGame()->mCollider){
        Vector2 offset = Vector2(0,0);
        CollSide side = player->mCollision->GetMinOverlap(c->mCollision, offset);
        if(side!=CollSide::None){
            player->SetPosition(player->GetPosition() + offset);
        }
    }
    UpdateSword();
    player->GetGame()->Camera.x = player->GetPosition().x -player->GetGame()->Width_resolution/2.0f;
    player->GetGame()->Camera.y = player->GetPosition().y -player->GetGame()->Height_resolution/2.0f;
    float x = player->GetGame()->Camera.x + 16.0f;
    float y = player->GetGame()->Camera.y + 16.0f;
    for(Heart* heart: player->GetGame()->mHeart){
        heart->SetPosition(Vector2(x,y));
        x+=32.0f;
    }
}
void PlayerMove::ProcessInput(const Uint8 *keyState){
    if(keyState[SDL_SCANCODE_SPACE] && !mSpacePressed){
        mSpacePressed = true;
        attack_timer = 0.25f;
        move = false;
    }
    else if(keyState[SDL_SCANCODE_LEFT]){
        SetForwardSpeed(-130.0f);
        move = true;
        dir = direction::Left;
    }
    else if(keyState[SDL_SCANCODE_RIGHT]){
        SetForwardSpeed(130.0f);
        move = true;
        dir = direction::Right;
    }
    else if(keyState[SDL_SCANCODE_UP]){
        SetForwardSpeed(-130.0f);
        move = true;
        dir = direction::Up;
    }
    else if(keyState[SDL_SCANCODE_DOWN]){
        SetForwardSpeed(130.0f);
        move = true;
        dir = direction::Down;
    }
    else{
        move = false;
    }
    if(!keyState[SDL_SCANCODE_SPACE]){
        mSpacePressed = false;
    }
    UpdateSword();
}
void PlayerMove::SetAnimation(){
    if(dir == direction::Up){
        player->ASprite->SetAnimation("walkup");
    }
    else if(dir == direction::Down){
        player->ASprite->SetAnimation("walkdown");
    }
    else if(dir == direction::Left){
        player->ASprite->SetAnimation("walkleft");
    }
    else if(dir == direction::Right){
        player->ASprite->SetAnimation("walkright");
    }
    if(!move && attack_timer<=0.0f){
        player->ASprite->SetIsPaused(true);
    }
    else{
        player->ASprite->SetIsPaused(false);
    }
}
void PlayerMove::OpenDoorHelper(Door *door){
    float travel_dit = player->GetGame()->TileWidth*4;
    bool travel = false;
    std::string previousRoom = player->GetGame()->room;
    if(door->state == "Open" ){
        if(door->direction == "Up" && player->ASprite->GetAnimName() == "walkup"){
            player->GetGame()->room = door->destination;
            player->SetPosition(Vector2(player->GetPosition().x, player->GetPosition().y - travel_dit));
            travel = true;
        }
        else if(door->direction == "Down" && player->ASprite->GetAnimName() == "walkdown"){
            player->GetGame()->room = door->destination;
            player->SetPosition(Vector2(player->GetPosition().x, player->GetPosition().y + travel_dit));
            travel = true;
        }
        else if(door->direction == "Left" && player->ASprite->GetAnimName() == "walkleft"){
            player->GetGame()->room = door->destination;
            player->SetPosition(Vector2(player->GetPosition().x - travel_dit, player->GetPosition().y));
            travel = true;
        }
        else if(door->direction == "Right" && player->ASprite->GetAnimName() == "walkright"){
            player->GetGame()->room = door->destination;
            player->SetPosition(Vector2(player->GetPosition().x + travel_dit, player->GetPosition().y));
            travel = true;
        }
    }
    else if(door->state == "Locked" && player->GetGame()->key_count>0){
        if(door->direction == "Up" && player->ASprite->GetAnimName() == "walkup"){
            player->GetGame()->room = door->destination;
            player->SetPosition(Vector2(player->GetPosition().x, player->GetPosition().y - travel_dit));
             door->Setup(door->destination, door->direction, "Open");
            Mix_PlayChannel(-1, player->GetGame()->GetSound("Assets/Sounds/DoorOpen.wav"), 0);
            player->GetGame()->key_count--;
            travel = true;
        }
        else if(door->direction == "Down" && player->ASprite->GetAnimName() == "walkdown"){
            player->GetGame()->room = door->destination;
            player->SetPosition(Vector2(player->GetPosition().x, player->GetPosition().y + travel_dit));
             door->Setup(door->destination, door->direction, "Open");
            Mix_PlayChannel(-1, player->GetGame()->GetSound("Assets/Sounds/DoorOpen.wav"), 0);
            player->GetGame()->key_count--;
            travel = true;
        }
        else if(door->direction == "Left" && player->ASprite->GetAnimName() == "walkleft"){
            player->GetGame()->room = door->destination;
            player->SetPosition(Vector2(player->GetPosition().x - travel_dit, player->GetPosition().y));
             door->Setup(door->destination, door->direction, "Open");
            Mix_PlayChannel(-1, player->GetGame()->GetSound("Assets/Sounds/DoorOpen.wav"), 0);
            player->GetGame()->key_count--;
            travel = true;
        }
        else if(door->direction == "Right" && player->ASprite->GetAnimName() == "walkright"){
            player->GetGame()->room = door->destination;
            player->SetPosition(Vector2(player->GetPosition().x + travel_dit, player->GetPosition().y));
             door->Setup(door->destination, door->direction, "Open");
            Mix_PlayChannel(-1, player->GetGame()->GetSound("Assets/Sounds/DoorOpen.wav"), 0);
            player->GetGame()->key_count--;
            travel = true;
        }
    }
    //if travel to another room activate the enemies in that room but deactivate the previous room
    if(travel){
        player->GetGame()->ActivateEnemy(player->GetGame()->room);
        player->GetGame()->DeactivateEnemy(previousRoom);
    }
}
void PlayerMove::UpdateSword(){
    if(attack_timer<=0){
        sword->mSprite->SetIsVisible(false);
    }
    else{
        sword->mSprite->SetIsVisible(true);
        if(dir == direction::Up){
            sword->mSprite->SetTexture(player->GetGame()->GetTexture("Assets/Sword/Up.png"));
            sword->SetPosition(player->GetPosition()+Vector2(-8, -16));
             player->ASprite->SetAnimation("attackUp");
        }
        else if(dir == direction::Down){
            sword->mSprite->SetTexture(player->GetGame()->GetTexture("Assets/Sword/Down.png"));
             sword->SetPosition(player->GetPosition()+Vector2(8, 16));
            player->ASprite->SetAnimation("attackDown");
        }
        else if(dir == direction::Left){
            sword->mSprite->SetTexture(player->GetGame()->GetTexture("Assets/Sword/Left.png"));
             sword->SetPosition(player->GetPosition()+Vector2(-16, 4));
            player->ASprite->SetAnimation("attackLeft");
        }
        else if (dir == direction::Right){
            sword->mSprite->SetTexture(player->GetGame()->GetTexture("Assets/Sword/Right.png"));
             sword->SetPosition(player->GetPosition()+Vector2(16, 4));
            player->ASprite->SetAnimation("attackRight");
        }
        Mix_PlayChannel(-1, player->GetGame()->GetSound("Assets/Sounds/SwordSlash.wav"), 0);
    }
}


