#ifndef __GH_TEXTBOX__
#define __GH_TEXTBOX__
#include <spice_graphics.h>

typedef struct {
    uint8_t enabled;
    sp_str text;

    struct nk_color color;

    struct nk_context* ctx;
} ghTextBoxManager;

void ghTextBoxShow();
void ghTextBoxHide();
int ghTextBoxGetEnabled();

void ghTextBoxSetColor(uint8_t r, uint8_t g, uint8_t b);
void ghTextBoxSetText(char* text);
void ghTextBoxSetImage();

void ghTextBoxDraw();

#endif