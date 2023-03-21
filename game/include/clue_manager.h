#ifndef __GH_CLUEMANAGER_H__
#define __GH_CLUEMANAGER_H__

#include <stdint.h>
#include <spice_util.h>
#include <spice_sprite.h>
#include <spice_graphics.h>
#include <spice_texture.h>

typedef struct CLUE_S {
    uint8_t clue_id;
    sp_str text;
    sp_texture* clue_img;
} ghClue;

typedef struct {
    int clue_count;
    ghClue clues[50];
} ghClueManager;

void ghClueManagerDraw();

uint16_t ghClueManagerNotifyAddClue(char* img_path);
void ghClueManagerNotifyRemoveClue();

#endif