#ifndef DEVICE_H
#define DEVICE_H

#include "core/rvdecode.h"
#include "types.h"

typedef enum exception (*read_func_t)(void *context, u64 address, u8 size, u64 *value);
typedef enum exception (*write_func_t)(void *context, u64 address, u8 size, u64 value);
typedef void (*update_func_t)(void *context, u32 interval);
typedef bool (*check_external_interrupt_func_t)(void *context);
typedef bool (*check_timer_interrupt_func_t)(void *context);

struct DeviceFunc {
    void *context;
    read_func_t read;
    write_func_t write;
    update_func_t update;
    check_external_interrupt_func_t check_external_interrupt;
    check_timer_interrupt_func_t check_timer_interrupt;
};

#endif
