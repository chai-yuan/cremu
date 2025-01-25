#include "device/uart.h"
#include "debug.h"
#include "device/device.h"
#include "types.h"

void uart_init(struct Uart *uart, struct InterruptFunc interrupt, get_char_func_t get,
               put_char_func_t put) {
    uart->get_char  = get;
    uart->put_char  = put;
    uart->interrupt = interrupt;
    uart->data[UART_LSR] |= (UART_LSR_TX_EMPTY | UART_LSR_THR_SR_EMPTY);
}

static enum exception uart_read(void *context, u64 addr, u8 size, usize *data) {
    struct Uart *uart = (struct Uart *)context;
    if (size != 1)
        return LOAD_ACCESS_FAULT;

    switch (addr) {
    case UART_RHR:
        *data = uart->data[UART_RHR];
        uart->data[UART_LSR] &= ~UART_LSR_RX_EMPTY;
        break;
    default:
        *data = uart->data[addr];
        break;
    }

    return EXC_NONE;
}

static enum exception uart_write(void *context, u64 addr, u8 size, usize data) {
    struct Uart *uart = (struct Uart *)context;
    if (size != 1)
        return LOAD_ACCESS_FAULT;

    switch (addr) {
    case UART_THR:
        if (uart->put_char)
            uart->put_char((u8)data);
        uart->data[UART_LSR] |= UART_LSR_TX_EMPTY;
        break;
    default:
        uart->data[addr] = (u8)data;
        break;
    }

    return EXC_NONE;
}

static void uart_update(void *context, u32 interval) {
    struct Uart *uart = (struct Uart *)context;
    uart->last_update += interval;
    if (!uart->get_char || uart->last_update < 10000)
        return;
    uart->last_update = 0;

    u8 input_char;
    if (uart->get_char(&input_char)) {
        if (!(uart->data[UART_LSR] & UART_LSR_RX_EMPTY)) {
            uart->data[UART_RHR] = input_char;
            uart->data[UART_LSR] |= UART_LSR_RX_EMPTY;
            uart->interrupt.raise_irq(uart->interrupt.context, uart->interrupt.interrupt_num);
        }
    }
}

struct DeviceFunc uart_get_func(struct Uart *uart) {
    return (struct DeviceFunc){
        .context = uart,
        .read    = uart_read,
        .write   = uart_write,
        .update  = uart_update,
    };
}
