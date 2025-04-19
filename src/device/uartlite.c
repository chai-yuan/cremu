#include "device/uartlite.h"

void uartlite_init(struct UARTLite *uart, get_char_func_t get, put_char_func_t put) {
    uart->get_char = get;
    uart->put_char = put;
}

static enum exception uart_read(void *context, u64 addr, u8 size, usize *data) {
    struct UARTLite *uart = (struct UARTLite *)context;
    if (addr > UART_lite_SIZE)
        return LOAD_ACCESS_FAULT;

    switch (addr) {
    case UART_RX:
        *data         = uart->regs[0];
        uart->regs[0] = 0;
        uart->regs[2] &= ~0x3; // 接收FIFO无数据
        break;
    case UART_STAT:
        *data = uart->regs[2];
        break;
    default:
        *data = 0;
        break;
    }

    return EXC_NONE;
}

static enum exception uart_write(void *context, u64 addr, u8 size, usize data) {
    struct UARTLite *uart = (struct UARTLite *)context;
    if (addr > UART_lite_SIZE)
        return STORE_AMO_ACCESS_FAULT;

    switch (addr) {
    case UART_TX:
        uart->regs[1] = data;
        if (uart->put_char != NULL) {
            uart->put_char(data);
        }
        break;
    case UART_CTRL:
        uart->regs[3] = data;
        if (data & (1 << 1))
            uart->regs[0] = 0; // 清空接受FIFO
    default:
        break;
    }

    return EXC_NONE;
}

static void uart_update(void *context, u32 interval) {
    struct UARTLite *uart = (struct UARTLite *)context;

    // 检查输入
    uart->last_update += interval;
    if (!uart->get_char || uart->last_update < 50000)
        return;
    uart->last_update = 0;

    if (uart->get_char != NULL) {
        u8 data;
        if (uart->get_char(&data)) {
            uart->regs[0] = data;
            uart->regs[2] |= 0x3; // 接收FIFO有数据
        }
    }
}

bool uartlite_check_irq(struct UARTLite *uart) { return false; }

struct DeviceFunc uartlite_get_func(struct UARTLite *uart) {
    return (struct DeviceFunc){
        .context = uart,
        .read    = uart_read,
        .write   = uart_write,
        .update  = uart_update,
    };
}
