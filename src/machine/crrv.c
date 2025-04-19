#include "machine/crrv.h"

void crrv_machine_step(void *context) {
    struct CrrvMachine *machine = context;
    struct DeviceFunc   bus     = bus_device_get_func(&machine->bus);
    bus.update(bus.context, 1);
    rvcore_step(&machine->core, (struct RiscvEnvInfo){0});
}

enum MachineCode crrv_machine_check(void *context) {
    struct CrrvMachine *machine = context;

    if (machine->core.decode.exception == BREAKPOINT) {
        return machine->core.regs[10] == 0 ? GOOD_END : BAD_END;
    }
    return RUNNING;
}

struct MachineFunc crrv_machine_init(struct CrrvMachine *machine, struct PortableOperations init) {
    bus_device_init(&machine->bus);
    sram_init(&machine->sram, init.sram_data, init.sram_size);
    uartlite_init(&machine->uart, init.get_char, init.put_char);
    clint_init(&machine->clint);

    bus_device_add(&machine->bus, 0x80000000, machine->sram.len, sram_get_func(&machine->sram));
    bus_device_add(&machine->bus, 0x10000000, UART_lite_SIZE, uartlite_get_func(&machine->uart));
    bus_device_add(&machine->bus, 0x02000000, CLINT_SIZE, clint_get_func(&machine->clint));

    rvcore_init(&machine->core, bus_device_get_func(&machine->bus));
    machine->core.pc = 0x80000000;

    return (struct MachineFunc){
        .context = machine,
        .step    = crrv_machine_step,
        .check   = crrv_machine_check,
    };
}
