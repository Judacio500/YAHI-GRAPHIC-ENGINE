#ifndef YAH_HASH_H
#define YAH_HASH_H

// FNV-1a Algorithm String to 32 bits
uint32_t yahHashString(const char *str);
uint32_t yahHashStringLength(const char *str, int length);

#endif