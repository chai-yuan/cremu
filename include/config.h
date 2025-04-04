#ifndef CONFIG_H
#define CONFIG_H

#define ARCH_RV32 32
#define ARCH_RV64 64

#define ENABLE_DEBUG_MACROS 1
#define DEBUG_LEVEL DEBUG_LEVEL_INFO

// 处理器拓展
#define CURRENT_ARCH ARCH_RV32
#define C_EXTENSION 1
#define SVADU_EXTENSION 1

#ifndef CURRENT_ARCH
#define CURRENT_ARCH ARCH_RV64
#elif CURRENT_ARCH != ARCH_RV32 && CURRENT_ARCH != ARCH_RV64
#error "Unsupported architecture."
#endif

#endif
