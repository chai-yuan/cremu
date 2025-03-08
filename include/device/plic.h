#ifndef PLIC_H
#define PLIC_H

#include "device/device.h"
#include "types.h"

// 默认32个中断源，4个上下文(双核)
#define SOURCE_NUM 32
#define CONTEXT_NUM 4

#define PLIC_SIZE (0x4000000)
#define PLIC_PRIORITY (0x0)
#define PLIC_PENDING (0x1000)
#define PLIC_ENABLE (0x2000)
#define PLIC_THRESHOLD (0x200000)
#define PLIC_CLAIM (0x200004)

struct PLIC {
    u32 priority[SOURCE_NUM + 1];
    u32 pending[(SOURCE_NUM + 32) / 32];
    u32 enable[CONTEXT_NUM][(SOURCE_NUM + 32) / 32];
    u32 threshold[CONTEXT_NUM];
    u32 claim[CONTEXT_NUM];
};

void plic_init(struct PLIC *plic);

void plic_update_interrupt(struct PLIC *plic, bool interrupt, u32 interrupt_num);

bool plic_check_irq(struct PLIC *plic, u32 context_id);

struct DeviceFunc plic_get_func(struct PLIC *plic);

#endif
