#include "yah_gameloop.h"
#include <stdlib.h>
#include <string.h>

static void syncDictBuffers(YAH_DICT *src, YAH_DICT *dst)
{
    if (!src || !dst) 
        return;

    if (dst->capacity < src->count)
    {
        int newCap = src->count;
        YAH_ENTRY *newEntries = (YAH_ENTRY *)realloc(dst->entries, sizeof(YAH_ENTRY) * newCap);
        if (!newEntries)
            return;
        dst->entries = newEntries;
        dst->capacity = newCap;
    }

    memcpy(dst->entries, src->entries, sizeof(YAH_ENTRY) * src->count);
    dst->count = src->count;
}

YAH_GAME_LOOP *yahCreateGameLoop(YAH_DICT *initialWorld, YAH_VTABLE *vt, float targetFPS)
{
    if (!initialWorld || !vt) 
        return NULL;

    YAH_GAME_LOOP *loop = (YAH_GAME_LOOP *)malloc(sizeof(YAH_GAME_LOOP));
    if (!loop) 
        return NULL;

    loop->currentWorld = initialWorld;
    loop->nextWorld = yahCreateDict(initialWorld->capacity);
    syncDictBuffers(loop->currentWorld, loop->nextWorld);

    loop->vtable = vt;
    loop->targetFPS = (targetFPS <= 0.0f) ? 60.0f : targetFPS;
    loop->fixedDeltaTime = 1.0f / loop->targetFPS;
    loop->accumulator = 0.0f;

    return loop;
}

void yahGameLoopSwapBuffers(YAH_GAME_LOOP *loop)
{
    if (!loop) 
        return;

    YAH_DICT *temp = loop->currentWorld;
    loop->currentWorld = loop->nextWorld;
    loop->nextWorld = temp;
}

void yahGameLoopTick(YAH_GAME_LOOP *loop, float realDeltaTime)
{
    if (!loop) return;

    loop->accumulator += realDeltaTime;

    static uint32_t actionKeyHash = 0;
    if (actionKeyHash == 0) 
        actionKeyHash = yahHashString("action");

    while (loop->accumulator >= loop->fixedDeltaTime)
    {
        syncDictBuffers(loop->currentWorld, loop->nextWorld);
        for (int i = 0; i < loop->currentWorld->count; i++)
        {
            if (loop->currentWorld->entries[i].value.type == YAH_TYPE_DICT)
            {
                YAH_DICT *readEntity = loop->currentWorld->entries[i].value.data.dict;
                YAH_DICT *writeEntity = loop->nextWorld->entries[i].value.data.dict;

                uint32_t *actHash = yahDictGetHash(readEntity, actionKeyHash);
                if (actHash)
                {
                    yahVTableDispatch(loop->vtable, *actHash, writeEntity, loop->fixedDeltaTime);
                }
            }
        }

        yahGameLoopSwapBuffers(loop);

        loop->accumulator -= loop->fixedDeltaTime;
    }
}

void yahFreeGameLoop(YAH_GAME_LOOP *loop)
{
    if (!loop) 
        return;
    free(loop);
}