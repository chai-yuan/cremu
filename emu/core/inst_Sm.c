#include "core/core.h"
#include "rvdecode.h"
#include "rvinst.h"

enum exception inst_wfi(struct RiscvCore *core) {
    core->wfi = true;
    // TODO
    return CAUSE_NONE;
}
enum exception inst_mret(struct RiscvCore *core) {
    // TODO
    return CAUSE_NONE;
}

struct Instruction instSm[] = {
    {.mask = 0xffffffff, .match = 0x10500073, .func = inst_wfi},
    {.mask = 0xffffffff, .match = 0x30200073, .func = inst_mret},
};

u32 instSm_num = sizeof(instSm) / sizeof(struct Instruction);
