#ifndef __GH_PLAYER__
#define __GH_PLAYER__
#include <holo_entity.h>
#include <spice_cam.h>

typedef struct {
    float y_wave;
} ghPlayerData;

holo_entity* ghPlayerSpawn();


#endif