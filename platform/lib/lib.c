#include "debug.h"
#include "device/plic.h"
#include "machine/spike.h"
#include "string.h"

u8                  sram_data[128 * 1024 * 1024];
struct SpikeMachine machine;

struct RiscvCore *difftest_init(const u8 *data, u64 data_size) {
    INFO("difftest_init");
    memcpy(sram_data, data, data_size);
    INFO("load data size : %lld", data_size);
    spike_machine_init(&machine, (struct SpikePortableOperations){
                                     .sram_data = sram_data,
                                     .sram_size = 128 * 1024 * 1024,
                                     .get_char  = NULL,
                                     .put_char  = NULL,
                                 });

    return &machine.core;
}

void difftest_step(void) { spike_machine_step(&machine); }

void difftest_interrupt(const u64 ip) {
    machine.core.csrs[MIP] = ip;
    if ((ip == 0x200) | (ip == 0x800)) {
        plic_update_interrupt(&machine.plic, true, 1);
    }else{
        plic_update_interrupt(&machine.plic, false, 1);
    }
}
