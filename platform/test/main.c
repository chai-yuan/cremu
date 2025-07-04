#include "debug.h"
#include "machine/machine.h"
#include "machine/test.h"
#include "parse_args.h"
#include "utils.h"
#include <dlfcn.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct TestRiscvCore {
    usize     regs[32];
    usize     pc;
    usize     csrs[4096];
    enum mode mode;
};

struct TestRiscvCore *difftestCore;
struct TestRiscvCore *(*difftest_init)(const uint8_t *data, uint64_t data_size) = NULL;
void (*difftest_step)(void)                                                     = NULL;
void (*difftest_interrupt)(const uint64_t)                                      = NULL;

struct TestMachine *machine;

void difftestInit(const char *soFileName, const uint8_t *data, uint64_t data_size) {
    void *handle = dlopen(soFileName, RTLD_LAZY);
    if (!handle) {
        ERROR("Error loading library : %s", dlerror());
        exit(1);
    }

    difftest_init      = (struct TestRiscvCore * (*)(const uint8_t *, uint64_t))dlsym(handle, "difftest_init");
    difftest_step      = (void (*)(void))dlsym(handle, "difftest_step");
    difftest_interrupt = (void (*)(uint64_t))dlsym(handle, "difftest_interrupt");

    if (!difftest_init || !difftest_step || !difftest_interrupt) {
        ERROR("er");
        dlclose(handle);
        exit(1);
    }

    difftestCore = difftest_init(data, data_size);
}

void checkDiff(void) {
    int diff = 0;

    for (int i = 0; i < 32; i++) {
        if (difftestCore->regs[i] != machine->core.regs[i]) {
            printf("ERROR: %d mismatch\n", i);
            diff = 1;
        }
    }
    if (difftestCore->pc != machine->core.pc) {
        diff = 1;
    }

    if (diff == 1) {
        printf("pc : %x\n", difftestCore->pc);
        printf("mode : %x\n", difftestCore->mode);
        for (int i = 0; i < 32; i += 2) {
            printf("reg %d : %x reg %d : %x\n", i, difftestCore->regs[i], i + 1, difftestCore->regs[i + 1]);
        }

        printf("pc : %x\n", machine->core.pc);
        printf("mode : %x\n", machine->core.mode);
        for (int i = 0; i < 32; i += 2) {
            printf("reg %d : %x reg %d : %x\n", i, machine->core.regs[i], i + 1, machine->core.regs[i + 1]);
        }
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    Config config;
    parse_args(argc, argv, &config);

    size_t    binary_size;
    void     *binary_data = load_binary(config.bin, &binary_size);
    const u64 memory_size = 128 * 1024 * 1024;
    u8       *memory      = malloc(memory_size);
    memcpy(memory, binary_data, binary_size);
    difftestInit("/home/charain/Project/RISCV-Emulator/libcremu.so", binary_data, binary_size);
    free(binary_data);

    struct PortableOperations operations = {
        .sram_data          = memory,
        .sram_size          = memory_size,
        .get_char           = NULL,
        .put_char           = NULL,
        .update_framebuffer = NULL,
    };
    machine                 = malloc(sizeof(struct TestMachine));
    struct MachineFunc func = test_machine_init(machine, operations);

    // 执行
    while ((func.check(func.context) == RUNNING) && config.step--) {
        difftest_step();
        func.step(func.context);
        checkDiff();
    }

    int ret_val = func.check(func.context) == GOOD_END ? 0 : 1;
    ASSERT(ret_val == 0, "ret_val : %d", ret_val);

    return ret_val;
}
