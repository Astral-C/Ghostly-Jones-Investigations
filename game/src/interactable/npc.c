#include "interactable/npc.h"
#include "flag_manager.h"
#include "ui/textbox.h"

static int counter = 0;

void ghNPCBasicUpdate(holo_entity* self){
    ghNPCData* data = (ghNPCData*)self->data;

    self->position.y = sin(data->y_wave) * 0.1f;
    self->billboard->position = self->position;
    data->y_wave += 0.05f;
}

void ghNPCInspect(holo_entity* self){
    ghTextBoxSetColor(255, 255, 255);
    const char* text = ini_get(self->config, "settings", "inspect");
    ghTextBoxSetText(text);
    ghTextBoxShow();
}

void ghNPCTalk(holo_entity* self){
    ghTextBoxSetColor(255, 255, 255);
    ghNPCData* data = (ghNPCData*)self->data;

    const char* next = ini_get(self->config, data->current_dialog, "next");
    const char* close = ini_get(self->config, data->current_dialog, "close");
    const char* check_flag = ini_get(self->config, data->current_dialog, "check_flag");

    if(counter == 1){
        ghTextBoxHide();
        counter = 0;
        return;
    }

    if(close != NULL && atoi(close) == 0 && counter == 0){
        ghTextBoxSetText(ini_get(self->config, data->current_dialog, "text"));
        if(next != NULL){
            data->current_dialog = next;
        }
        if(check_flag != NULL){
            if(ghGetFlag(atoi(check_flag))){
                data->current_dialog = ini_get(self->config, data->current_dialog, "if_flag");
            } else {
                data->current_dialog = ini_get(self->config, data->current_dialog, "if_not_flag");
            }
        }
        ghTextBoxShow();
    } else if(close != NULL && atoi(close) == 1 && counter == 0) {
        ghTextBoxSetText(ini_get(self->config, data->current_dialog, "text"));
        if(next != NULL){
            data->current_dialog = next;
        }
        if(check_flag != NULL){
            if(ghGetFlag(atoi(check_flag))){
                data->current_dialog = ini_get(self->config, data->current_dialog, "if_flag");
            } else {
                data->current_dialog = ini_get(self->config, data->current_dialog, "if_not_flag");
            }
        }
        counter = 1;
        ghTextBoxShow();  
    }
}

void ghNPCCleanup(holo_entity* self){
    ini_free(self->config);
    self->config = NULL;
}

void ghNPCSpawn(char* config_path){
    holo_entity* self = holoNewEntity();
    
    self->radius = 1.0f;

    self->billboard = spicePointSpriteNew();
    self->billboard->sprite_size = 2046;
    self->billboard->texture = 1;
    self->position = (sp_vec3){0, 0, 0};

    self->update = ghNPCBasicUpdate;
    self->talk = ghNPCTalk;
    self->inspect = ghNPCInspect;

    ghNPCData* data = (ghNPCData*)self->data;
    data->y_wave = rand() % 100;

    self->config = ini_load(config_path);
    data->current_dialog = ini_get(self->config, "settings", "dialog");
    ini_sget(self->config, "settings", "pos_x", "%f", &self->position.x);
    ini_sget(self->config, "settings", "pos_y", "%f", &self->position.y);
    ini_sget(self->config, "settings", "pos_z", "%f", &self->position.z);
    ini_sget(self->config, "settings", "type", "%d", &self->billboard->texture);
}