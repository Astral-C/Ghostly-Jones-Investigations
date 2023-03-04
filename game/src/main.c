#include <spice_graphics.h>
#include <spice_skybox.h>
#include <spice_mesh.h>
#include <spice_sprite.h>
#include <spice_input.h>
#include <holo_entity.h>
#include "player.h"
#include "interactable/npc.h"
#include "ui/textbox.h"

int main(int argc, char* argv[]){
    spiceGraphicsInit("Ghostly Jones Investigations", 1280, 720, 120, 0);
    spiceInputInit("assets/input.cfg");

    spiceOrbitCamInit(45.0f, 1280, 720, 0.01f, 10000.0f);

    spiceTextureManagerInit(120);
    spiceSpriteInit(15);
    spiceMeshManagerInit(5);
    spicePointSpritesInit(10, 2, 256);
    
    holoEntityManagerInit(200);

    struct nk_context* ctx = spiceGetNuklearContext();

    sp_input* exit = spiceInputGetCmd("quit");

    int quit = 0;

    sp_mesh* office = spiceMeshLoadCinnamodel("assets/environments/JonesOffice.cnmdl");
    office->texture = spiceTexture2DLoad("assets/environments/wood2.bmp");

    sp_skybox* skybox = spiceSkyboxNew();
    skybox->texture = spiceTextureLoadCubeMap((char*[6]){
        "assets/skybox/right.png",
        "assets/skybox/left.png",
        "assets/skybox/top.png",
        "assets/skybox/bot.png",
        "assets/skybox/front.png",
        "assets/skybox/back.png"
    });

    spicePointSpriteSetTexture(0, "assets/characters/jones.png");
    spicePointSpriteSetTexture(1, "assets/characters/cop.png");

    holo_entity* player = ghPlayerSpawn();
    ghGhostCopSpawn();

    while(!quit){
        spiceOrbitCamUpdate();
        spiceGetOrbitCam()->target = (tm_vec3){player->position.x, player->position.y, -player->position.z};

        SDL_PumpEvents();

        //SDL_Event e;
        //while(SDL_PollEvent(&e)){}
        if(exit->state == SP_INPUT_PRESSED){
            quit = 1;
        }

        holoEntityManagerThink();
        holoEntityManagerUpdate();

        nk_input_begin(ctx);

        spiceInputUpdate();

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            nk_sdl_handle_event(&e);

            if(e.type == SDL_MOUSEBUTTONDOWN){
                int mx, my;
                tm_vec3 ray_dir;
                SDL_GetMouseState(&mx, &my);
                spiceOrbitCamRaycast(mx, my, 1280, 720, &ray_dir);

                holoEntityManagerClick(spiceGetOrbitCam()->position, ray_dir, 2);

            }

        }

        nk_input_end(ctx);

        ghTextBoxDraw();
        
        spiceSkyboxRender(skybox);

        spiceMeshManagerDraw();
        spicePointSpriteDraw();

        nk_sdl_render(NK_ANTI_ALIASING_ON, 512 * 1024, 512 * 1024);

        spiceGraphicsDraw();
        spiceGraphicsStep();
    }
    
    spiceSkyboxFree(skybox);
    spiceMeshFree(office);

    return 0;
}