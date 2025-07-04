#include "core/core.h"
#include "rvdecode.h"
#include "rvinst.h"
#include "utils/uint128.h"

enum exception inst_mul(struct RiscvCore *core) {
    *RD(core) = RS1(core) * RS2(core);
    return CAUSE_NONE;
}
enum exception inst_mulh(struct RiscvCore *core) {
    if (sizeof(usize) == 4) {
        *RD(core) = ((i64)(i32)RS1(core) * (i64)(i32)RS2(core)) >> 32;
    } else {
        struct uint128 a = signed2u128(RS1(core)), b = signed2u128(RS2(core));
        struct uint128 ans = u128mul(a, b);
        *RD(core)          = ((u64)ans.nums[3] << 32) | (u64)ans.nums[2];
    }
    return CAUSE_NONE;
}
enum exception inst_mulhsu(struct RiscvCore *core) {
    if (sizeof(usize) == 4) {
        *RD(core) = ((i64)(i32)RS1(core) * (u64)RS2(core)) >> 32;
    } else {
        struct uint128 a = signed2u128(RS1(core)), b = unsigned2u128(RS2(core));
        struct uint128 ans = u128mul(a, b);
        *RD(core)          = ((u64)ans.nums[3] << 32) | (u64)ans.nums[2];
    }
    return CAUSE_NONE;
}
enum exception inst_mulhu(struct RiscvCore *core) {
    if (sizeof(usize) == 4) {
        *RD(core) = ((u64)RS1(core) * (u64)RS2(core)) >> 32;
    } else {
        struct uint128 a = unsigned2u128(RS1(core)), b = unsigned2u128(RS2(core));
        struct uint128 ans = u128mul(a, b);
        *RD(core)          = ((u64)ans.nums[3] << 32) | (u64)ans.nums[2];
    }
    return CAUSE_NONE;
}
enum exception inst_div(struct RiscvCore *core) {
    isize src1 = RS1(core);
    isize src2 = RS2(core);
    if (src2 == 0) {
        *RD(core) = (isize)-1;
    } else if (src1 == INTMIN && src2 == -1) {
        *RD(core) = src1;
    } else {
        *RD(core) = src1 / src2;
    }
    return CAUSE_NONE;
}
enum exception inst_divu(struct RiscvCore *core) {
    usize src1 = RS1(core);
    usize src2 = RS2(core);
    if (src2 == 0) {
        *RD(core) = (isize)-1;
    } else {
        *RD(core) = src1 / src2;
    }
    return CAUSE_NONE;
}
enum exception inst_rem(struct RiscvCore *core) {
    isize src1 = RS1(core);
    isize src2 = RS2(core);
    if (src2 == 0) {
        *RD(core) = src1;
    } else if (src1 == INTMIN && src2 == -1) {
        *RD(core) = 0;
    } else {
        *RD(core) = src1 % src2;
    }
    return CAUSE_NONE;
}
enum exception inst_remu(struct RiscvCore *core) {
    usize src1 = RS1(core);
    usize src2 = RS2(core);
    if (src2 == 0) {
        *RD(core) = src1;
    } else {
        *RD(core) = src1 % src2;
    }
    return CAUSE_NONE;
}

struct Instruction inst32M[] = {
    {.mask = 0xfe00707f, .match = 0x2000033, .func = inst_mul},
    {.mask = 0xfe00707f, .match = 0x2001033, .func = inst_mulh},
    {.mask = 0xfe00707f, .match = 0x2002033, .func = inst_mulhsu},
    {.mask = 0xfe00707f, .match = 0x2003033, .func = inst_mulhu},
    {.mask = 0xfe00707f, .match = 0x2004033, .func = inst_div},
    {.mask = 0xfe00707f, .match = 0x2005033, .func = inst_divu},
    {.mask = 0xfe00707f, .match = 0x2006033, .func = inst_rem},
    {.mask = 0xfe00707f, .match = 0x2007033, .func = inst_remu},
};

u32 inst32M_num = sizeof(inst32M) / sizeof(struct Instruction);
