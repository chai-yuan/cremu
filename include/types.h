#ifndef TYPES_H
#define TYPES_H

#include "config.h"
#include "macro.h"

// 无符号整数类型
typedef unsigned char      u8;  // 8 位无符号整数
typedef unsigned short     u16; // 16 位无符号整数
typedef unsigned int       u32; // 32 位无符号整数
typedef unsigned long long u64; // 64 位无符号整数

// 有符号整数类型
typedef signed char      i8;  // 8 位有符号整数
typedef signed short     i16; // 16 位有符号整数
typedef signed int       i32; // 32 位有符号整数
typedef signed long long i64; // 64 位有符号整数

// 浮点数类型
typedef float  f32; // 32 位浮点数
typedef double f64; // 64 位浮点数

#define NULL ((void *)0)

#define INT32MIN 0x80000000
#define UINT32MAX 0xffffffff
#define INT64MIN 0x8000000000000000
#define UINT64MAX 0xffffffffffffffff

#if CURRENT_ARCH == ARCH_RV32
typedef u32 usize;
typedef i32 isize;
#define INTMIN INT32MIN
#elif CURRENT_ARCH == ARCH_RV64
typedef u64 usize;
typedef i64 isize;
#define INTMIN INT64MIN
#endif

// 布尔类型
#ifndef bool
#define bool unsigned char
#endif
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif
typedef bool bool_t; // 布尔类型的别名

// 指针大小类型
typedef unsigned long uptr; // 指针大小的无符号整数
typedef signed long   iptr; // 指针大小的有符号整数

#endif // TYPES_H
