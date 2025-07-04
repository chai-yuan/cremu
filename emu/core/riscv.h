#ifndef RVCODE_H
#define RVCODE_H

#include "core/core.h"
#include "types.h"

enum exception {
    CAUSE_NONE                     = -1,
    CAUSE_MISALIGNED_FETCH         = 0x0,
    CAUSE_FETCH_ACCESS             = 0x1,
    CAUSE_ILLEGAL_INSTRUCTION      = 0x2,
    CAUSE_BREAKPOINT               = 0x3,
    CAUSE_MISALIGNED_LOAD          = 0x4,
    CAUSE_LOAD_ACCESS              = 0x5,
    CAUSE_MISALIGNED_STORE         = 0x6,
    CAUSE_STORE_ACCESS             = 0x7,
    CAUSE_USER_ECALL               = 0x8,
    CAUSE_SUPERVISOR_ECALL         = 0x9,
    CAUSE_VIRTUAL_SUPERVISOR_ECALL = 0xa,
    CAUSE_MACHINE_ECALL            = 0xb,
    CAUSE_FETCH_PAGE_FAULT         = 0xc,
    CAUSE_LOAD_PAGE_FAULT          = 0xd,
    CAUSE_STORE_PAGE_FAULT         = 0xf,
    CAUSE_DOUBLE_TRAP              = 0x10,
    CAUSE_SOFTWARE_CHECK_FAULT     = 0x12,
    CAUSE_HARDWARE_ERROR_FAULT     = 0x13,
    CAUSE_FETCH_GUEST_PAGE_FAULT   = 0x14,
    CAUSE_LOAD_GUEST_PAGE_FAULT    = 0x15,
    CAUSE_VIRTUAL_INSTRUCTION      = 0x16,
    CAUSE_STORE_GUEST_PAGE_FAULT   = 0x17,
};

enum extension {
    A,
    B,
    C,
    D,
    F,
    H,
    I,
    M,
    S,
    Sm,
    Sv32,
    Sv39,
    V,
    Zicsr,
    Zifencei,
};

enum csraddr {
    // Unprivileged Counter/Timers
    CYCLE   = 0xc00,
    TIME    = 0xc01,
    INSTRET = 0xc02,
    // Supervisor Trap Setup
    SSTATUS    = 0x100,
    SIE        = 0x104,
    STVEC      = 0x105,
    SCOUNTEREN = 0x106,
    // Supervisor Trap Handling
    SSCRATCH = 0x140,
    SEPC     = 0x141,
    SCAUSE   = 0x142,
    STVAL    = 0x143,
    SIP      = 0x144,
    // Supervisor Protection and Translation
    SATP = 0x180,
    // Machine Information Registers
    MVENDORID  = 0xf11,
    MARCHID    = 0xf12,
    MIMPID     = 0xf13,
    MHARTID    = 0xf14,
    MCONFIGPTR = 0xf15,
    // Machine Trap Setup
    MSTATUS    = 0x300,
    MISA       = 0x301,
    MEDELEG    = 0x302,
    MIDELEG    = 0x303,
    MIE        = 0x304,
    MTVEC      = 0x305,
    MCOUNTEREN = 0x306,
    // Machine Trap Handling
    MSCRATCH = 0x340,
    MEPC     = 0x341,
    MCAUSE   = 0x342,
    MTVAL    = 0x343,
    MIP      = 0x344,
    // Machine Memory Protection
    PMPCFG0   = 0x3a0,
    PMPCFG1   = 0x3a1,
    PMPCFG2   = 0x3a2,
    PMPCFG3   = 0x3a3,
    PMPADDR0  = 0x3b0,
    PMPADDR1  = 0x3b1,
    PMPADDR2  = 0x3b2,
    PMPADDR3  = 0x3b3,
    PMPADDR4  = 0x3b4,
    PMPADDR5  = 0x3b5,
    PMPADDR6  = 0x3b6,
    PMPADDR7  = 0x3b7,
    PMPADDR8  = 0x3b8,
    PMPADDR9  = 0x3b9,
    PMPADDR10 = 0x3ba,
    PMPADDR11 = 0x3bb,
    PMPADDR12 = 0x3bc,
    PMPADDR13 = 0x3bd,
    PMPADDR14 = 0x3be,
    PMPADDR15 = 0x3bf,
    // Machine Counter/Timers
    MCYCLE   = 0xb00,
    MINSTRET = 0xb02,
    TSELECT  = 0x7a0,
    TDATA1   = 0x7a1
};

// exec.c
enum exception rvcore_exec(struct RiscvCore *core);
// mmu.c
enum exception mmu_translate(struct RiscvCore *core, enum exception exc, usize addr, u64 *paddr);
enum exception rvcore_mmu_read(struct RiscvCore *core, usize addr, u8 size, usize *data);
enum exception rvcore_mmu_write(struct RiscvCore *core, usize addr, u8 size, usize data);
enum exception rvcore_mmu_fetch(struct RiscvCore *core);
// intr.c
enum exception rvcore_env_update(struct RiscvCore *core, struct RiscvEnvInfo envinfo);
// trap.c
void rvcore_trap_handle(struct RiscvCore *core, enum exception cause);
// csr.c


#endif
