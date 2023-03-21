#include <spice_input.h>
#include "player.h"
#include "ui/textbox.h"

static sp_input *left, *right, *forward, *back, *cursor;
static struct nk_context* ctx;

void ghPlayerThink(holo_entity* self){
    ghPlayerData* data = (ghPlayerData*)self->data;
    if(left->state == SP_INPUT_HELD){
        self->velocity.x = min(1.0f, self->velocity.x + 0.05f);
        self->billboard->flip = 1;
    } else if(right->state == SP_INPUT_HELD){
        self->velocity.x = max(-1.0f, self->velocity.x - 0.05f);
        self->billboard->flip = 0;
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

    if(self->position.x + self->velocity.x > 19){
        self->velocity.x = 0;
    } else if(self->position.x + self->velocity.x < -19){
        self->velocity.x = 0;
    }

    if(self->position.z + self->velocity.z > 15){
        self->velocity.z = 0;
    } else if(self->position.z + self->velocity.z < -15){
        self->velocity.z = 0;
    }

    if(cursor->state == SP_INPUT_PRESSED){
        data->cursor_mode = (data->cursor_mode + 1) % 3;
    }

    if(data->cursor_mode == 0){
        nk_style_set_cursor(ctx, NK_CURSOR_ARROW);
    } else if (data->cursor_mode == 1){
        nk_style_set_cursor(ctx, NK_CURSOR_MOVE);
    } else {
        nk_style_set_cursor(ctx, NK_CURSOR_RESIZE_HORIZONTAL);
    }

}

void ghPlayerUpdate(holo_entity* self){
    ghPlayerData* data = (ghPlayerData*)self->data;

    self->billboard->position = self->position;
    self->billboard->position.y = sin(data->y_wave) * 0.1f;
    data->y_wave += 0.05f;
}

void ghPlayerInspect(holo_entity* self){
    if(!ghTextBoxGetEnabled()){
        ghTextBoxSetColor(0, 255, 0);
        ghTextBoxSetText("Looking good Jones.. Heh... what was I doing again?");
        ghTextBoxShow();
    } else {
        ghTextBoxHide();
    }    
}

void ghPlayerTalk(holo_entity* self){
    if(!ghTextBoxGetEnabled()){
        ghTextBoxSetColor(0, 255, 0);
        ghTextBoxSetText("Why would I talk to myself?");
        ghTextBoxShow();
    } else {
        ghTextBoxHide();
    }
}


holo_entity* ghPlayerSpawn(){
    holo_entity* self = holoNewEntity();

    ctx = spiceGetNuklearContext();

    forward = spiceInputGetCmd("fw");
    back = spiceInputGetCmd("bk");

    left = spiceInputGetCmd("left");
    right = spiceInputGetCmd("right");
    
    cursor = spiceInputGetCmd("cursor");

    self->radius = 0.5f;

    self->billboard = spicePointSpriteNew();
    self->billboard->sprite_size = 2046;

    self->think = ghPlayerThink;
    self->update = ghPlayerUpdate;
    self->talk = ghPlayerTalk;
    self->inspect = ghPlayerInspect;

    ghPlayerData* data = (ghPlayerData*)self->data;
    data->y_wave = rand() % 100;

    return self;
}