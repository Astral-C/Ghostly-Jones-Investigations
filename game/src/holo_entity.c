#include <holo_entity.h>

static holo_entity_manager entity_manager = {0};

void holoEntityManagerInit(uint32_t max_entities){
    entity_manager._max_entities = max_entities;
    entity_manager.entities = malloc(sizeof(holo_entity) * max_entities);

    memset(entity_manager.entities, 0, sizeof(holo_entity) * max_entities);

    entity_manager.used_list = NULL;
    entity_manager.free_list = entity_manager.entities;

    holo_entity* previous = NULL;
    for(size_t i = 0; i < max_entities; i++){
        entity_manager.entities[i]._id = i;
        entity_manager.entities[i].previous = previous;

        previous = &entity_manager.entities[i];
        entity_manager.entities[i].next = &entity_manager.entities[i+1];
    }
    entity_manager.entities[max_entities-1].next = NULL;

    atexit(holoEntityManagerExit);
}

void holoEntityManagerExit(){
    free(entity_manager.entities);
}

holo_entity* holoNewEntity(){
    holo_entity* entity = entity_manager.free_list;
    entity_manager.free_list = entity->next;

    if(entity_manager.used_list != NULL){
        entity_manager.used_list->previous = entity;
        entity->next = entity_manager.used_list;
        entity_manager.used_list = entity;
        entity->previous = NULL;
    } else {
        entity_manager.used_list = entity;
        entity->next = NULL;
        entity->previous = NULL;
    }

    entity->_inuse = 1;

    return entity;
};

void holoEntityFree(holo_entity* entity){

    // Remove from in use list
    if(entity->previous != NULL){
        entity->previous->next = entity->next;

    }

    if(entity->next != NULL){
        entity->next->previous = entity->previous;
    }

    // If we are the head of the used list, update the head
    if(entity == entity_manager.used_list){
        entity_manager.used_list = entity->next;
    }

    if(entity->free != NULL){
        entity->free(entity);
    }

    if(entity->config != NULL){
        ini_free(entity->config);
    }

    // Free sprite resource
    if(entity->billboard) spicePointSpriteFree(entity->billboard);
    if(entity->mesh) spiceMeshFree(entity->mesh);
    if(entity->sprite) spiceFreeSprite(entity->sprite);

    // Clear entity
    memset(entity, 0, sizeof(holo_entity));

    // Add to front of free list
    entity->previous = NULL;
    entity->next = entity_manager.free_list;
    entity_manager.free_list->previous = entity;
    entity_manager.free_list = entity;
}

void holoEntityManagerUpdate(){
    holo_entity* entity = entity_manager.used_list;
    while(entity != NULL){
        holo_entity* next = entity->next;
        if(entity->update != NULL){
            entity->update(entity);
            vec3_add(entity->position, entity->velocity, &entity->position);
        }
        entity = next;
    }
}

void holoEntityManagerThink(){
    holo_entity* entity = entity_manager.used_list;
    while(entity != NULL){
        holo_entity* next = entity->next;
        if(entity->think != NULL){
            entity->think(entity);
        }
        entity = next;
    }
}


void holoEntityManagerClick(tm_vec3 camera_position, tm_vec3 ray_dir, CursorMode mode, int* hit){
    holo_entity* entity = entity_manager.used_list;
    while(entity != NULL){
        holo_entity* next = entity->next;

        // What? Why?
        tm_vec3 pos = {entity->position.x, entity->position.y, -entity->position.z};
        if(spiceSphereCollideRay(camera_position, ray_dir, pos, entity->radius) == SP_SUCCESS){
            switch (mode){
            case LOOK:
                if(entity->inspect) entity->inspect(entity);
                break;
            case HAND:
                if(entity->interact) entity->interact(entity);
                break;                
            case TALK:
                if(entity->talk) entity->talk(entity);
                break;
            
            default:
                break;
            }
            *hit = 1;
        }

        entity = next;
    }
}

void holoEntityManagerPrintState(){
    printf("[ENT]: [");
    for(size_t i = 0; i < 20; i++){
        if(entity_manager.entities[i]._inuse == 1){
            printf("#");
        } else {
            printf(" ");
        }
    }
    printf("]\n");
}

void holoEntityManagerDraw(){
    holo_entity* entity = entity_manager.used_list;
    while(entity != NULL){
        if(entity->sprite != NULL){
            spiceDrawSprite(entity->sprite, entity->position.x, entity->position.y, 0, 0, entity->frame);
        }
        entity = entity->next;
    }   
}
