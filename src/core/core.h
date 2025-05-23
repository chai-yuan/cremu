#ifndef RV_CORE_PRIV_H
#define RV_CORE_PRIV_H

#include "core/riscv.h"
#include "core/rvcore.h"

void rvcore_decode_init(struct RiscvDecode *decode);

bool rvcore_csr_read(struct RiscvCore *core, u16 addr, usize *value);
void rvcore_csr_write(struct RiscvCore *core, u16 addr, usize value);

void rvcore_exec(struct RiscvCore *core);
void rvcore_exception_handle(struct RiscvCore *core);
bool rvcore_interrupt_handle(struct RiscvCore *core);

#define CSRR(addr, value)                                                                                              \
    do {                                                                                                               \
        if (!rvcore_csr_read(core, addr, &value)) {                                                                    \
            DEC.exception     = ILLEGAL_INSTRUCTION;                                                                   \
            DEC.exception_val = DEC.inst;                                                                              \
            return;                                                                                                    \
        }                                                                                                              \
    } while (0);
#define CSRW(addr, value) rvcore_csr_write(core, addr, value)

#define DR(addr, size, value) core->device_func.read(core->device_func.context, addr, size, value)
#define DW(addr, size, value) core->device_func.write(core->device_func.context, addr, size, value)

typedef void (*instruction_func_t)(struct RiscvCore *core);
struct Instruction {
    u32                mask, match;
    instruction_func_t func;
};

#define RD core->regs[core->decode.rd]
#define RS1 core->regs[core->decode.rs1]
#define RS2 core->regs[core->decode.rs2]
#define DEC core->decode
#define MR(addr, size, data)                                                                                           \
    do {                                                                                                               \
        if (EXC_NONE != (DEC.exception = rvcore_mmu_read(core, addr, size, &data))) {                                  \
            DEC.exception_val = addr;                                                                                  \
            return;                                                                                                    \
        }                                                                                                              \
    } while (0);
#define MW(addr, size, data)                                                                                           \
    do {                                                                                                               \
        if (EXC_NONE != (DEC.exception = rvcore_mmu_write(core, addr, size, data)))                                    \
            DEC.exception_val = addr;                                                                                  \
    } while (0);

#define MSTATUS_WMASK IS_RV64(0x80000000ffffffffLL, 0x8fffffff)
#define SSTATUS_RMASK IS_RV64(0x80000003000de762LL, 0x800de762)
#define SSTATUS_WMASK IS_RV64(0x80000000000de762LL, 0x800de762)
#define IP_WMASK 0x26
#define MEDELEG_WMASK 0xfb3ff
#define MIDELEG_WMASK 0x222
// MTVEC
#define MTVEC_MODE GET_BITFIELD(core->csrs[MTVEC], 0, 2)
#define MTVEC_BASE (GET_BITFIELD(core->csrs[MTVEC], 2, IS_RV64(62, 30)) * 4)
#define STVEC_MODE GET_BITFIELD(core->csrs[STVEC], 0, 2)
#define STVEC_BASE (GET_BITFIELD(core->csrs[STVEC], 2, IS_RV64(62, 30)) * 4)
// MSTATUS
#define MSTATUS_SIE GET_BITFIELD(core->csrs[MSTATUS], 1, 1)
#define MSTATUS_MIE GET_BITFIELD(core->csrs[MSTATUS], 3, 1)
#define MSTATUS_SPIE GET_BITFIELD(core->csrs[MSTATUS], 5, 1)
#define MSTATUS_UBE GET_BITFIELD(core->csrs[MSTATUS], 6, 1)
#define MSTATUS_MPIE GET_BITFIELD(core->csrs[MSTATUS], 7, 1)
#define MSTATUS_SPP GET_BITFIELD(core->csrs[MSTATUS], 8, 1)
#define MSTATUS_VS GET_BITFIELD(core->csrs[MSTATUS], 9, 2)
#define MSTATUS_MPP GET_BITFIELD(core->csrs[MSTATUS], 11, 2)
#define MSTATUS_FS GET_BITFIELD(core->csrs[MSTATUS], 13, 2)
#define MSTATUS_XS GET_BITFIELD(core->csrs[MSTATUS], 15, 2)
#define MSTATUS_MPRV GET_BITFIELD(core->csrs[MSTATUS], 17, 1)
#define MSTATUS_SUM GET_BITFIELD(core->csrs[MSTATUS], 18, 1)
#define MSTATUS_MXR GET_BITFIELD(core->csrs[MSTATUS], 19, 1)
#define MSTATUS_TVM GET_BITFIELD(core->csrs[MSTATUS], 20, 1)
#define MSTATUS_TW GET_BITFIELD(core->csrs[MSTATUS], 21, 1)
#define MSTATUS_TSR GET_BITFIELD(core->csrs[MSTATUS], 22, 1)
#define MSTATUS_UXL GET_BITFIELD(core->csrs[MSTATUS], 32, 2)
#define MSTATUS_SXL GET_BITFIELD(core->csrs[MSTATUS], 34, 2)
#define MSTATUS_SBE GET_BITFIELD(core->csrs[MSTATUS], 36, 1)
#define MSTATUS_MBE GET_BITFIELD(core->csrs[MSTATUS], 37, 1)
#define MSTATUS_SD GET_BITFIELD(core->csrs[MSTATUS], IS_RV64(63, 31), 1)

