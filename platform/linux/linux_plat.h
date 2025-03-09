#ifndef LINUX_PLAT_H
#define LINUX_PLAT_H

#include "types.h"
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

void *load_binary(const char *filename, size_t *size) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);

    void *data = malloc(*size);
    if (!data) {
        perror("Failed to allocate memory");
        fclose(file);
        return NULL;
    }

    if (fread(data, 1, *size, file) != *size) {
        perror("Failed to read file");
        free(data);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return data;
}

bool get_char(u8 *data) {
    char    c;
    ssize_t count = read(STDIN_FILENO, &c, 1);
    if (count > 0) {
        *data = (u8)c;
        return true;
    }
    return false;
}

void put_char(u8 data) {
    char    c     = (char)data;
    ssize_t count = write(STDOUT_FILENO, &c, 1);
    if (count < 0)
        exit(1);
}

static struct termios orig_termios;

void restore_terminal(void) { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); }

void handle_sigint(int sig) {
    restore_terminal();
    exit(EXIT_SUCCESS);
}

void init(void) {
    struct termios new_termios;

    // 保存原始终端设置
    tcgetattr(STDIN_FILENO, &orig_termios);
    new_termios = orig_termios;

    // 设置非规范模式并关闭回显
    new_termios.c_lflag &= ~(ICANON | ECHO);
    // 立即处理输入字符（最小字节数和超时）
    new_termios.c_cc[VMIN]  = 0;
    new_termios.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_termios);

    // 设置非阻塞模式
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    // 注册清理函数
    atexit(restore_terminal);
    signal(SIGINT, handle_sigint);
}

#endif
