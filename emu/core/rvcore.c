#include "core/core.h"
#include "debug.h"
#include "riscv.h"

void rvcore_ready(struct RiscvCore *core) {
    core->inst = 0;
    core->rd = core->rd_val = 0;
    core->exce_val          = 0;
}

void rvcore_commit(struct RiscvCore *core, enum exception exce) {
    if (exce != CAUSE_NONE) {
        rvcore_trap_handle(core, exce);
    } else {
        core->regs[core->rd] = core->rd_val;
        core->pc             = core->next_pc;
        core->regs[0]        = 0;
    }
}

void rvcore_step(struct RiscvCore *core, struct RiscvEnvInfo envinfo) {
    rvcore_ready(core);
    enum exception exec = CAUSE_NONE;

    exec = rvcore_env_update(core, envinfo);
    if (exec != CAUSE_NONE)
        goto commit;
    exec = rvcore_mmu_fetch(core);
    if (exec != CAUSE_NONE)
        goto commit;
    exec = rvcore_exec(core);
    if (exec != CAUSE_NONE)
        goto commit;

commit:
    rvcore_commit(core, exec);
}

void rvcore_init(struct RiscvCore *core, struct DeviceFunc device_func) {
    for (int i = 0; i < sizeof(struct RiscvCore); i++)
        *((u8 *)core + i) = 0;
    core->pc               = 0x00001000;
    core->mode             = MACHINE;
    core->reservation_addr = -1;
    core->wfi              = false;
    core->halt             = false;
    core->device_func      = device_func;
}

bool check_implemented(enum extension ext) {
    switch (ext) {
    case I:
    case A:
        return true;
    default:
        return false;
    }
}
