#ifndef YAH_TYPES_H
#define YAH_TYPES_H

typedef enum 
{
    YAH_TYPE_NIL = 0,
    YAH_TYPE_INT,
    YAH_TYPE_FLOAT,
    YAH_TYPE_VEC4,
    YAH_TYPE_HASH,
    YAH_TYPE_ARRAY,
    YAH_TYPE_DICT
}YAH_TYPE;

struct yah_dict;  

// Since the dictionary is "interpreted"
// the value to save must have al the interpretations of said value
typedef struct yah_value 
{
    YAH_TYPE type; // The cast type
    union // The union operator declares memory just for the biggest element declared inside
          // it's a way to say "im saving one of these best case i just need 4 bytes worst 16 bytes"
    {
        int i;
        float f;
        float vec[4];
        uint32_t hash;
        struct 
        {
            struct yah_value *items;
            int count;
        }array;
        struct yah_dict *dict;
    }data;
}YAH_VALUE;

// KEY-VALUE PAIR
typedef struct yah_entry
{
    uint32_t key; 
    YAH_VALUE value;  // we are saving one of the types declared at the union plus the cast type
}YAH_ENTRY;

typedef struct yah_dict
{
    YAH_ENTRY *entries;
    int count;      
    int capacity; // When count reaches capacity the dict will realloc its entries
}YAH_DICT;

YAH_DICT *yahCreateDict(int initialCapacity);
YAH_VALUE yahValueFromInt(int i);
YAH_VALUE yahValueFromFloat(float f);
YAH_VALUE yahValueFromDict(YAH_DICT *dict);
YAH_VALUE yahValueFromVec4(const float vec[4]);
YAH_VALUE yahValueFromHash(uint32_t hashVal);
YAH_VALUE yahValueFromArray(YAH_VALUE *itemsArray, int itemCount);
int yahDictSetFloat(YAH_DICT *dict, uint32_t keyHash, float f);
int yahDictSetInt(YAH_DICT *dict, uint32_t keyHash, int i);
int yahDictSetDict(YAH_DICT *dict, uint32_t keyHash, YAH_DICT *subDict);
int yahDictSetVec4(YAH_DICT *dict, uint32_t keyHash, const float vec[4]);
int yahDictSetHash(YAH_DICT *dict, uint32_t keyHash, uint32_t hashVal);
int yahDictSetArray(YAH_DICT *dict, uint32_t keyHash, YAH_VALUE *itemsArray, int itemCount);
int yahDictSet(YAH_DICT *dict, uint32_t keyHash, YAH_VALUE val);
YAH_VALUE *yahDictGet(YAH_DICT *dict, uint32_t keyHash);
float *yahDictGetFloat(YAH_DICT *dict, uint32_t keyHash);
int *yahDictGetInt(YAH_DICT *dict, uint32_t keyHash);
float *yahDictGetVec4(YAH_DICT *dict, uint32_t keyHash);
uint32_t *yahDictGetHash(YAH_DICT *dict, uint32_t keyHash);
YAH_DICT *yahDictGetDict(YAH_DICT *dict, uint32_t keyHash);

#endif