// PROOF OF CONCEPT

#include "yah_renderer_gl.h"
#include "../yah_hash.h"
#include <GL/glut.h> 

void yahRenderWorldGL(YAH_DICT *world)
{
    if(!world) 
        return;

    static uint32_t xHash = 0;
    static uint32_t yHash = 0;
    static uint32_t sizeHash = 0;
    static uint32_t angleHash = 0;
    static uint32_t colorHash = 0;

    if(xHash == 0)
    {
        xHash = yahHashString("x");
        yHash = yahHashString("y");
        sizeHash = yahHashString("size");
        angleHash = yahHashString("angle");
        colorHash = yahHashString("color");
    }

    for(int i = 0; i < world->count; i++)
    {
        if(world->entries[i].value.type != YAH_TYPE_DICT) 
            continue;

        YAH_DICT *entity = world->entries[i].value.data.dict;
        if(!entity) continue;

        float *x = yahDictGetFloat(entity, xHash);
        float *y = yahDictGetFloat(entity, yHash);
        float *size = yahDictGetFloat(entity, sizeHash);
        float *angle = yahDictGetFloat(entity, angleHash);
        float *color = yahDictGetVec4(entity, colorHash); 

        if (!x || !y) 
            continue;

        float renderSize  = size  ? *size  : 20.0f;
        float renderAngle = angle ? *angle : 0.0f;

        glPushMatrix();

        glTranslatef(*x, *y, 0.0f);
        if (renderAngle != 0.0f)
        {
            glRotatef(renderAngle, 0.0f, 0.0f, 1.0f);
        }

        if (color)
        {
            glColor4f(color[0], color[1], color[2], color[3]);
        }
        else
        {
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        }

        float halfSize = renderSize * 0.5f;
        glBegin(GL_QUADS);
            glVertex2f(-halfSize, -halfSize);
            glVertex2f( halfSize, -halfSize);
            glVertex2f( halfSize,  halfSize);
            glVertex2f(-halfSize,  halfSize);
        glEnd();

        glPopMatrix();
    }
}