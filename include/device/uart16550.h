#ifndef UART_16550_H
#define UART_16550_H

#include "device/device.h"
#include "types.h"

// 输入输出函数，返回值表示是否成功(是否读取到数据)
typedef bool (*get_char_func_t)(u8 *data);
typedef void (*put_char_func_t)(u8 data);

#define UART_SIZE (0x8)
#define UART_THR (0) // TX
#define UART_RHR (0) // RX 这里直接使用RX充当接收缓冲区
#define UART_IER (1)
#define UART_IIR (2)
#define UART_FCR (2)
#define UART_LCR (3)
#define UART_LSR (5)
#define UART_MSR (6)
#define UART_SCR (7)
// 线路状态寄存器标志位
#define UART_IER_RDA (1)
#define UART_IER_THRE (1 << 1)
#define UART_FCR_CLEAR (1 << 1)
#define UART_LCR_DIVREG (1 << 7)
#define UART_LSR_DATA_READY (1 << 0)
#define UART_LSR_TX_EMPTY (1 << 5)
#define UART_LSR_THR_SR_EMPTY (1 << 6)

struct UART16550 {
    u8  data[UART_SIZE]; // 模拟 UART 寄存器
    u8  divregs[2];      // 除数寄存器
    u64 last_update;

    get_char_func_t get_char; // 外部提供的输入函数
    put_char_func_t put_char; // 外部提供的输出函数
};

void uart_init(struct UART16550 *uart, get_char_func_t get, put_char_func_t put);

bool uart_check_irq(struct UART16550 *uart);

struct DeviceFunc uart_get_func(struct UART16550 *uart);

#endif
