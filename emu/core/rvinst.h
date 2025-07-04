#ifndef INST_H
#define INST_H

#include "core/core.h"
#include "riscv.h"
#include "types.h"

typedef enum exception (*instruction_func_t)(struct RiscvCore *core);
struct Instruction {
    u32                mask, match;
    instruction_func_t func;
};

extern struct Instruction inst32I[];
extern u32                inst32I_num;
extern struct Instruction inst32A[];
extern u32                inst32A_num;

#endif
