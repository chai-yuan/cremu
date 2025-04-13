#include "core/rvcore.h"
#include "core.h"
#include "core/riscv.h"
#include "debug.h"

void rvcore_env_update(struct RiscvCore *core, struct RiscvEnvInfo envinfo) {
    core->csrs[MIP] = 0;
    core->csrs[MIP] |= envinfo.meint ? (1 << MACHINE_EXTERNAL_INTERRUPT) : 0;
    core->csrs[MIP] |= envinfo.seint ? (1 << SUPERVISOR_EXTERNAL_INTERRUPT) : 0;
    core->csrs[MIP] |= envinfo.mtint ? (1 << MACHINE_TIMER_INTERRUPT) : 0;
    core->csrs[TIME] = envinfo.time;
}

void rvcore_step(struct RiscvCore *core, struct RiscvEnvInfo envinfo) {
    rvcore_env_update(core, envinfo);
    rvcore_decode_init(&core->decode);

    if (rvcore_interrupt_handle(core)) {
        
    } else if (!core->wfi) { // 如果没有处于休眠
        rvcore_mmu_fetch(core);
        if (core->decode.exception == EXC_NONE)
            rvcore_exec(core);

        if (core->decode.exception != EXC_NONE)
            rvcore_exception_handle(core);
    }

    core->regs[0] = 0;
    core->pc      = core->decode.next_pc;
}

void rvcore_init(struct RiscvCore *core, struct DeviceFunc device_func) {
    for (int i = 0; i < sizeof(struct RiscvCore); i++)
        *((u8 *)core + i) = 0;
    core->pc               = 0x00001000;
    core->mode             = MACHINE;
    core->reservation_addr = -1;
    core->wfi              = false;
    core->device_func      = device_func;

    // csr 初始化
    core->csrs[MARCHID] = 0x5;
    core->csrs[MISA]    = IS_RV64(0x8000000000141105, 0x40141105);
    core->csrs[MSTATUS] = IS_RV64(0xa00000000, 0x0);
}
