#ifndef YAH_TYPES_H
#define YAH_TYPES_H

#include <stdint.h>
#include <stddef.h>

typedef enum 
{
    YAH_TYPE_NIL = 0,
    YAH_TYPE_INT,
    YAH_TYPE_FLOAT,
    YAH_TYPE_VEC4,
    YAH_TYPE_HASH,
    YAH_TYPE_ARRAY
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
}



#endif