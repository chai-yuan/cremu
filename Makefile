PROJECT_NAME = emu

SRCS_DIR = cremu
HEADS_DIR = cremu/include

SRCS = $(shell find $(SRCS_DIR) -name '*.c')
HEADS = $(shell find $(HEADS_DIR) -name '*.h')

PLATFORM ?= linux
PLATFORM_DIR = platform/$(PLATFORM)
include $(PLATFORM_DIR)/Makefile
