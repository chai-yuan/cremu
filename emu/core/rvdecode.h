#include "core/core.h"
#include "macro.h"
#include "types.h"

static inline usize *RD(struct RiscvCore *core) {
    core->rd = GET_BITFIELD(core->inst, 7, 5);
    return &core->rd_val;
}
static inline usize RS1(struct RiscvCore *core) { return core->regs[GET_BITFIELD(core->inst, 15, 5)]; }
static inline usize RS2(struct RiscvCore *core) { return core->regs[GET_BITFIELD(core->inst, 20, 5)]; }

static inline isize immI(struct RiscvCore *core) { return SIGN_EXTEND(GET_BITFIELD(core->inst, 20, 12), 12); }
static inline isize immB(struct RiscvCore *core) {
    return (SIGN_EXTEND(GET_BITFIELD(core->inst, 31, 1), 1) << 12) | (GET_BITFIELD(core->inst, 25, 6) << 5) |
           (GET_BITFIELD(core->inst, 8, 4) << 1) | (GET_BITFIELD(core->inst, 7, 1) << 11);
}
static inline isize immU(struct RiscvCore *core) { return (SIGN_EXTEND(GET_BITFIELD(core->inst, 12, 20), 20) << 12); }
static inline isize immJ(struct RiscvCore *core) {
    return (SIGN_EXTEND(GET_BITFIELD(core->inst, 31, 1), 1) << 20) | (GET_BITFIELD(core->inst, 21, 10) << 1) |
           (GET_BITFIELD(core->inst, 20, 1) << 11) | (GET_BITFIELD(core->inst, 12, 8) << 12);
}
static inline isize immS(struct RiscvCore *core) {
    return (SIGN_EXTEND(GET_BITFIELD(core->inst, 25, 7), 7) << 5) | GET_BITFIELD(core->inst, 7, 5);
}
