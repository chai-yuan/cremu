PROJECT_NAME = cremu
SRCS_DIR = src
HEADS_DIR = include
BUILD_DIR = build

SRCS = $(shell find $(SRCS_DIR) -name '*.c')
HEADS = $(shell find $(HEADS_DIR) -name '*.h')
HEADS += $(shell find $(SRCS_DIR) -name '*.h')

PLATFORM ?= linux
PLATFORM_DIR = platform/$(PLATFORM)
include $(PLATFORM_DIR)/Makefile.$(PLATFORM)
