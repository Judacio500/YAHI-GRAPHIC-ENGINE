#ifndef YAH_VTABLE_H
#define YAH_VTABLE_H

#include "yah_types.h"
#include "yah_hash.h"

typedef void (*YAH_ACTION_FN)(YAH_DICT *entity, float dt);

typedef struct 
{
    uint32_t hash;
    YAH_ACTION_FN fn;
}YAH_VTABLE_ENTRY;

typedef struct 
{
    YAH_VTABLE_ENTRY *entries;
    int count;
    int capacity;
} YAH_VTABLE;

YAH_VTABLE *yahCreateVTable(int capacity);
int yahVTableRegister(YAH_VTABLE *vt, const char *actionName, YAH_ACTION_FN fn);
void yahVTableDispatch(YAH_VTABLE *vt, uint32_t actionHash, YAH_DICT *entity, float dt);
void yahFreeVTable(YAH_VTABLE *vt);
void yahVTableDispatchEntity(YAH_VTABLE *vt, YAH_DICT *entity, float dt);
void yahVTableDispatchWorld(YAH_VTABLE *vt, YAH_DICT *world, float dt);

#endif