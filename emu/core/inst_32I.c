#include "core/core.h"
#include "rvdecode.h"
#include "rvinst.h"

enum exception inst_add(struct RiscvCore *core) {
    *RD(core) = RS1(core) + RS2(core);
    return CAUSE_NONE;
}

enum exception inst_sub(struct RiscvCore *core) {
    *RD(core) = RS1(core) - RS2(core);
    return CAUSE_NONE;
}

enum exception inst_xor(struct RiscvCore *core) {
    *RD(core) = RS1(core) ^ RS2(core);
    return CAUSE_NONE;
}

enum exception inst_or(struct RiscvCore *core) {
    *RD(core) = RS1(core) | RS2(core);
    return CAUSE_NONE;
}

enum exception inst_and(struct RiscvCore *core) {
    *RD(core) = RS1(core) & RS2(core);
    return CAUSE_NONE;
}

enum exception inst_sll(struct RiscvCore *core) {
    u8 shamt  = RS2(core) & 0x3F;
    *RD(core) = RS1(core) << shamt;
    return CAUSE_NONE;
}

enum exception inst_slt(struct RiscvCore *core) {
    *RD(core) = (isize)RS1(core) < (isize)RS2(core) ? 1 : 0;
    return CAUSE_NONE;
}

enum exception inst_sltu(struct RiscvCore *core) {
    *RD(core) = RS1(core) < RS2(core) ? 1 : 0;
    return CAUSE_NONE;
}

enum exception inst_srl(struct RiscvCore *core) {
    u8 shamt  = RS2(core) & 0x3F;
    *RD(core) = RS1(core) >> shamt;
    return CAUSE_NONE;
}

enum exception inst_sra(struct RiscvCore *core) {
    u8 shamt  = RS2(core) & 0x3F;
    *RD(core) = (isize)RS1(core) >> shamt;
    return CAUSE_NONE;
}

enum exception inst_addi(struct RiscvCore *core) {
    *RD(core) = RS1(core) + immI(core);
    return CAUSE_NONE;
}

enum exception inst_xori(struct RiscvCore *core) {
    *RD(core) = RS1(core) ^ immI(core);
    return CAUSE_NONE;
}

enum exception inst_ori(struct RiscvCore *core) {
    *RD(core) = RS1(core) | immI(core);
    return CAUSE_NONE;
}

enum exception inst_andi(struct RiscvCore *core) {
    *RD(core) = RS1(core) & immI(core);
    return CAUSE_NONE;
}

enum exception inst_slli(struct RiscvCore *core) {
    u8 shamt  = immI(core) & 0x3F;
    *RD(core) = RS1(core) << shamt;
    return CAUSE_NONE;
}

enum exception inst_srli(struct RiscvCore *core) {
    u8 shamt  = immI(core) & 0x3F;
    *RD(core) = RS1(core) >> shamt;
    return CAUSE_NONE;
}

enum exception inst_srai(struct RiscvCore *core) {
    u8 shamt  = immI(core) & 0x3F;
    *RD(core) = (isize)RS1(core) >> shamt;
    return CAUSE_NONE;
}

enum exception inst_slti(struct RiscvCore *core) {
    *RD(core) = (isize)RS1(core) < immI(core) ? 1 : 0;
    return CAUSE_NONE;
}

enum exception inst_sltiu(struct RiscvCore *core) {
    *RD(core) = RS1(core) < (usize)immI(core) ? 1 : 0;
    return CAUSE_NONE;
}

enum exception inst_beq(struct RiscvCore *core) {
    if ((isize)RS1(core) == (isize)RS2(core))
        core->next_pc = core->pc + immB(core);
    return CAUSE_NONE;
}

enum exception inst_bne(struct RiscvCore *core) {
    if ((isize)RS1(core) != (isize)RS2(core))
        core->next_pc = core->pc + immB(core);
    return CAUSE_NONE;
}

enum exception inst_blt(struct RiscvCore *core) {
    if ((isize)RS1(core) < (isize)RS2(core))
        core->next_pc = core->pc + immB(core);
    return CAUSE_NONE;
}

enum exception inst_bge(struct RiscvCore *core) {
    if ((isize)RS1(core) >= (isize)RS2(core))
        core->next_pc = core->pc + immB(core);
    return CAUSE_NONE;
}

enum exception inst_bltu(struct RiscvCore *core) {
    if (RS1(core) < RS2(core))
        core->next_pc = core->pc + immB(core);
    return CAUSE_NONE;
}
enum exception inst_bgeu(struct RiscvCore *core) {
    if (RS1(core) >= RS2(core))
        core->next_pc = core->pc + immB(core);
    return CAUSE_NONE;
}
enum exception inst_jal(struct RiscvCore *core) {
    core->next_pc = core->pc + immJ(core);
    *RD(core)     = core->pc + 4;
    return CAUSE_NONE;
}
enum exception inst_jalr(struct RiscvCore *core) {
    core->next_pc = (RS1(core) + immI(core)) & ~1;
    *RD(core)     = core->pc + 4;
    return CAUSE_NONE;
}
enum exception inst_lui(struct RiscvCore *core) {
    *RD(core) = immU(core);
    return CAUSE_NONE;
}
enum exception inst_auipc(struct RiscvCore *core) {
    *RD(core) = core->pc + immU(core);
    return CAUSE_NONE;
}

