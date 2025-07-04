PROJECT_NAME = cremu

SRCS_DIR = emu
HEADS_DIR = emu/include

SRCS = $(shell find $(SRCS_DIR) -name '*.c')
HEADS = $(shell find $(SRCS_DIR) -name '*.h')

PLATFORM ?= linux
PLATFORM_DIR = platform/$(PLATFORM)
include $(PLATFORM_DIR)/Makefile
