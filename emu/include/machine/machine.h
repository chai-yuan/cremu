#ifndef MACHINE_H
#define MACHINE_H

#include "device/gpu.h"
#include "device/uart16550.h"

enum MachineCode {
    RUNNING,
    GOOD_END,
    BAD_END,
};

struct PortableOperations {
    u8                       *sram_data;
    u64                       sram_size;
    get_char_func_t           get_char;
    put_char_func_t           put_char;
    update_framebuffer_func_t update_framebuffer;
};

typedef void (*step_func_t)(void *machine);
typedef enum MachineCode (*check_func_t)(void *machine);

struct MachineFunc {
    void        *context;
    step_func_t  step;
    check_func_t check;
};

#endif
