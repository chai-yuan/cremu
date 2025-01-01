#ifndef RV_CORE_64_H
#define RV_CORE_64_H

#include "core/rvcore.h"
#include "device/device.h"
#include "types.h"

struct RiscvCore64 {
    u64               regs[32];
    u64               pc;
    u64               csrs[4096];
    enum mode         mode;
    bool              reservation_valid;
    u64               reservation_addr;
    bool              halt;
    struct DeviceFunc device_func;
};

void riscvcore64_init(struct RiscvCore64 *core, struct DeviceFunc device_func);

void riscvcore64_step(struct RiscvCore64 *core);

#endif
