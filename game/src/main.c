#include <spice_graphics.h>
#include <spice_gamestate.h>
#include <spice_input.h>
#include <spice_skybox.h>
#include <spice_mixer.h>
#include <time.h>
#include "player.h"
#include "interactable/npc.h"
#include "game.h"
#include "titlescreen.h"
#include "gamestates.h"
#include "globals.h"

sp_clip* clickSfx;

void transition_fade(uint64_t cur_time, uint64_t total_time, sp_gamestate* previous, sp_gamestate* next){
	struct nk_context* ctx = spiceGetNuklearContext();

    if(cur_time <= 255){
        nk_style_push_color(ctx, &ctx->style.window.background, nk_rgba(0,0,0,cur_time));
        nk_style_push_style_item(ctx, &ctx->style.window.fixed_background, nk_style_item_color(nk_rgba(0,0,0,cur_time)));
    } else {
        nk_style_push_color(ctx, &ctx->style.window.background, nk_rgba(0,0,0,512-(cur_time)));
        nk_style_push_style_item(ctx, &ctx->style.window.fixed_background, nk_style_item_color(nk_rgba(0,0,0,512-cur_time)));
    }

	nk_style_push_vec2(ctx, &ctx->style.window.padding, nk_vec2(0,0));
	nk_begin(ctx, "fade_bg", nk_rect(0, 0, 1280, 720), NK_WINDOW_NOT_INTERACTIVE);
	nk_end(ctx);
	nk_style_pop_vec2(ctx);
	nk_style_pop_style_item(ctx);
	nk_style_pop_color(ctx);

    next->enter();
    if(cur_time <= 255){
        previous->update();
        previous->draw();
    } else if(cur_time < 512) {
        next->update();
        next->draw();
    }
    previous->exit();

}

int main(int argc, char* argv[]){
    srand(time(NULL));

    spiceGraphicsInit("Ghostly Jones Investigations", 1280, 720, 120, 0);
    spiceInputInit("assets/input.cfg");

    spiceMixerInit(10);

    clickSfx = spiceMixerLoadWav("assets/click.wav");
    clickSfx->volume = 1.0f;
    clickSfx->loop = 0;
    clickSfx->playing = 0;

    SDL_ShowCursor(0);

    spiceTextureManagerInit(120);

    spiceMeshManagerInit(20);
    spicePointSpritesInit(20, 10, 256);
    //spiceSpriteInit(50);

    spiceGamestateInit(2);
    spiceGamestateRegister(GH_GAMESTATE_TITLESCREEN, ghTitlescreeUpdate, ghTitlescreenDraw, ghTitlescreenEnter, ghTitlescreenExit);
    spiceGamestateRegister(GH_GAMESTATE_MAIN, ghMainGameUpdate, ghMainGameDraw, ghMainGameEnter, ghMainGameExit);
    spiceGamestateChange(GH_GAMESTATE_TITLESCREEN, 0);

    spiceGamestateSetTransitionFunc(transition_fade);
    spiceGamestateSetTransitionSpeed(4);
    spiceGamestateSetTransitionTime(512);

    spiceOrbitCamInit(45.0f, 1280, 720, 0.01f, 10000.0f);
    spiceGetOrbitCam()->distance = 10.0f;

    sp_skybox* skybox = spiceSkyboxNew();
    skybox->texture = spiceTextureLoadCubeMap((char*[6]){
        "assets/skybox/left.png",
        "assets/skybox/right.png",
        "assets/skybox/top.png",
        "assets/skybox/bot.png",
        "assets/skybox/front.png",
        "assets/skybox/back.png"
    });

    struct nk_context* ctx = spiceGetNuklearContext();
    sp_texture* hand = spiceTexture2DLoad("assets/ui/hand.png");
    sp_texture* eye = spiceTexture2DLoad("assets/ui/eye.png");
    sp_texture* talk = spiceTexture2DLoad("assets/ui/talk.png");

    struct nk_cursor handcursor = (struct nk_cursor){nk_image_id(hand->texture_id), nk_vec2(64,64), nk_vec2(0,0)};
    struct nk_cursor eyecursor = (struct nk_cursor){nk_image_id(eye->texture_id), nk_vec2(64,64), nk_vec2(0,0)};
    struct nk_cursor talkcursor = (struct nk_cursor){nk_image_id(talk->texture_id), nk_vec2(64,64), nk_vec2(0,0)};

    struct nk_font_atlas* atlas;
    nk_sdl_font_stash_begin(&atlas);
    nk_sdl_font_stash_end();

    nk_style_load_all_cursors(ctx, atlas->cursors);
    nk_style_load_cursor(ctx, NK_CURSOR_ARROW, &handcursor);
    nk_style_load_cursor(ctx, NK_CURSOR_MOVE, &eyecursor);
    nk_style_load_cursor(ctx, NK_CURSOR_RESIZE_HORIZONTAL, &talkcursor);

    spiceMixerPause(0);

    quit = 0;
    while(!quit){

        spiceSkyboxRender(skybox);

        spiceGamestateUpdate();

        nk_sdl_render(NK_ANTI_ALIASING_ON, 512 * 1024, 512 * 1024);

        spiceGraphicsDraw();
        spiceGraphicsStep();
    }

    return 0;
}