#ifndef PLIC_H
#define PLIC_H

#include "device/device.h"
#include "types.h"

#define PLIC_SIZE (0x4000000)
#define PLIC_PRIORITY (0x0)
#define PLIC_PENDING (0x1000)
#define PLIC_MENABLE (0x2000)
#define PLIC_SENABLE (0x2080)
#define PLIC_MPRIORITY (0x200000)
#define PLIC_SPRIORITY (0x201000)
#define PLIC_MCLAIM (0x200004)
#define PLIC_SCLAIM (0x201004)

struct PLIC {
    u32 priority;
    u32 pending;
    u32 menable;
    u32 senable;
    u32 mpriority;
    u32 spriority;
    u32 mclaim;
    u32 sclaim;
};

void plic_init(struct PLIC *plic);

void plic_update_intterupt(struct PLIC *plic, bool interrupt, u32 interrupt_num);

bool plic_check_irq(struct PLIC *plic, u32 context_id);

struct DeviceFunc plic_get_func(struct PLIC *plic);

#endif
