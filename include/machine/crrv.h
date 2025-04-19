#ifndef CRRV_M_H
#define CRRV_M_H

#include "core/rvcore.h"
#include "device/bus.h"
#include "device/clint.h"
#include "device/sram.h"
#include "device/uartlite.h"
#include "machine/machine.h"

struct CrrvMachine {
    struct RiscvCore core;
    struct BusDevice bus;
    struct Sram      sram;
    struct CLINT     clint;
    struct UARTLite  uart;
};

struct MachineFunc crrv_machine_init(struct CrrvMachine *machine, struct PortableOperations init);

#endif
