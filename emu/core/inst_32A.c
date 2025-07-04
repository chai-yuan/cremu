#include "core/core.h"
#include "rvdecode.h"
#include "rvinst.h"

enum exception inst_lr_w(struct RiscvCore *core) {
    usize          data;
    enum exception ret     = rvcore_mmu_read(core, RS1(core), 4, &data);
    *RD(core)              = (isize)(i32)data;
    core->reservation_addr = RS1(core);
    return ret;
}

enum exception inst_sc_w(struct RiscvCore *core) {
    if (core->reservation_addr == RS1(core)) {
        enum exception ret     = rvcore_mmu_write(core, RS1(core), 4, (u32)RS2(core));
        *RD(core)              = 0;
        core->reservation_addr = -1;
        return ret;
    } else {
        *RD(core)              = 1;
        core->reservation_addr = -1;
        return CAUSE_NONE;
    }
}

enum exception inst_amoswap_w(struct RiscvCore *core) {
    usize          data;
    enum exception ret = rvcore_mmu_read(core, RS1(core), 4, &data);
    if (ret != CAUSE_NONE)
        return ret;
    ret       = rvcore_mmu_write(core, RS1(core), 4, (u32)RS2(core));
    *RD(core) = (isize)(i32)data;
    return ret;
}

enum exception inst_amoadd_w(struct RiscvCore *core) {
    usize          data;
    enum exception ret = rvcore_mmu_read(core, RS1(core), 4, &data);
    if (ret != CAUSE_NONE)
        return ret;
    i32 value  = (i32)data;
    i32 result = value + (i32)RS2(core);
    ret        = rvcore_mmu_write(core, RS1(core), 4, (u32)result);
    *RD(core)  = (isize)value;
    return ret;
}

enum exception inst_amoxor_w(struct RiscvCore *core) {
    usize          data;
    enum exception ret = rvcore_mmu_read(core, RS1(core), 4, &data);
    if (ret != CAUSE_NONE)
        return ret;
    i32 value  = (i32)data;
    i32 result = value ^ (i32)RS2(core);
    ret        = rvcore_mmu_write(core, RS1(core), 4, (u32)result);
    *RD(core)  = (isize)value;
    return ret;
}

enum exception inst_amoor_w(struct RiscvCore *core) {
    usize          data;
    enum exception ret = rvcore_mmu_read(core, RS1(core), 4, &data);
    if (ret != CAUSE_NONE)
        return ret;
    i32 value  = (i32)data;
    i32 result = value | (i32)RS2(core);
    ret        = rvcore_mmu_write(core, RS1(core), 4, (u32)result);
    *RD(core)  = (isize)value;
    return ret;
}

enum exception inst_amoand_w(struct RiscvCore *core) {
    usize          data;
    enum exception ret = rvcore_mmu_read(core, RS1(core), 4, &data);
    if (ret != CAUSE_NONE)
        return ret;
    i32 value  = (i32)data;
    i32 result = value & (i32)RS2(core);
    ret        = rvcore_mmu_write(core, RS1(core), 4, (u32)result);
    *RD(core)  = (isize)value;
    return ret;
}

enum exception inst_amomin_w(struct RiscvCore *core) {
    usize          data;
    enum exception ret = rvcore_mmu_read(core, RS1(core), 4, &data);
    if (ret != CAUSE_NONE)
        return ret;
    i32 value  = (i32)data;
    i32 result = (value < (i32)RS2(core)) ? value : (i32)RS2(core);
    ret        = rvcore_mmu_write(core, RS1(core), 4, (u32)result);
    *RD(core)  = (isize)value;
    return ret;
}

enum exception inst_amomax_w(struct RiscvCore *core) {
    usize          data;
    enum exception ret = rvcore_mmu_read(core, RS1(core), 4, &data);
    if (ret != CAUSE_NONE)
        return ret;
    i32 value  = (i32)data;
    i32 result = (value > (i32)RS2(core)) ? value : (i32)RS2(core);
    ret        = rvcore_mmu_write(core, RS1(core), 4, (u32)result);
    *RD(core)  = (isize)value;
    return ret;
}

enum exception inst_amominu_w(struct RiscvCore *core) {
    usize          data;
    enum exception ret = rvcore_mmu_read(core, RS1(core), 4, &data);
    if (ret != CAUSE_NONE)
        return ret;
    u32 value  = (i32)data;
    u32 result = (value < (u32)RS2(core)) ? value : (u32)RS2(core);
    ret        = rvcore_mmu_write(core, RS1(core), 4, result);
    *RD(core)  = (isize)(i32)value;
    return ret;
}

enum exception inst_amomaxu_w(struct RiscvCore *core) {
    usize          data;
    enum exception ret = rvcore_mmu_read(core, RS1(core), 4, &data);
    if (ret != CAUSE_NONE)
        return ret;
    u32 value  = (i32)data;
    u32 result = (value > (u32)RS2(core)) ? value : (u32)RS2(core);
    ret        = rvcore_mmu_write(core, RS1(core), 4, result);
    *RD(core)  = (isize)(i32)value;
    return ret;
}

struct Instruction inst32A[] = {
    {.mask = 0xf800707f, .match = 0x1000202f, .func = inst_lr_w},
    {.mask = 0xf800707f, .match = 0x1800202f, .func = inst_sc_w},
    {.mask = 0xf800707f, .match = 0x0800202f, .func = inst_amoswap_w},
    {.mask = 0xf800707f, .match = 0x0000202f, .func = inst_amoadd_w},
    {.mask = 0xf800707f, .match = 0x2000202f, .func = inst_amoxor_w},
    {.mask = 0xf800707f, .match = 0x4000202f, .func = inst_amoor_w},
    {.mask = 0xf800707f, .match = 0x6000202f, .func = inst_amoand_w},
    {.mask = 0xf800707f, .match = 0x8000202f, .func = inst_amomin_w},
    {.mask = 0xf800707f, .match = 0xa000202f, .func = inst_amomax_w},
    {.mask = 0xf800707f, .match = 0xc000202f, .func = inst_amominu_w},
    {.mask = 0xf800707f, .match = 0xe000202f, .func = inst_amomaxu_w},
};

u32 inst32A_num = sizeof(inst32A) / sizeof(struct Instruction);
