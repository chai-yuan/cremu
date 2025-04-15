#ifndef SPIKE_H
#define SPIKE_H

#include "core/rvcore.h"
#include "device/bus.h"
#include "device/clint.h"
#include "device/plic.h"
#include "device/sram.h"
#include "device/uart16550.h"
#include "machine/machine.h"

extern const unsigned char spike_rv32_dtb[];
extern const unsigned int  spike_rv32_dtb_size;

extern const unsigned char spike_rv64_dtb[];
extern const unsigned int  spike_rv64_dtb_size;

// 模拟兼容spike的机器实现
struct SpikeMachine {
    struct RiscvCore core;
    struct BusDevice bus;
    struct Sram      sram;
    struct Sram      rom;
    struct UART16550 uart;
    struct CLINT     clint;
    struct PLIC      plic;
};

struct MachineFunc spike_machine_init(struct SpikeMachine *machine, struct PortableOperations init);

#endif
