#include <nuklear.h>
#include "ui/textbox.h"

static ghTextBoxManager textbox_manager = {0};

void ghTextBoxSetText(char* text){
    memset(textbox_manager.text, 0, sizeof(sp_str));
    strncpy(textbox_manager.text, text, (strlen(text) > sizeof(sp_str) ? sizeof(sp_str) : strlen(text)));
}

void ghTextBoxShow(){
    textbox_manager.enabled = 1;
}

void ghTextBoxHide(){
    textbox_manager.enabled = 0;
}

int ghTextBoxGetEnabled(){
    return textbox_manager.enabled;
}

void ghTextBoxSetImage();

void ghTextBoxDraw(){
    if(textbox_manager.ctx == NULL) textbox_manager.ctx = spiceGetNuklearContext();
    if(textbox_manager.enabled){
        nk_style_push_style_item(textbox_manager.ctx, &textbox_manager.ctx->style.window.fixed_background, nk_style_item_color(nk_rgba(0,0,0,150)));
        nk_style_push_float(textbox_manager.ctx, &textbox_manager.ctx->style.window.border, 3.0f);

        nk_begin(textbox_manager.ctx, "talk_box", (struct nk_rect){20, 720-140, 1280-280, 135}, NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER);
        nk_layout_row_begin(textbox_manager.ctx, NK_STATIC, 128, 2);
        nk_layout_row_push(textbox_manager.ctx, 128);
        //nk_image(ctx, ghost_cop_talk->image);
        nk_layout_row_push(textbox_manager.ctx, 1280-250);

        nk_label_colored_wrap(textbox_manager.ctx, textbox_manager.text, nk_rgb(200, 50, 150));

        nk_end(textbox_manager.ctx);

        nk_style_pop_float(textbox_manager.ctx);
        nk_style_pop_style_item(textbox_manager.ctx);
    }
}