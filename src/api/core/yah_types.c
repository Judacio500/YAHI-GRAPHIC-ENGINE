#include "yah_types.h"

YAH_DICT *yahCreateDict(int initialCapacity)
{
    YAH_DICT *dict = (YAH_DICT*)malloc(sizeof(YAH_DICT));

    if(!dict)
        return NULL;

    dict->capacity = (initialCapacity <= 0) ? 10 : initialCapacity;
    dict->count = 0;
    dict->entries = (YAH_ENTRY*)malloc(sizeof(YAH_ENTRY)*dict->capacity);(

    if(!dict->entries)
    {
        free(dict);
        return NULL;
    }

    return dict;
}

YAH_VALUE yahValueFromInt(int i) 
{
    YAH_VALUE val;
    val.type = YAH_TYPE_INT;
    val.data.i = i;
    return val;
}

YAH_VALUE yahValueFromFloat(float f) 
{
    YAH_VALUE val;
    val.type = YAH_TYPE_FLOAT;
    val.data.f = f;
    return val;
}

YAH_VALUE yahValueFromDict(YAH_DICT *dict) 
{
    YAH_VALUE val;
    val.type = YAH_TYPE_DICT;
    val.data.dict = dict;
    return val;
}

YAH_VALUE yahValueFromVec4(const float vec[4]) 
{
    YAH_VALUE val;
    val.type = YAH_TYPE_VEC4;
    memcpy(val.data.vec, vec, sizeof(float) * 4);
    return val;
}

YAH_VALUE yahValueFromHash(uint32_t hashVal) 
{
    YAH_VALUE val;
    val.type = YAH_TYPE_HASH;
    val.data.hash = hashVal;
    return val;
}

YAH_VALUE yahValueFromArray(YAH_VALUE *itemsArray, int itemCount) 
{
    YAH_VALUE val;
    val.type = YAH_TYPE_ARRAY;
    val.data.array.items = itemsArray;
    val.data.array.count = itemCount;
    return val;
}

int yahDictSetFloat(YAH_DICT *dict, uint32_t keyHash, float f) 
{
    return yahDictSet(dict, keyHash, yahValueFromFloat(f));
}

int yahDictSetInt(YAH_DICT *dict, uint32_t keyHash, int i) 
{
    return yahDictSet(dict, keyHash, yahValueFromInt(i));
}

int yahDictSetDict(YAH_DICT *dict, uint32_t keyHash, YAH_DICT *subDict) 
{
    return yahDictSet(dict, keyHash, yahValueFromDict(subDict));
}

int yahDictSetVec4(YAH_DICT *dict, uint32_t keyHash, const float vec[4]) 
{
    return yahDictSet(dict, keyHash, yahValueFromVec4(vec));
}

int yahDictSetHash(YAH_DICT *dict, uint32_t keyHash, uint32_t hashVal) 
{
    return yahDictSet(dict, keyHash, yahValueFromHash(hashVal));
}

int yahDictSetArray(YAH_DICT *dict, uint32_t keyHash, YAH_VALUE *itemsArray, int itemCount) 
{
    return yahDictSet(dict, keyHash, yahValueFromArray(itemsArray, itemCount));
}

int yahDictSet(YAH_DICT *dict, uint32_t keyHash, YAH_VALUE val) 
{
    if (!dict) 
        return -1;

    for (int i = 0; i < dict->count; i++) 
    {
        if (dict->entries[i].keyHash == keyHash) 
        {
            dict->entries[i].val = val; 
            return 0;
        }
    }

    if (dict->count >= dict->capacity) 
    {
        int newCap = dict->capacity * 2;
        YAH_ENTRY *newEntries = (YAH_ENTRY *)realloc(dict->entries, sizeof(YAH_ENTRY) * newCap);
        if (!newEntries) 
            return -1;

        dict->entries = newEntries;
        dict->capacity = newCap;
    }

    dict->entries[dict->count].keyHash = keyHash;
    dict->entries[dict->count].val = val;
    dict->count++;

    return 0;
}