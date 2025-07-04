#include "riscv.h"

enum exception mmu_translate(struct RiscvCore *core, enum exception exc, usize addr, u64 *paddr) {
    *paddr = addr;
    return CAUSE_NONE;
}

enum exception rvcore_mmu_read(struct RiscvCore *core, usize addr, u8 size, usize *data) {
    u64 rdata = 0;
    if (core->device_func.read(core->device_func.context, addr, size, &rdata)) {
        *data = rdata;
        return CAUSE_NONE;
    }
    return CAUSE_LOAD_ACCESS;
}

enum exception rvcore_mmu_write(struct RiscvCore *core, usize addr, u8 size, usize data) {
    if (core->device_func.write(core->device_func.context, addr, size, data)) {
        return CAUSE_NONE;
    }
    return CAUSE_STORE_ACCESS;
}

enum exception rvcore_mmu_fetch(struct RiscvCore *core) {
    u64            inst  = 0;
    u64            paddr = 0;
    enum exception ret   = mmu_translate(core, CAUSE_NONE, core->pc, &paddr);
    if (ret != CAUSE_NONE) {
        return ret;
    }
    core->device_func.read(core->device_func.context, paddr, 4, &inst);

    core->inst = inst;
    return CAUSE_NONE;
}
