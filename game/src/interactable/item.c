#include "interactable/item.h"
#include "clue_manager.h"
#include "flag_manager.h"
#include "ui/textbox.h"

void ghItemUpdate(holo_entity* self){
    self->billboard->position = self->position;
}

void ghItemInspect(holo_entity* self){
    ghTextBoxSetColor(255, 255, 255);
    const char* text = ini_get(self->config, "settings", "inspect");
    ghTextBoxSetText(text);
    ghTextBoxShow();
}

void ghItemCollect(holo_entity* self){
    const char* set_flag = ini_get(self->config, "settings", "set_flag");
    const char* give_clue = ini_get(self->config, "clue", "id");
    const char* clue_image = ini_get(self->config, "clue", "img");
    const char* clue_found_text = ini_get(self->config, "clue", "text");

    if(set_flag != NULL){
        ghSetFlag(atoi(set_flag), 1);
    }

    if(give_clue != NULL && clue_image != NULL & clue_found_text != NULL){
        ghClueManagerNotifyAddClue(clue_image);
        ghTextBoxSetColor(0,255,0);
        ghTextBoxSetText(clue_found_text);
        ghTextBoxShow();
    }

    holoEntityFree(self);
}

void ghItemCleanup(holo_entity* self){

}

void ghItemSpawn(char* config_path){
    holo_entity* self = holoNewEntity();
    
    self->radius = 1.5f;

    self->billboard = spicePointSpriteNew();
    self->billboard->sprite_size = 2046;
    self->billboard->texture = 2;
    //TODO: spawn at position function?
    self->position = (sp_vec3){0,0,0};

    self->update = ghItemUpdate;
    self->interact = ghItemCollect;
    self->inspect = ghItemInspect;
    self->free = ghItemCleanup;

    self->config = ini_load(config_path);
    ini_sget(self->config, "settings", "pos_x", "%f", &self->position.x);
    ini_sget(self->config, "settings", "pos_y", "%f", &self->position.y);
    ini_sget(self->config, "settings", "pos_z", "%f", &self->position.z);
    ini_sget(self->config, "clue", "sprite", "%d", &self->billboard->texture);
    //data->current_dialog = ini_get(self->config, "settings", "dialog");
}