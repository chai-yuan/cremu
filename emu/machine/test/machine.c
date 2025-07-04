#include "machine/machine.h"
#include "machine/test.h"

void test_machine_step(void *context) {
    struct TestMachine *machine = context;

    rvcore_step(&machine->core, (struct RiscvEnvInfo){.meint = false, .seint = false, .mtint = false, .time = false});
}

enum MachineCode test_machine_check(void *context) {
    struct TestMachine *machine = context;
    if (machine->core.halt) {
        return machine->core.regs[10] == 0 ? GOOD_END : BAD_END;
    }
    return RUNNING;
}

struct MachineFunc test_machine_init(struct TestMachine *machine, struct PortableOperations init) {
    bus_device_init(&machine->bus);
    sram_init(&machine->sram, init.sram_data, init.sram_size);

    bus_device_add(&machine->bus, 0x80000000, machine->sram.len, sram_get_func(&machine->sram));

    rvcore_init(&machine->core, bus_device_get_func(&machine->bus));
    machine->core.pc = 0x80000000;

    return (struct MachineFunc){
        .context = machine,
        .step    = test_machine_step,
        .check   = test_machine_check,
    };
}
