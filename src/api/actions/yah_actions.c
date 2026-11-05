#include "yah_actions.h"
#include "../yah_hash.h"

void yahActionMove2D(YAH_DICT *entity, float dt)
{
    static uint32_t xHash = 0, yHash = 0, vxHash = 0, vyHash = 0;
    if(xHash == 0)
    {
        xHash  = yahHashString("x");
        yHash  = yahHashString("y");
        vxHash = yahHashString("vx");
        vyHash = yahHashString("vy");
    }

    float *x  = yahDictGetFloat(entity, xHash);
    float *y  = yahDictGetFloat(entity, yHash);
    float *vx = yahDictGetFloat(entity, vxHash);
    float *vy = yahDictGetFloat(entity, vyHash);

    if(x && vx)
        *x += (*vx) * dt;
    if(y && vy) 
        *y += (*vy) * dt;
}

void yahActionRotate(YAH_DICT *entity, float dt)
{
    static uint32_t angleHash = 0, speedHash = 0;
    if(angleHash == 0)
    {
        angleHash = yahHashString("angle");
        speedHash = yahHashString("rot_speed");
    }

    float *angle = yahDictGetFloat(entity, angleHash);
    float *speed = yahDictGetFloat(entity, speedHash);

    if(angle && speed)
    {
        *angle += (*speed) * dt;
        if(*angle >= 360.0f) 
            *angle -= 360.0f;
    }
}

void yahActionFadeColor(YAH_DICT *entity, float dt)
{
    static uint32_t colorHash = 0, fadeSpeedHash = 0;
    if(colorHash == 0)
    {
        colorHash     = yahHashString("color");
        fadeSpeedHash = yahHashString("fade_speed");
    }

    float *color = yahDictGetVec4(entity, colorHash); 
    float *speed = yahDictGetFloat(entity, fadeSpeedHash);

    if(color && speed)
    {
        color[3] -= (*speed) * dt;
        if (color[3] <= 0.0f)
        {
            color[3] = 1.0f; 
        }
    }
}

void yahRegisterBaseActions(YAH_VTABLE *vt)
{
    yahVTableRegister(vt, "move_2d", yahActionMove2D);
    yahVTableRegister(vt, "rotate", yahActionRotate);
    yahVTableRegister(vt, "fade_color", yahActionFadeColor);
}