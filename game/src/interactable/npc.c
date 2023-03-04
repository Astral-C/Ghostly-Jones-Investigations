#include "interactable/npc.h"
#include "ui/textbox.h"

void ghNPCBasicUpdate(holo_entity* self){
    ghGhostCop* data = (ghGhostCop*)self->data;

    self->position.y = sin(data->y_wave) * 0.1f;
    self->billboard->position = self->position;
    data->y_wave += 0.05f;
}

void ghNPCTalk(holo_entity* self){
    ghTextBoxSetText("This is a test");
    ghTextBoxShow();
}


void ghGhostCopSpawn(){
    holo_entity* self = holoNewEntity();
    
    self->billboard = spicePointSpriteNew();
    self->billboard->sprite_size = 2046;
    self->billboard->texture = 1;
    self->position = (sp_vec3){25, 25, -5};
    //self->think = gh;
    self->update = ghNPCBasicUpdate;
    self->talk = ghNPCTalk;
}