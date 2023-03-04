#include <spice_input.h>
#include "player.h"
#include "ui/textbox.h"

static tm_orbit_camera* camera;
static sp_input *left, *right, *forward, *back;

void ghPlayerThink(holo_entity* self){
    if(left->state == SP_INPUT_HELD){
        self->velocity.x = min(1.0f, self->velocity.x + 0.05f);
    } else if(right->state == SP_INPUT_HELD){
        self->velocity.x = max(-1.0f, self->velocity.x - 0.05f);
    } else {
        if(self->velocity.x > 0){
            self->velocity.x = max(self->velocity.x - 0.05f, 0);
        } else if(self->velocity.x < 0){
            self->velocity.x = min(self->velocity.x + 0.05f, 0);
        }
    }

    if(forward->state == SP_INPUT_HELD){
        self->velocity.z = min(1.0f, self->velocity.z + 0.05f);
    } else if(back->state == SP_INPUT_HELD){
        self->velocity.z = max(-1.0f, self->velocity.z - 0.05f);
    } else {
        if(self->velocity.z > 0){
            self->velocity.z = max(self->velocity.z - 0.05f, 0);
        } else if(self->velocity.z < 0){
            self->velocity.z = min(self->velocity.z + 0.05f, 0);
        }
    }

}

void ghPlayerUpdate(holo_entity* self){
    ghPlayerData* data = (ghPlayerData*)self->data;

    self->billboard->position = self->position;
    self->billboard->position.y = sin(data->y_wave) * 0.1f;
    data->y_wave += 0.05f;
}

void ghPlayerTalk(holo_entity* self){
    if(!ghTextBoxGetEnabled()){
        ghTextBoxSetText("Why would I talk to myself?");
        ghTextBoxShow();
    } else {
        ghTextBoxHide();
    }
}


holo_entity* ghPlayerSpawn(){
    holo_entity* self = holoNewEntity();

    camera = spiceGetOrbitCam();
    camera->distance = 10.0f;

    forward = spiceInputGetCmd("fw");
    back = spiceInputGetCmd("bk");

    left = spiceInputGetCmd("left");
    right = spiceInputGetCmd("right");
    
    self->billboard = spicePointSpriteNew();
    self->billboard->sprite_size = 2046;
    self->think = ghPlayerThink;
    self->update = ghPlayerUpdate;
    self->talk = ghPlayerTalk;

    return self;
}