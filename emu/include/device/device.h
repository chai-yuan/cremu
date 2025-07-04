#ifndef DEVICE_H
#define DEVICE_H

#include "debug.h"
#include "types.h"

// 设备iommu访问接口
typedef bool (*device_read_func_t)(void *context, u64 address, u8 size, u64 *data);
typedef bool (*device_write_func_t)(void *context, u64 address, u8 size, u64 data);
struct DeviceFunc {
    void               *context;
    device_read_func_t  read;
    device_write_func_t write;
};

static inline void device_read(u8 *buf, u64 address, u8 size, u64 *data) {
    *data = 0;
    switch (size) {
    case 1:
        *data = REG8(buf, address);
        break;
    case 2:
        *data = REG16(buf, address);
        break;
    case 4:
        *data = REG32(buf, address);
        break;
    case 8:
        *data = REG64(buf, address);
        break;
    default:
        ERROR("error size %d", size);
    }
}

static inline void device_write(u8 *buf, u64 address, u8 size, u64 data) {
    switch (size) {
    case 1:
        REG8(buf, address) = data;
        break;
    case 2:
        REG16(buf, address) = data;
        break;
    case 4:
        REG32(buf, address) = data;
        break;
    case 8:
        REG64(buf, address) = data;
        break;
    default:
        ERROR("error size %d", size);
    }
}

#endif
