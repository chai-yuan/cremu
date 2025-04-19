#ifndef NEMU_M_H
#define NEMU_M_H

#include "core/rvcore.h"
#include "device/bus.h"
#include "device/clint.h"
#include "device/sram.h"
#include "device/uartlite.h"
#include "machine/machine.h"

struct NemuMachine {
    struct RiscvCore core;
    struct BusDevice bus;
    struct Sram      sram;
    struct CLINT     clint;
    struct UARTLite  uart;
    struct GPU       gpu;
};

struct MachineFunc nemu_machine_init(struct NemuMachine *machine, struct PortableOperations init);

#endif
