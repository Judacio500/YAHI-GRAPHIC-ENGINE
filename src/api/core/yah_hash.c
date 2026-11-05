#include "yah_hash.h"

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