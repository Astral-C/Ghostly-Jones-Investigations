#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <spice_math.h>
#include <spice_sprite.h>
#include <spice_mesh.h>
#include "ini.h"

typedef enum {
    HAND,
    LOOK,
    TALK
} CursorMode;

typedef struct ENT_S {
    uint8_t _inuse;
    uint16_t _id;
    float frame;
    float radius;

    int32_t health;

    sp_sprite* sprite;
    sp_point_sprite* billboard;
    sp_mesh* mesh;

    sp_vec3 position;
    sp_vec3 velocity;
    sp_bounding_box hitbox;


    struct ENT_S* next;
    struct ENT_S* previous;
    struct ENT_S* target;

    ini_t* config;

    char data[64];

    void (*think)(struct ENT_S* self);
    void (*update)(struct ENT_S* self);

    void (*interact)(struct ENT_S* self);
    void (*inspect)(struct ENT_S* self);
    void (*talk)(struct ENT_S* self);

    void (*free)(struct ENT_S* self);
    
} holo_entity;

typedef struct ENTITY_MANAGER_S {
    uint16_t _max_entities;
    holo_entity* entities;
    
    holo_entity* free_list;
    holo_entity* used_list;

} holo_entity_manager;

void holoEntityManagerInit(uint32_t max_entities);
void holoEntityManagerExit();

holo_entity* holoNewEntity();

void holoEntityManagerClick(tm_vec3 camera_position, tm_vec3 ray_dir, CursorMode mode, int* hit);

void holoEntityManagerThink();

void holoEntityManagerUpdate();

void holoEntityManagerDraw();

void holoEntityManagerPrintState();

void holoEntityClear();

void holoEntityFree(holo_entity* entity);

#endif