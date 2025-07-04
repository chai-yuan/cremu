#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

#include <getopt.h>
#include <stdint.h>

typedef struct {
    char    *bin;
    char    *machine;
    uint64_t step;
} Config;

typedef struct {
    const char *name;    // 长选项名称
    int         has_arg; // 是否需要参数
    int        *flag;    // 标志位
    int         val;     // 选项枚举值
    const char *help;    // 帮助信息
} OptionInfo;

int parse_args(int argc, char *argv[], Config *config);

#endif
