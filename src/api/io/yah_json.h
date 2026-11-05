#ifndef YAH_JSON_H
#define YAH_JSON_H

#include "../yah_types.h"

// Esta es la unica funcion que se expone del API o sea se, la unica que puede usar el usuario
YAH_DICT *yahParseJson(const char *jsonString);

#endif