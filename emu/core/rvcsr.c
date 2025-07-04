#include "riscv.h"

#define MSTATUS_WMASK IS_RV64(0x80000000ffffffffLL, 0x8fffffff)
#define SSTATUS_RMASK IS_RV64(0x80000003000de762LL, 0x800de762)
#define SSTATUS_WMASK IS_RV64(0x80000000000de762LL, 0x800de762)
#define IP_WMASK 0x26
#define MEDELEG_WMASK 0xfb3ff
#define MIDELEG_WMASK 0x222

enum exception rvcore_csr_read(struct RiscvCore *core, u16 addr, usize *value) {
    if (((addr >> 8) & 0x3) > core->mode) // 权限检查
        return CAUSE_ILLEGAL_INSTRUCTION;

    if (addr == 0x14d)
        return CAUSE_ILLEGAL_INSTRUCTION; // Sstc
    if (addr == 0x30c)
        return CAUSE_ILLEGAL_INSTRUCTION; // Smstateen/Ssstateen
    if (addr == 0xda0)
        return CAUSE_ILLEGAL_INSTRUCTION; // Sscofpmf
    if (addr == 0x321)
        return CAUSE_ILLEGAL_INSTRUCTION; // Smcntrpmf
    if (addr == 0xfb0)
        return CAUSE_ILLEGAL_INSTRUCTION; // ???

    switch (addr) {
    case SSTATUS:
        *value = core->csrs[MSTATUS] & SSTATUS_RMASK;
        break;
    case SIE:
        *value = core->csrs[MIE] & core->csrs[MIDELEG];
        break;
    case SIP:
        *value = core->csrs[MIP] & core->csrs[MIDELEG];
        break;

    default:
        *value = core->csrs[addr];
    }
    return true;
}

enum exception rvcore_csr_write(struct RiscvCore *core, u16 addr, usize value) {
    if ((addr >> 10) == 0x3) // read only
        return CAUSE_NONE;
    if (addr == MISA) // 简化实现，misa不可写
        return CAUSE_NONE;
    if ((0x3c0 <= addr && addr <= 0x3ef) || (0x3a4 <= addr && addr <= 0x3af)) // pmp仅开启前16项
        return CAUSE_NONE;
    if ((0xb03 <= addr && addr <= 0xb1f) || (0xb83 <= addr && addr <= 0xb9f)) // 忽略事件触发CSR计数器
        return CAUSE_NONE;

    switch (addr) {
    case MSTATUS: {
        core->csrs[MSTATUS] = (core->csrs[MSTATUS] & ~MSTATUS_WMASK) | (value & MSTATUS_WMASK);
        break;
    }
    case SSTATUS: {
        core->csrs[MSTATUS] = (core->csrs[MSTATUS] & ~SSTATUS_WMASK) | (value & SSTATUS_WMASK);
        break;
    }
    case MEDELEG: {
        core->csrs[MEDELEG] = value & MEDELEG_WMASK;
        break;
    }
    case MIDELEG: {
        core->csrs[MIDELEG] = value & MIDELEG_WMASK;
        break;
    }
    case MIP: {
        core->csrs[MIP] = (core->csrs[MIP] & ~IP_WMASK) | (value & IP_WMASK);
        break;
    }
    case SIE: {
        usize mask      = core->csrs[MIDELEG];
        core->csrs[MIE] = (core->csrs[MIE] & ~mask) | (value & mask);
        break;
    }
    case SIP: {
        usize mask      = core->csrs[MIDELEG] & IP_WMASK;
        core->csrs[MIP] = (core->csrs[MIP] & ~mask) | (value & mask);
        break;
    }
    case SATP: {
        core->csrs[SATP] = value;
#if CURRENT_ARCH == ARCH_RV64 // 64位情况下仅支持sv39
        if (SATP_MODE != 0x8 && SATP_MODE != 0)
            SATP_SET_MODE(0);
#endif
        break;
    }
    default:
        core->csrs[addr] = value;
    }
    return CAUSE_NONE;
}
