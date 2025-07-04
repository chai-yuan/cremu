#include "core/core.h"
#include "riscv.h"
#include "rvdecode.h"
#include "rvinst.h"

#define EXEC(inst_set)                                                                                                 \
    do {                                                                                                               \
        for (u32 i = 0; i < inst_set##_num; i++) {                                                                     \
            if ((core->inst & inst_set[i].mask) == inst_set[i].match)                                                  \
                return inst_set[i].func(core);                                                                         \
        }                                                                                                              \
    } while (0);

enum exception rvcore_exec(struct RiscvCore *core) {
    if ((core->inst & 0x3) == 0x3) {
        core->next_pc = core->pc + 4;
        EXEC(inst32I);
        EXEC(inst32A);
    } else {
        core->next_pc = core->pc + 2;
    }

    core->exce_val = core->inst;
    return CAUSE_ILLEGAL_INSTRUCTION;
}
