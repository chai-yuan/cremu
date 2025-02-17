#ifndef LINUX_PLAT_H
#define LINUX_PLAT_H

#include "types.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
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
    char c;
    if (read(STDIN_FILENO, &c, 1) > 0) {
        *data = (u8)c;
        return true;
    }
    return false;
}

void put_char(u8 data) {
    char c = (char)data;
    printf("%c", c);
    fflush(stdout);
}

void init(void) {
    // 设置 STDIN_FILENO 为非阻塞模式
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

#endif
