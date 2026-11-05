#ifndef YAH_GAMELOOP_H
#define YAH_GAMELOOP_H

#include "../yah_types.h"
#include "../yah_vtable.h"

typedef struct 
{
    YAH_DICT *currentWorld;
    YAH_DICT *nextWorld;
    YAH_VTABLE *vtable;
    
    float targetFPS;
    float fixedDeltaTime;    
    float accumulator;      
} YAH_GAME_LOOP;

YAH_GAME_LOOP *yahCreateGameLoop(YAH_DICT *initialWorld, YAH_VTABLE *vt, float targetFPS);
void yahGameLoopTick(YAH_GAME_LOOP *loop, float realDeltaTime);
void yahGameLoopSwapBuffers(YAH_GAME_LOOP *loop);
void yahFreeGameLoop(YAH_GAME_LOOP *loop);

#endif