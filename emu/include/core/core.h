#ifndef RV_CORE_H
#define RV_CORE_H

#include "device/device.h"
#include "types.h"

enum mode { USER = 0x0, SUPERVISOR = 0x1, MACHINE = 0x3 };

struct RiscvEnvInfo {
    u64  time;
    bool meint;
    bool seint;
    bool mtint;
};

struct RiscvCore {
    // 处理器状态
    usize     regs[32];
    usize     pc;
    usize     csrs[4096];
    enum mode mode;
    u64       reservation_addr;
    bool      wfi;
    bool      halt;
    // 处理器临时状态
    u32   inst;
    usize rd, rd_val;
    usize next_pc;
    usize exce_val;
    // 处理器方法
    struct DeviceFunc device_func;
};

void rvcore_init(struct RiscvCore *core, struct DeviceFunc device_func);
void rvcore_step(struct RiscvCore *core, struct RiscvEnvInfo envinfo);
#endif
