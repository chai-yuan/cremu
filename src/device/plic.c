#include "device/plic.h"
#include "core/riscv.h"
#include "debug.h"

void plic_init(struct PLIC *plic) {
    for (int i = 0; i < sizeof(struct PLIC); i++)
        *((u8 *)plic + i) = 0;
}

bool plic_check_irq(struct PLIC *plic, u32 context_id) {
    if (context_id >= CONTEXT_NUM) {
        return false;
    }

    // 检查是否有任何中断等待处理
    for (int i = 0; i < (SOURCE_NUM + 32) / 32; i++) {
        // 当前挂起且启用的中断
        //   u32 pending_enabled = plic->pending[i] & plic->enable[context_id][i];
        u32 pending_enabled = plic->pending[i];

        if (pending_enabled) {
            // 查找优先级高于阈值的中断
            for (int bit = 0; bit < 32; bit++) {
                u32 irq = bit + i * 32;

                if (irq == 0)
                    continue; // 0是无效中断源

                if ((pending_enabled & (1u << bit)) && plic->priority[irq] > plic->threshold[context_id]) {
                    plic->claim[context_id] = irq;
                    return true;
                }
            }
        }
    }
    return false;
}

void plic_update_interrupt(struct PLIC *plic, bool interrupt, u32 interrupt_num) {
    if (interrupt_num == 0 || interrupt_num > SOURCE_NUM) {
        return; // 忽略无效的中断号
    }

    u32 index = interrupt_num / 32;
    u32 bit   = interrupt_num % 32;

    if (interrupt) {
        // 设置中断挂起位
        plic->pending[index] |= (1u << bit);
    } else {
        // 清除中断挂起位
        plic->pending[index] &= ~(1u << bit);
    }
}

static enum exception plic_read(void *context, u64 addr, u8 size, usize *data) {
    struct PLIC *plic = (struct PLIC *)context;
    if (size != 4 || addr + size > PLIC_SIZE)
        return LOAD_ACCESS_FAULT;

    *data = 0;

    // 优先级寄存器
    if (addr >= PLIC_PRIORITY && addr < PLIC_PRIORITY + 4 * (SOURCE_NUM + 1)) {
        u32 source = (addr - PLIC_PRIORITY) / 4;
        *data      = plic->priority[source];
        return EXC_NONE;
    }

    // 挂起位寄存器
    if (addr >= PLIC_PENDING && addr < PLIC_PENDING + 4 * ((SOURCE_NUM + 31) / 32)) {
        u32 reg_idx = (addr - PLIC_PENDING) / 4;
        *data       = plic->pending[reg_idx];
        return EXC_NONE;
    }

    // 中断使能寄存器
    if (addr >= PLIC_ENABLE && addr < PLIC_ENABLE + 4 * ((SOURCE_NUM + 31) / 32)) {
        u32 reg_idx     = (addr - PLIC_ENABLE) / 4;
        u32 context_idx = (addr - PLIC_ENABLE) / 0x80;
        *data           = plic->enable[context_idx][reg_idx];
        return EXC_NONE;
    }

    // 优先级阈值
    if ((addr & 0xf) == 0x0) {
        u32 context_idx = (addr & 0xffff) / 0x1000;
        *data           = plic->threshold[context_idx]; // 通常3位优先级
        return EXC_NONE;
    }

    // 完成寄存器
    if ((addr & 0xf) == 0x4) {
        u32 context_idx = (addr & 0xffff) / 0x1000;
        *data           = plic->claim[context_idx];
        return EXC_NONE;
    }

    return EXC_NONE;
}

static enum exception plic_write(void *context, u64 addr, u8 size, usize data) {
    struct PLIC *plic = (struct PLIC *)context;
    if (size != 4 || addr + size > PLIC_SIZE)
        return STORE_AMO_ACCESS_FAULT;

    // 优先级寄存器
    if (addr >= PLIC_PRIORITY && addr < PLIC_PRIORITY + 4 * (SOURCE_NUM + 1)) {
        u32 source = (addr - PLIC_PRIORITY) / 4;
        if (source == 0) {
            // 源0是保留的，不能写入
            return EXC_NONE;
        }
        plic->priority[source] = data & 0x7; // 通常PLIC支持3位优先级
        return EXC_NONE;
    }

    // 挂起位寄存器 (通常只读，但某些实现可能支持通过写1来清除)
    if (addr >= PLIC_PENDING && addr < PLIC_PENDING + 4 * ((SOURCE_NUM + 31) / 32)) {
        // 忽略写入挂起寄存器的操作
        return EXC_NONE;
    }

    // 中断使能寄存器
    if (addr >= PLIC_ENABLE && addr < PLIC_ENABLE + 4 * ((SOURCE_NUM + 31) / 32)) {
        u32 reg_idx     = (addr - PLIC_ENABLE) / 4;
        u32 context_idx = (addr - PLIC_ENABLE) / 0x80;
        // 源0是无效的，所以在第一个寄存器中屏蔽位0
        if (reg_idx == 0) {
            plic->enable[context_idx][0] = data & ~1u;
        } else {
            plic->enable[context_idx][reg_idx] = data;
        }
        return EXC_NONE;
    }

    // 优先级阈值
    if ((addr & 0xf) == 0x0) {
        u32 context_idx              = (addr & 0xffff) / 0x1000;
        plic->threshold[context_idx] = data & 0x7; // 通常3位优先级
        return EXC_NONE;
    }

    // 完成寄存器
    if ((addr & 0xf) == 0x4) {
        u32 context_idx          = (addr & 0xffff) / 0x1000;
        plic->claim[context_idx] = 0;
        return EXC_NONE;
    }

    return EXC_NONE;
}

struct DeviceFunc plic_get_func(struct PLIC *plic) {
    return (struct DeviceFunc){
        .context = plic,
        .read    = plic_read,
        .write   = plic_write,
        .update  = NULL,
    };
}
