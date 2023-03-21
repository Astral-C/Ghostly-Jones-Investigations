#ifndef __GH_NPC__
#define __GH_NPC__
#include <holo_entity.h>

typedef struct {
    float y_wave;
    char* current_dialog;
} ghNPCData;

void ghNPCSpawn(char* config_path);

#endif