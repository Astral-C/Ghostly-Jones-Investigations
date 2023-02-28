#include <spice_graphics.h>
#include <spice_skybox.h>
#include <spice_mesh.h>
#include <spice_sprite.h>
#include <spice_input.h>

int main(int argc, char* argv[]){
    spiceGraphicsInit("Ghostly Jones Investigations", 1280, 720, 120, 0);
    spiceInputInit("assets/input.cfg");

    spiceOrbitCamInit(90.0f, 1280, 720, 0.01f, 10000.0f);
    tm_orbit_camera* camera = spiceGetOrbitCam();

    spiceTextureManagerInit(120);
    spiceSpriteInit(15);
    spiceMeshManagerInit(5);
    spicePointSpritesInit(10, 2, 256);

    struct nk_context* ctx = spiceGetNuklearContext();

    sp_input* exit = spiceInputGetCmd("quit");


    int quit = 0;

    sp_skybox* skybox = spiceSkyboxNew();
    skybox->texture = spiceTextureLoadCubeMap((char*[6]){
        "assets/skybox/right.png",
        "assets/skybox/left.png",
        "assets/skybox/top.png",
        "assets/skybox/bot.png",
        "assets/skybox/front.png",
        "assets/skybox/back.png"
    });


    camera->distance = 10.0f;

    while(!quit){
        spiceOrbitCamUpdate();

        SDL_PumpEvents();

        //SDL_Event e;
        //while(SDL_PollEvent(&e)){}
        if(exit->state == SP_INPUT_PRESSED){
            quit = 1;
        }


        nk_input_begin(ctx);

        spiceInputUpdate();

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            nk_sdl_handle_event(&e);
        }

        nk_input_end(ctx);
        
        spiceSkyboxRender(skybox);

        spiceMeshManagerDraw();
        spicePointSpriteDraw();

        nk_sdl_render(NK_ANTI_ALIASING_ON, 512 * 1024, 512 * 1024);

        spiceGraphicsDraw();
        spiceGraphicsStep();
    }
    
    spiceSkyboxFree(skybox);

    return 0;
}