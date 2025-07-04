#ifndef SDL2_H
#define SDL2_H

#include "stdint.h"

void init_sdl(int width, int height);
void update_framebuffer(uint32_t *data, int width, int height);
void cleanup_sdl(void);

#endif
