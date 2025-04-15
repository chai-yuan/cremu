#include "debug.h"
#include "linux_plat.h"
#include "machine/machine.h"
#include "machine/nemu.h"
#include "machine/spike.h"
#include "parse_args.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    Config config;
    parse_args(argc, argv, &config);
    init();

    size_t    binary_size;
    void     *binary_data = load_binary(config.bin, &binary_size);
    const u64 memory_size = 128 * 1024 * 1024;
    u8       *memory      = malloc(memory_size);
    memcpy(memory, binary_data, binary_size);
    free(binary_data);

    INFO("%s start!", config.machine);
    int                ret_val = 0;
    struct MachineFunc func    = {
           .context = NULL,
           .step    = NULL,
           .check   = NULL,
    };
    struct PortableOperations operations = {
        .sram_data = memory,
        .sram_size = memory_size,
        .get_char  = get_char,
        .put_char  = put_char,
    };

    if (strcmp(config.machine, "spike") == 0) {
        struct SpikeMachine *machine = malloc(sizeof(struct SpikeMachine));
        func                         = spike_machine_init(machine, operations);
    } else if (strcmp(config.machine, "nemu") == 0) {
        struct NemuMachine *machine = malloc(sizeof(struct NemuMachine));
        func                        = nemu_machine_init(machine, operations);
    }

    while ((func.check(func.context) == RUNNING) && config.step--) {
        func.step(func.context);
    }

    ret_val = func.check(func.context) == GOOD_END ? 0 : 1;
    ASSERT(ret_val == 0, "ret_val : %d", ret_val);
    return ret_val;
}
