#include "flag_manager.h"

// Set flag names to use in code here
typedef enum {
    GH_FLAG_COP_ACCUSED,
    GH_FLAG_COP_BEFRIENDED,
    GH_FLAG_COUNT
} ghFlagNames;

static uint8_t ghFlags[500] = {0};

uint8_t ghGetFlag(uint16_t flag){
    return ghFlags[flag];
}

void ghSetFlag(uint16_t flag, uint8_t value){
    ghFlags[flag] = value;
}