enum exception inst_lb(struct RiscvCore *core) {
    usize          data;
    enum exception ret = rvcore_mmu_read(core, RS1(core) + immI(core), 1, &data);
    *RD(core)          = (i8)data;
    return ret;
}
enum exception inst_lh(struct RiscvCore *core) {
    usize          data;
    enum exception ret = rvcore_mmu_read(core, RS1(core) + immI(core), 2, &data);
    *RD(core)          = (i16)data;
    return ret;
}
enum exception inst_lw(struct RiscvCore *core) {
    usize          data;
    enum exception ret = rvcore_mmu_read(core, RS1(core) + immI(core), 4, &data);
    *RD(core)          = (i32)data;
    return ret;
}
enum exception inst_lbu(struct RiscvCore *core) {
    usize          data = 0;
    enum exception ret  = rvcore_mmu_read(core, RS1(core) + immI(core), 1, &data);
    *RD(core)           = data;
    return ret;
}
enum exception inst_lhu(struct RiscvCore *core) {
    usize          data = 0;
    enum exception ret  = rvcore_mmu_read(core, RS1(core) + immI(core), 2, &data);
    *RD(core)           = data;
    return ret;
}
enum exception inst_sb(struct RiscvCore *core) {
    return rvcore_mmu_write(core, RS1(core) + immS(core), 1, RS2(core));
}
enum exception inst_sh(struct RiscvCore *core) {
    return rvcore_mmu_write(core, RS1(core) + immS(core), 2, RS2(core));
}
enum exception inst_sw(struct RiscvCore *core) {
    return rvcore_mmu_write(core, RS1(core) + immS(core), 4, RS2(core));
}

enum exception inst_ecall(struct RiscvCore *core) {
    if (core->mode == USER) {
        return CAUSE_USER_ECALL;
    } else if (core->mode == SUPERVISOR) {
        return CAUSE_SUPERVISOR_ECALL;
    } else if (core->mode == MACHINE) {
        return CAUSE_MACHINE_ECALL;
    }
    return CAUSE_NONE;
}

enum exception inst_ebreak(struct RiscvCore *core) {
    core->exce_val = core->pc;
    return CAUSE_BREAKPOINT;
}

enum exception inst_fence(struct RiscvCore *core) {return CAUSE_NONE;}

struct Instruction inst32I[] = {
    {.mask = 0xfe00707f, .match = 0x33, .func = inst_add},
    {.mask = 0xfe00707f, .match = 0x40000033, .func = inst_sub},
    {.mask = 0xfe00707f, .match = 0x4033, .func = inst_xor},
    {.mask = 0xfe00707f, .match = 0x6033, .func = inst_or},
    {.mask = 0xfe00707f, .match = 0x7033, .func = inst_and},
    {.mask = 0xfe00707f, .match = 0x1033, .func = inst_sll},
    {.mask = 0xfe00707f, .match = 0x2033, .func = inst_slt},
    {.mask = 0xfe00707f, .match = 0x3033, .func = inst_sltu},
    {.mask = 0xfe00707f, .match = 0x5033, .func = inst_srl},
    {.mask = 0xfe00707f, .match = 0x40005033, .func = inst_sra},
    {.mask = 0x707f, .match = 0x13, .func = inst_addi},
    {.mask = 0x707f, .match = 0x4013, .func = inst_xori},
    {.mask = 0x707f, .match = 0x6013, .func = inst_ori},
    {.mask = 0x707f, .match = 0x7013, .func = inst_andi},
    {.mask = 0xfc00707f, .match = 0x1013, .func = inst_slli},
    {.mask = 0xfc00707f, .match = 0x5013, .func = inst_srli},
    {.mask = 0xfc00707f, .match = 0x40005013, .func = inst_srai},
    {.mask = 0x707f, .match = 0x2013, .func = inst_slti},
    {.mask = 0x707f, .match = 0x3013, .func = inst_sltiu},
    {.mask = 0x707f, .match = 0x3, .func = inst_lb},
    {.mask = 0x707f, .match = 0x1003, .func = inst_lh},
    {.mask = 0x707f, .match = 0x2003, .func = inst_lw},
    {.mask = 0x707f, .match = 0x4003, .func = inst_lbu},
    {.mask = 0x707f, .match = 0x5003, .func = inst_lhu},
    {.mask = 0x707f, .match = 0x23, .func = inst_sb},
    {.mask = 0x707f, .match = 0x1023, .func = inst_sh},
    {.mask = 0x707f, .match = 0x2023, .func = inst_sw},
    {.mask = 0x707f, .match = 0x67, .func = inst_jalr},
    {.mask = 0x707f, .match = 0x63, .func = inst_beq},
    {.mask = 0x707f, .match = 0x5063, .func = inst_bge},
    {.mask = 0x707f, .match = 0x7063, .func = inst_bgeu},
    {.mask = 0x707f, .match = 0x4063, .func = inst_blt},
    {.mask = 0x707f, .match = 0x6063, .func = inst_bltu},
    {.mask = 0x707f, .match = 0x1063, .func = inst_bne},
    {.mask = 0x7f, .match = 0x37, .func = inst_lui},
    {.mask = 0x7f, .match = 0x6f, .func = inst_jal},
    {.mask = 0x7f, .match = 0x17, .func = inst_auipc},

    {.mask = 0xffffffff, .match = 0x00000073, .func = inst_ecall},
    {.mask = 0xffffffff, .match = 0x100073, .func = inst_ebreak},
    {.mask = 0x0000707f, .match = 0x0000000f, .func = inst_fence},
};

u32 inst32I_num = sizeof(inst32I) / sizeof(struct Instruction);
