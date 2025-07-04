#ifndef TEST_MACHINE_H
#define TEST_MACHINE_H

#include "core/core.h"
#include "device/bus.h"
#include "device/sram.h"
#include "machine/machine.h"

// 用于riscv测试的机器
struct TestMachine {
    struct RiscvCore core;
    struct BusDevice bus;
    struct Sram      sram;
};

struct MachineFunc test_machine_init(struct TestMachine *machine, struct PortableOperations init);

#endif
