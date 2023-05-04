#include "game.h"

#include <spice_skybox.h>
#include <spice_mesh.h>
#include <spice_mixer.h>
#include <spice_archive.h>
#include <spice_sprite.h>
#include <spice_input.h>
#include <spice_gamestate.h>
#include <holo_entity.h>

#include "player.h"
#include "interactable/npc.h"
#include "interactable/item.h"
#include "clue_manager.h"
#include "ui/textbox.h"

#include "globals.h"


//todo wrap this all into a struct
static int GameInit = 0;
static holo_entity* player;
static tm_orbit_camera* camera;
static struct nk_context* ctx;
static sp_input* pause_btn = NULL;
static int is_paused = 0;
static int current_pause_menu = 0, fadeout = 0;

static prkArchive* character_sprites = NULL;

static sp_mesh* office = NULL;

static sp_texture *board, *book;
static sp_clip* sceneMusic = NULL;

int quit = 0;

void ghMainGameCleanup(){
    prkArchiveClose(character_sprites);
}

void ghMainGameEnter(){
    if(!GameInit){
        ghMainGameInit();
        GameInit = 1;
    }
    if(sceneMusic != NULL){
        sceneMusic->loop = 1;
        sceneMusic->playing = 1;
        sceneMusic->fadein = 1;
        sceneMusic->fadespeed = 0.03f;
        sceneMusic->volume = 0.0f;
    }
}

void ghMainGameExit(){
}

void ghMainGameInit(){
    camera = spiceGetOrbitCam();
    camera->distance = 10.0f;
    
    holoEntityManagerInit(255);

    ctx = spiceGetNuklearContext();

    office = spiceMeshLoadCinnamodel("assets/environments/JonesOffice.cnmdl");

    character_sprites = prkArchiveOpen("assets/character_sprites.prka");

    if(character_sprites == NULL){
        spice_error("Couldn't load character sprite archive!");
    }

    prkArcFile* jones = prkArchiveGetFile(character_sprites, "jones.png");
    prkArcFile* cop = prkArchiveGetFile(character_sprites, "cop.png");
    prkArcFile* sunhat = prkArchiveGetFile(character_sprites, "sunhat.png");

    spicePointSpriteSetTextureFromMemory(0, jones->data, jones->size);
    spicePointSpriteSetTextureFromMemory(1, cop->data, cop->size);
    spicePointSpriteSetTextureFromMemory(2, sunhat->data, sunhat->size);
    spicePointSpriteSetTexture(3, "assets/items/item.png");
    spicePointSpriteSetTexture(4, "assets/items/camera.png");

    board = spiceTexture2DLoad("assets/ui/board.png");
    book = spiceTexture2DLoad("assets/ui/book.png");

    player = ghPlayerSpawn();
    ini_t* scenario_settings = ini_load("assets/scenarios.ini");
    ini_t* scenario = ini_load(ini_get(scenario_settings, "settings", "boot"));
    
    if(scenario == NULL){
        //Couldn't load?
        exit(1);
    }

    char* npcs = ini_get(scenario, "scenario", "npcs");
    char* items = ini_get(scenario, "scenario", "items");
    char* music = ini_get(scenario, "scenario", "music");

    if(npcs != NULL){
        char* npc_config = strtok(npcs, ",");
        while(npc_config != NULL){
            ghNPCSpawn(npc_config);
            npc_config = strtok(NULL, ",");
        }
    }

    if(items != NULL){
        char* item_config = strtok(items, ",");
        while(item_config != NULL){
            ghItemSpawn(item_config);
            item_config = strtok(NULL, ",");
        }
    }

    if(music != NULL){
        sceneMusic = spiceMixerLoadWav(music);
        sceneMusic->fadespeed = 0.02f;
    }

    ini_free(scenario_settings);
    ini_free(scenario);

    pause_btn = spiceInputGetCmd("pause");

    atexit(ghMainGameCleanup);
}

