#include <spice_graphics.h>
#include <spice_gamestate.h>
#include <spice_sprite.h>
#include <spice_mixer.h>
#include <spice_cam.h>

#include "titlescreen.h"
#include "gamestates.h"
#include "game.h"

#include "globals.h"

static struct nk_context* ctx = NULL;
static sp_texture* titlecard = NULL;
static sp_clip* titlescreenMusic = NULL;
static int init = 0, fadeout = 0;

void ghTitlescreenEnter(){
    if(ctx == NULL) ctx = spiceGetNuklearContext();
	if(titlecard == NULL) titlecard = spiceTexture2DLoad("assets/ui/titlecard.png");
	if(titlescreenMusic == NULL) titlescreenMusic = spiceMixerLoadWav("assets/titlescreen.wav");

	titlescreenMusic->loop = 1;
	titlescreenMusic->playing = 1;
	titlescreenMusic->volume = 0.0f;
	titlescreenMusic->fadein = 1;
	titlescreenMusic->fadespeed = 0.03f;
}

void ghTitlescreenExit(){
}



void ghTitlescreenDraw(){

	spiceOrbitCamUpdate();
	spiceGetOrbitCam()->horizontal_angle += 0.0005f;

	nk_style_push_style_item(ctx, &ctx->style.window.fixed_background, nk_style_item_color(nk_rgba(0,0,0,100)));
	nk_style_push_color(ctx, &ctx->style.window.background, nk_rgba(0,0,0,0));
	nk_style_push_vec2(ctx, &ctx->style.window.padding, nk_vec2(25,25));

	nk_begin(ctx, "start_menu_titlecard", nk_rect(335, 50, 640,205), NK_WINDOW_BACKGROUND | NK_WINDOW_NO_SCROLLBAR);
	nk_layout_row_dynamic(ctx, 180, 1);
	nk_image(ctx, nk_image_id(titlecard->texture_id));
	nk_end(ctx);

	nk_begin(ctx, "start_menu", nk_rect(480, 300, 350, 200), NK_WINDOW_BACKGROUND);
	nk_layout_row_dynamic(ctx, 32, 1);
	if(nk_button_label(ctx, "Start")){
		spiceGamestateChange(GH_GAMESTATE_MAIN, 1);
		titlescreenMusic->fadein = 0;
		titlescreenMusic->fadeout = 1;
	}
		
    if(nk_button_label(ctx, "Quit")){
		quit = 1;
	}

	nk_end(ctx);

	nk_style_pop_vec2(ctx);
	nk_style_pop_color(ctx);
	nk_style_pop_style_item(ctx);

}

void ghTitlescreeUpdate(){
    nk_input_begin(ctx);

    spiceInputUpdate();

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if(e.type == SDL_MOUSEBUTTONDOWN){
            clickSfx->playing = 1;
		}
        nk_sdl_handle_event(&e);

    }
    nk_input_end(ctx);
}