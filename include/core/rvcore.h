#ifndef RV_CORE_H
#define RV_CORE_H

#include "core/riscv.h"
#include "device/device.h"
#include "types.h"

struct RiscvDecode {
    u32            inst;
    u8             rd, rs1, rs2, rs1_, rs2_;
    isize          immI, immB, immU, immJ, immS;
    usize          csr_imm, next_pc;
    bool           is_inst16;
    enum exception exception;
    usize          exception_val;
    enum interrupt interrupt;
};

struct RiscvEnvInfo {
    u64  time;
    bool meint;
    bool seint;
    bool mtint;
};

struct RiscvCore {
    usize     regs[32];
    usize     pc;
    usize     csrs[4096];
    enum mode mode;
    u64       reservation_addr;
    bool      wfi;

    struct RiscvDecode decode;
    struct DeviceFunc  device_func;
};

// basic
void rvcore_init(struct RiscvCore *core, struct DeviceFunc device_func);
void rvcore_step(struct RiscvCore *core, struct RiscvEnvInfo envinfo);
// mmu
enum exception rvcore_mmu_read(struct RiscvCore *core, usize addr, u8 size, usize *data);
enum exception rvcore_mmu_write(struct RiscvCore *core, usize addr, u8 size, usize data);
void           rvcore_mmu_fetch(struct RiscvCore *core);

void rvcore_update(struct RiscvCore *core, struct RiscvEnvInfo envinfo);
#endif
