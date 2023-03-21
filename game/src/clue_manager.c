#include "clue_manager.h"

static ghClueManager clue_manager = {0};

uint16_t ghClueManagerNotifyAddClue(char* img_path){
    if(clue_manager.clue_count >= 50) return -1;
    
    clue_manager.clues[clue_manager.clue_count].clue_id = 1; //hack
    clue_manager.clues[clue_manager.clue_count].clue_img = spiceTexture2DLoad(img_path);
    clue_manager.clue_count++;
    return clue_manager.clue_count;
}

void ghClueManagerNotifyRemoveClue(){

}

void ghClueManagerDraw(){
    struct nk_context* ctx = spiceGetNuklearContext();

    nk_style_push_style_item(ctx, &ctx->style.button.normal, nk_style_item_color(nk_rgba(0,0,0,0)));
    nk_style_push_style_item(ctx, &ctx->style.button.hover, nk_style_item_color(nk_rgba(0,0,0,0)));
    nk_style_push_style_item(ctx, &ctx->style.button.active, nk_style_item_color(nk_rgba(0,0,0,0)));
    
    nk_style_push_float(ctx, &ctx->style.button.border, 0.0f);

    nk_layout_row_dynamic(ctx, 200, 5);

    for (size_t i = 0; i < clue_manager.clue_count; i++){
        if(nk_button_image(ctx, nk_image_id(clue_manager.clues[i].clue_img->texture_id))){
            spice_info("Clicked on clue %d\n", clue_manager.clues[i].clue_id);
        }
    }
    
    nk_style_pop_float(ctx);
    nk_style_pop_style_item(ctx);
    nk_style_pop_style_item(ctx);
    nk_style_pop_style_item(ctx);
}