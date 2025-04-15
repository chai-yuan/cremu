#include "machine/nemu.h"

void nemu_machine_step(void *context) {
    struct NemuMachine *machine = context;

    rvcore_step(&machine->core, (struct RiscvEnvInfo){0});
}

enum MachineCode nemu_machine_check(void *context) {
    struct NemuMachine *machine = context;

    if (machine->core.decode.exception == BREAKPOINT) {
        return machine->core.regs[10] == 0 ? GOOD_END : BAD_END;
    }
    return RUNNING;
}

struct MachineFunc nemu_machine_init(struct NemuMachine *machine, struct PortableOperations init) {
    bus_device_init(&machine->bus);
    sram_init(&machine->sram, init.sram_data, init.sram_size);
    uart_init(&machine->uart, init.get_char, init.put_char);

    bus_device_add(&machine->bus, 0x80000000, machine->sram.len, sram_get_func(&machine->sram));
    bus_device_add(&machine->bus, 0x10000000, UART_SIZE, uart_get_func(&machine->uart));

    rvcore_init(&machine->core, bus_device_get_func(&machine->bus));
    machine->core.pc = 0x80000000;

    return (struct MachineFunc){
        .context = machine,
        .step    = nemu_machine_step,
        .check   = nemu_machine_check,
    };
}