#define MSTATUS_SET_SIE(value) SET_BITFIELD(core->csrs[MSTATUS], 1, 1, value)
#define MSTATUS_SET_MIE(value) SET_BITFIELD(core->csrs[MSTATUS], 3, 1, value)
#define MSTATUS_SET_SPIE(value) SET_BITFIELD(core->csrs[MSTATUS], 5, 1, value)
#define MSTATUS_SET_MPIE(value) SET_BITFIELD(core->csrs[MSTATUS], 7, 1, value)
#define MSTATUS_SET_SPP(value) SET_BITFIELD(core->csrs[MSTATUS], 8, 1, value)
#define MSTATUS_SET_MPP(value) SET_BITFIELD(core->csrs[MSTATUS], 11, 2, value)
// SATP
#define SATP_PPN GET_BITFIELD(core->csrs[SATP], 0, IS_RV64(44, 22))
#define SATP_ASID GET_BITFIELD(core->csrs[SATP], IS_RV64(44, 22), IS_RV64(16, 9))
#define SATP_MODE GET_BITFIELD(core->csrs[SATP], IS_RV64(60, 31), IS_RV64(4, 1))
#define SATP_SET_MODE(value) SET_BITFIELD(core->csrs[SATP], IS_RV64(60, 31), IS_RV64(4, 1), value)
// SV32PTE
#define SV32_V(pte) GET_BITFIELD(pte, 0, 1)
#define SV32_R(pte) GET_BITFIELD(pte, 1, 1)
#define SV32_W(pte) GET_BITFIELD(pte, 2, 1)
#define SV32_X(pte) GET_BITFIELD(pte, 3, 1)
#define SV32_U(pte) GET_BITFIELD(pte, 4, 1)
#define SV32_A(pte) GET_BITFIELD(pte, 6, 1)
#define SV32_D(pte) GET_BITFIELD(pte, 7, 1)
#define SV32_PPN(pte) GET_BITFIELD(pte, 10, 22)
// SV39PTE
#define SV39_V(pte) GET_BITFIELD(pte, 0, 1)
#define SV39_R(pte) GET_BITFIELD(pte, 1, 1)
#define SV39_W(pte) GET_BITFIELD(pte, 2, 1)
#define SV39_X(pte) GET_BITFIELD(pte, 3, 1)
#define SV39_U(pte) GET_BITFIELD(pte, 4, 1)
#define SV39_A(pte) GET_BITFIELD(pte, 6, 1)
#define SV39_D(pte) GET_BITFIELD(pte, 7, 1)
#define SV39_PPN(pte) GET_BITFIELD(pte, 10, 44)
// IP
#define IP_SSIP(pending) GET_BITFIELD(pending, 1, 1)
#define IP_MSIP(pending) GET_BITFIELD(pending, 3, 1)
#define IP_STIP(pending) GET_BITFIELD(pending, 5, 1)
#define IP_MTIP(pending) GET_BITFIELD(pending, 7, 1)
#define IP_SEIP(pending) GET_BITFIELD(pending, 9, 1)
#define IP_MEIP(pending) GET_BITFIELD(pending, 11, 1)

#endif
