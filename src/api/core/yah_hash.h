#ifndef YAH_HASH_H
#define YAH_HASH_H

#include <stdint.h>
#include <stddef.h>

// FNV-1a Algorithm String to 32 bits
uint32_t yahHashString(const char *str);

#endif