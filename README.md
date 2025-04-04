# cremu - 轻量级RISC-V模拟器 🚀

[![GitHub Stars](https://img.shields.io/github/stars/chai-yuan/cremu?style=flat-square)](https://github.com/chai-yuan/cremu)
[![License](https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square)](LICENSE)

**cremu** 是一个用C99实现的轻量级RISC-V模拟器，设计注重易学性和可移植性。它支持多种RISC-V特性和操作系统，是学习计算机体系结构和系统模拟的理想工具。

👉 **[立即体验WebAssembly版本](https://chai-yuan.github.io/cremu/)** 👈

## ✨ 主要特性

- **指令集支持**：
  - RV32IMAC & RV64IAMC
- **特权级支持**：
  - Machine (M)、Supervisor (S) 和 User (U) 模式
- **虚拟内存**：
  - SV32 (RV32) & SV39 (RV64)
- **设备模拟**：
  - 串口设备 (UART16550)
  - 中断控制器 (PLIC & CLINT)
  
## 🏗️ 构建指南

```bash
# 选择目标平台进行构建 (linux/lib/web)
make PLATFORM=linux  # 默认构建Linux版本
```

## 🚀 快速开始

### Linux平台使用

```bash
# 运行模拟器
./cremu --bin fw_payload.bin --machine [machine_type]

# 获取帮助
./cremu --help

Usage: ./cremu [OPTIONS]
Options:
  --bin             Specify the binary path
  --machine         Specify the machine type
  --step            Specify the step
  --help            Display this help message
```

## 🐧 支持运行的软件

- OpenSBI v1.6 (RV32 & RV64)
- Linux 6.12
- xv6

## 📦 预构建测试文件

我们提供了可直接运行的测试文件，你可以在[Release页面](https://github.com/yourusername/cremu/releases)下载。

## ⚙️ 自定义配置

通过修改 `include/config.h` 文件，你可以调整模拟器的各种编译选项：

```c
// 调试设置
#define ENABLE_DEBUG_MACROS 1
#define DEBUG_LEVEL DEBUG_LEVEL_INFO
// 处理器拓展
#define CURRENT_ARCH ARCH_RV32
#define C_EXTENSION 1
#define SVADU_EXTENSION 1
```

## 🌍 支持的平台

| 平台 | 状态 | 备注 |
|------|------|------|
| Linux | ✅ 完全支持 | 推荐平台 |
| WebAssembly | ✅ 支持 | 可在浏览器中运行 |

## 📜 许可证

本项目采用MIT许可证 - 详情见[LICENSE](LICENSE)文件。
