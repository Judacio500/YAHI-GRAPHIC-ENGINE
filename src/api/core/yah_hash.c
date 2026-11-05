#include "yah_hash.h"
#include <stdint.h>
#include <stddef.h>


uint32_t yahHashString(const char *str)
{
    if (!str) 
        return 0;
    
    uint32_t hash = 2166136261u; // Offset basis FNV-1a
    while (*str)
    {
        hash ^= (uint8_t)(*str);
        hash *= 16777619u;      // FNV Prime
        str++;
    }
    return hash;
}

uint32_t yahHashStringLength(const char *str, int length)
{
    if (!str || length <= 0) 
        return 0;
    
    uint32_t hash = 2166136261u;

    for (int i = 0; i < length; i++)
    {
        hash ^= (uint8_t)str[i];
        hash *= 16777619u;
    }
    return hash;
}