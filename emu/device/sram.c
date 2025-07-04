#include "device/sram.h"

void sram_init(struct Sram *sram, u8 *data, u32 len) {
    sram->data = data;
    sram->len  = len;
}

static bool sram_read(void *context, u64 address, u8 size, u64 *data) {
    struct Sram *sram = (struct Sram *)context;
    if (sram->len < address + size)
        return false;

    device_read(sram->data, address, size, data);
    return true;
}

static bool sram_write(void *context, u64 address, u8 size, u64 data) {
    struct Sram *sram = (struct Sram *)context;

    if (sram->len < address + size)
        return false;

    device_write(sram->data, address, size, data);
    return true;
}

struct DeviceFunc sram_get_func(struct Sram *sram) {
    return (struct DeviceFunc){
        .context = sram,
        .read    = sram_read,
        .write   = sram_write,
    };
}