void ghMainGameUpdate(){
    if(!GameInit){
        ghMainGameInit();
        GameInit = 1;
    }

    spiceOrbitCamUpdate();

    if(pause_btn != NULL && pause_btn->state == SP_INPUT_PRESSED){
        is_paused = !is_paused;
        current_pause_menu = 0;
    }

    camera->target = (tm_vec3){player->position.x, player->position.y, -player->position.z};
    camera->horizontal_angle = 0.0f;

    nk_style_push_style_item(ctx, &ctx->style.window.fixed_background, nk_style_item_color(nk_rgba(0,0,0,0)));
    nk_style_push_style_item(ctx, &ctx->style.button.normal, nk_style_item_color(nk_rgba(0,0,0,0)));
    nk_style_push_style_item(ctx, &ctx->style.button.hover, nk_style_item_color(nk_rgba(0,0,0,0)));
    nk_style_push_style_item(ctx, &ctx->style.button.active, nk_style_item_color(nk_rgba(0,0,0,0)));
    
    nk_style_push_float(ctx, &ctx->style.button.border, 0.0f);
	nk_style_push_vec2(ctx, &ctx->style.window.padding, nk_vec2(0,0));

    nk_begin(ctx, "ui_elements", (struct nk_rect){1280-256, 720-128, 256, 128}, NK_WINDOW_NO_SCROLLBAR);
    nk_layout_row_dynamic(ctx, 128, 2);
    if(nk_button_image(ctx, nk_image_id(board->texture_id))){
        is_paused = !is_paused;
        current_pause_menu = 1;
    }
    nk_button_image(ctx, nk_image_id(book->texture_id));
    nk_end(ctx);

    nk_style_pop_vec2(ctx);
    nk_style_pop_float(ctx);
    nk_style_pop_style_item(ctx);
    nk_style_pop_style_item(ctx);
    nk_style_pop_style_item(ctx);
    nk_style_pop_style_item(ctx);


    if(!is_paused){
        if(!ghTextBoxGetEnabled()){
            holoEntityManagerThink();
            holoEntityManagerUpdate();
        }
    } else {
		nk_style_push_style_item(ctx, &ctx->style.window.fixed_background, nk_style_item_color(nk_rgba(0,0,0,100)));
		nk_style_push_color(ctx, &ctx->style.window.background, nk_rgba(0,0,0,100));
		nk_style_push_vec2(ctx, &ctx->style.window.padding, nk_vec2(0,0));
		nk_begin(ctx, "pm_bg", nk_rect(0, 0, 1280, 720), NK_WINDOW_NOT_INTERACTIVE);
		nk_end(ctx);
		nk_style_pop_vec2(ctx);
		
		nk_style_push_vec2(ctx, &ctx->style.window.padding, nk_vec2(50,25));
		
		switch (current_pause_menu)
		{
		case 0:
			nk_begin(ctx, "pause_menu", nk_rect(450, 230, 375, 260), 0);
			nk_layout_row_static(ctx, 32, 260, 1);
			if(nk_button_label(ctx, "Continue")){
				is_paused = 0;
			}
			nk_layout_row_static(ctx, 32, 260, 1);
			if(nk_button_label(ctx, "Quit")){
				spiceGamestateChange(0, 1);
                sceneMusic->fadein = 0;
                sceneMusic->fadeout = 1;
			}
			nk_end(ctx);

			break;

        case 1:
            nk_style_push_style_item(ctx, &ctx->style.window.fixed_background, nk_style_item_color(nk_rgb(70,31,0)));
            nk_style_push_color(ctx, &ctx->style.window.border_color, nk_rgb(70, 31, 0));
            nk_style_push_float(ctx, &ctx->style.window.border, 3.0f);

            nk_begin(ctx, "corkboard_ui", (struct nk_rect){(1280/2)-500, (720/2)-250, 250*4, 250*2}, NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER);

            ghClueManagerDraw();

            nk_end(ctx);

            nk_style_pop_float(ctx);
            nk_style_pop_color(ctx);
            nk_style_pop_style_item(ctx);
            break;

		default:
			nk_begin(ctx, "default_menu", nk_rect(450, 260, 375, 200), 0);
			nk_layout_row_static(ctx, 32, 260, 1);
			if(nk_button_label(ctx, "Back")){
				current_pause_menu = 0;	
			}
			nk_end(ctx);
			break;
		}
		
		nk_style_pop_vec2(ctx);
		nk_style_pop_color(ctx);
		nk_style_pop_style_item(ctx);

    }

    SDL_PumpEvents();

    nk_input_begin(ctx);
    spiceInputUpdate();

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        nk_sdl_handle_event(&e);

        if(e.type == SDL_QUIT){
            quit = 1;
        }

        if(e.type == SDL_MOUSEBUTTONDOWN){
            clickSfx->playing = 1;

            int mx, my;
            tm_vec3 ray_dir;
            SDL_GetMouseState(&mx, &my);
            spiceOrbitCamRaycast(mx, my, 1280, 720, &ray_dir);

            int hit = 0;
            holoEntityManagerClick(camera->position, ray_dir, player->data[0], &hit);

            if(hit == 0 && ghTextBoxGetEnabled()){
                ghTextBoxHide();
            }

        }

    }
    nk_input_end(ctx);

}

void ghMainGameDraw(){
    ghTextBoxDraw();

    spiceMeshManagerDraw();
    spicePointSpriteDraw();
}


