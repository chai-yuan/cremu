#include "core.h"
#include "debug.h"

void rvcore_trap_handle_s(struct RiscvCore *core, usize cause) {
    usize vec   = ((cause & INTMIN) && STVEC_MODE == 1) ? (4 * cause) : 0;
    DEC.next_pc = STVEC_BASE + vec;
    MSTATUS_SET_SPP(core->mode & 0x1);
    core->mode         = SUPERVISOR;
    core->csrs[SEPC]   = core->pc;
    core->csrs[SCAUSE] = cause;
    core->csrs[STVAL]  = DEC.exception_val;
    MSTATUS_SET_SPIE(MSTATUS_SIE);
    MSTATUS_SET_SIE(0);
}

void rvcore_trap_handle_m(struct RiscvCore *core, usize cause) {
    usize vec   = ((cause & INTMIN) && MTVEC_MODE == 1) ? (4 * cause) : 0;
    DEC.next_pc = MTVEC_BASE + vec;
    MSTATUS_SET_MPP(core->mode);
    core->mode         = MACHINE;
    core->csrs[MEPC]   = core->pc;
    core->csrs[MCAUSE] = cause;
    core->csrs[MTVAL]  = DEC.exception_val;
    MSTATUS_SET_MPIE(MSTATUS_MIE);
    MSTATUS_SET_MIE(0);
}

void rvcore_exception_handle(struct RiscvCore *core) {
    if (DEC.exception != EXC_NONE) {
        usize cause = DEC.exception;
        if ((core->mode <= SUPERVISOR) && ((core->csrs[MEDELEG] >> (u16)cause) & 1)) { // 异常委托
            rvcore_trap_handle_s(core, cause);
        } else {
            rvcore_trap_handle_m(core, cause);
        }
    } else {
        return;
    }
}

bool rvcore_interrupt_handle(struct RiscvCore *core) {
    usize pending = core->csrs[MIP] & core->csrs[MIE];
    if (pending == 0)
        return false;

    usize supervisor_enable = core->mode < SUPERVISOR || (core->mode == SUPERVISOR && MSTATUS_SIE);
    usize machine_enable    = core->mode < MACHINE || (core->mode == MACHINE && MSTATUS_MIE);

    usize cause = INT_NONE;
    if (IP_SSIP(pending))
        cause = SUPERVISOR_SOFTWARE_INTERRUPT;
    if (IP_STIP(pending))
        cause = SUPERVISOR_TIMER_INTERRUPT;
    if (IP_SEIP(pending))
        cause = SUPERVISOR_EXTERNAL_INTERRUPT;
    if (IP_MSIP(pending))
        cause = MACHINE_SOFTWARE_INTERRUPT;
    if (IP_MTIP(pending))
        cause = MACHINE_TIMER_INTERRUPT;
    if (IP_MEIP(pending))
        cause = MACHINE_EXTERNAL_INTERRUPT;

    if (cause != INT_NONE) {
        core->wfi = false; // 触发中断后清除休眠标志
        if (supervisor_enable) {
            rvcore_trap_handle_s(core, cause | INTMIN);
        } else if (machine_enable) {
            rvcore_trap_handle_m(core, cause | INTMIN);
        }
    }
    return cause != INT_NONE;
}
