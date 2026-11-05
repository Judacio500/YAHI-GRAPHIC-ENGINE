#include "yah_vtable.h"
#include <stdlib.h>

YAH_VTABLE *yahCreateVTable(int capacity)
{
    YAH_VTABLE *vt = (YAH_VTABLE *)malloc(sizeof(YAH_VTABLE));
    if (!vt) return NULL;
    
    vt->capacity = (capacity <= 0) ? 16 : capacity;
    vt->count = 0;
    vt->entries = (YAH_VTABLE_ENTRY *)malloc(sizeof(YAH_VTABLE_ENTRY) * vt->capacity);
    return vt;
}

int yahVTableRegister(YAH_VTABLE *vt, const char *actionName, YAH_ACTION_FN fn)
{
    if (!vt || !actionName || !fn) return -1;

    uint32_t actionHash = yahHashString(actionName);

    if (vt->count >= vt->capacity) 
    {
        int newCap = vt->capacity * 2;
        YAH_VTABLE_ENTRY *newEntries = (YAH_VTABLE_ENTRY *)realloc(vt->entries, sizeof(YAH_VTABLE_ENTRY) * newCap);
        if (!newEntries) 
            return -1;
        vt->entries = newEntries;
        vt->capacity = newCap;
    }

    vt->entries[vt->count].hash = actionHash;
    vt->entries[vt->count].fn = fn;
    vt->count++;
    return 0;
}

void yahVTableDispatch(YAH_VTABLE *vt, uint32_t actionHash, YAH_DICT *entity, float dt)
{
    if (!vt || !entity) 
        return;

    for (int i = 0; i < vt->count; i++) 
    {
        if (vt->entries[i].hash == actionHash) 
        {
            vt->entries[i].fn(entity, dt);
            return;
        }
    }
}