#ifndef YAH_ACTIONS_H
#define YAH_ACTIONS_H

#include "../yah_types.h"
#include "../yah_vtable.h"

void yahActionMove2D(YAH_DICT *entity, float dt);
void yahActionRotate(YAH_DICT *entity, float dt);
void yahActionFadeColor(YAH_DICT *entity, float dt);
void yahRegisterBaseActions(YAH_VTABLE *vt);

#endif