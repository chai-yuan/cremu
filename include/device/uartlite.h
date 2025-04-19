#ifndef UART_lite_H
#define UART_lite_H

#include "device/device.h"
#include "types.h"
#include "uart16550.h"

#define UART_lite_SIZE (0x4 * 4)
#define UART_RX (0x0)
#define UART_TX (0x4)
#define UART_STAT (0x8)
#define UART_CTRL (0xc)
// 线路状态寄存器标志位

struct UARTLite {
    u32 regs[4];
    u64 last_update;

    get_char_func_t get_char; // 外部提供的输入函数
    put_char_func_t put_char; // 外部提供的输出函数
};

void uartlite_init(struct UARTLite *uart, get_char_func_t get, put_char_func_t put);

bool uartlite_check_irq(struct UARTLite *uart);

struct DeviceFunc uartlite_get_func(struct UARTLite *uart);

#endif
