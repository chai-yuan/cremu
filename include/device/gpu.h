#ifndef GPU_H
#define GPU_H

#include "device/device.h"

#define WIDTH 640
#define HEIGHT 480
#define GPU_SIZE (WIDTH * HEIGHT * 4)

typedef void (*update_framebuffer_func_t)(u32 *data, int width, int height);

struct GPU {
    u32                       vram[WIDTH * HEIGHT];
    u64                       last_update;
    update_framebuffer_func_t update_frame;
};

void gpu_init(struct GPU *gpu, update_framebuffer_func_t update_frame);

struct DeviceFunc gpu_get_func(struct GPU *gpu);

#endif
