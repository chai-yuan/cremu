#ifndef LINUX_PLAT_H
#define LINUX_PLAT_H

#include "SDL2/SDL.h"
#include "types.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
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

static SDL_Window   *window   = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture  *texture  = NULL;

void init_sdl(int width, int height) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "cremu");
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);
}
void update_framebuffer(uint32_t *data, int width, int height) {
    SDL_UpdateTexture(texture, NULL, data, width * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
void cleanup_sdl(void) {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
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

    // 初始化显示窗口
    init_sdl(640, 480);
}

#endif
