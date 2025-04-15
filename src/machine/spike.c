#include "machine/spike.h"
#include "machine/machine.h"

u32 boot_rom[0x1000] = {
    0x00000297, // auipc t0, 0x0: 将当前PC值的高20位与立即数0相加，结果存入t0
    0x02028593, // addi a1, t0, 32: 将t0的值与立即数32相加，结果存入a1，即设备树地址
    0xf1402573, // csrr a0, mhartid: 读取mhartid寄存器的值，存入a0
#if CURRENT_ARCH == ARCH_RV32
    0x0182a283, // lw t0, 24(t0): 从内存地址t0 + 24处加载一个字到t0
#elif CURRENT_ARCH == ARCH_RV64
    0x0182b283, // ld t0, 24(t0): 从内存地址t0 + 24处加载一个字到t0
#endif
    0x00028067, // jr t0: 跳转到t0寄存器中的地址
    0x00000000, // 未使用（填充）
    0x80000000, // 0x1018处的值：复位目标地址
    0x00000000,
};

void spike_machine_step(void *context) {
    struct SpikeMachine *machine = context;

    plic_update_interrupt(&machine->plic, uart_check_irq(&machine->uart), 1);

    rvcore_step(&machine->core, (struct RiscvEnvInfo){.meint = false,
                                                      .seint = plic_check_irq(&machine->plic, 1),
                                                      .mtint = clint_check_irq(&machine->clint),
                                                      .time  = clint_get_time(&machine->clint)});

    struct DeviceFunc bus = bus_device_get_func(&machine->bus);
    bus.update(bus.context, 1);
}

enum MachineCode spike_machine_check(void *context) {
    struct SpikeMachine *machine = context;

    if (machine->core.decode.exception == BREAKPOINT) {
        return machine->core.regs[10] == 0 ? GOOD_END : BAD_END;
    }
    return RUNNING;
}

struct MachineFunc spike_machine_init(struct SpikeMachine *machine, struct PortableOperations init) {
    bus_device_init(&machine->bus);
    sram_init(&machine->sram, init.sram_data, init.sram_size);
    sram_init(&machine->rom, (u8 *)boot_rom, sizeof(boot_rom));
    uart_init(&machine->uart, init.get_char, init.put_char);
    plic_init(&machine->plic);
    clint_init(&machine->clint);

    bus_device_add(&machine->bus, 0x80000000, machine->sram.len, sram_get_func(&machine->sram));
    bus_device_add(&machine->bus, 0x10000000, UART_SIZE, uart_get_func(&machine->uart));
    bus_device_add(&machine->bus, 0x0C000000, PLIC_SIZE, plic_get_func(&machine->plic));
    bus_device_add(&machine->bus, 0x02000000, CLINT_SIZE, clint_get_func(&machine->clint));
    bus_device_add(&machine->bus, 0x00001000, machine->rom.len, sram_get_func(&machine->rom));

    rvcore_init(&machine->core, bus_device_get_func(&machine->bus));

    // 将设备树文件写入rom中
    const unsigned char *dtb      = IS_RV64(spike_rv64_dtb, spike_rv32_dtb);
    unsigned int         dtb_size = IS_RV64(spike_rv64_dtb_size, spike_rv32_dtb_size);

    u8 *boot_rom_p = (u8 *)(&boot_rom[8]);
    for (int i = 0; i < dtb_size; i++)
        boot_rom_p[i] = dtb[i];

    return (struct MachineFunc){
        .context = machine,
        .step    = spike_machine_step,
        .check   = spike_machine_check,
    };
}
