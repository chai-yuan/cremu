#include "core/core.h"
#include "debug.h"
#include "riscv.h"

void rvcore_trap_handle(struct RiscvCore *core, enum exception cause) {
    if (cause == CAUSE_BREAKPOINT) {
        core->halt = true;
    } else {
        ERROR("unknow exception pc 0x%x : 0x%x , 0x%x", core->pc, cause, core->exce_val);
    }
}
