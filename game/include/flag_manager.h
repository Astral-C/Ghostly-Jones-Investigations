#ifndef __GH_FLAGS__
#define __GH_FLAGS__
#include <stdint.h>

uint8_t ghGetFlag(uint16_t flag);
void ghSetFlag(uint16_t flag, uint8_t value);

#endif