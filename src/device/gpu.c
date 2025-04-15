#include "device/gpu.h"
#include "debug.h"

void gpu_init(struct GPU *gpu, update_framebuffer_func_t update_frame) {
    gpu->last_update  = 0;
    gpu->update_frame = update_frame;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            u8 r                     = x * 255 / WIDTH;
            u8 g                     = y * 255 / HEIGHT;
            u8 b                     = 128;
            u8 a                     = 255;
            gpu->vram[y * WIDTH + x] = (a << 24) | (r << 16) | (g << 8) | b;
        }
    }
}

static enum exception gpu_read(void *context, u64 addr, u8 size, usize *data) {
    struct GPU *gpu = (struct GPU *)context;
    if (GPU_SIZE < addr + size)
        return LOAD_ACCESS_FAULT;

    device_read((u8 *)gpu->vram, addr, size, data);

    return EXC_NONE;
}

static enum exception gpu_write(void *context, u64 addr, u8 size, usize data) {
    struct GPU *gpu = (struct GPU *)context;
    if (GPU_SIZE < addr + size)
        return STORE_AMO_ACCESS_FAULT;

    device_write((u8 *)gpu->vram, addr, size, data);

    return EXC_NONE;
}

static void gpu_update(void *context, u32 interval) {
    struct GPU *gpu = (struct GPU *)context;

    gpu->last_update += interval;
    if (!gpu->update_frame || gpu->last_update < 2000000)
        return;
    gpu->last_update = 0;

    gpu->update_frame(gpu->vram, WIDTH, HEIGHT);
}

struct DeviceFunc gpu_get_func(struct GPU *gpu) {
    return (struct DeviceFunc){
        .context = gpu,
        .read    = gpu_read,
        .write   = gpu_write,
        .update  = gpu_update,
    };
}
