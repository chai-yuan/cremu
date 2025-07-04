SRCS += $(wildcard $(PLATFORM_DIR)/*.c)
OBJS = $(patsubst %.c, build/%.o, $(SRCS))

CC = gcc
CFLAGS = -Wall -O2 -I$(HEADS_DIR)
CFLAGS += `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`

ARGS ?= 

$(PROJECT_NAME): $(OBJS)
	@echo "[LINK] Linking final executable: $@"
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

build/%.o: %.c $(HEADS)
	@echo "[CC] Compiling $< -> $@"
	@mkdir -p $(dir $@)  
	@$(CC) $(CFLAGS) -c $< -o $@

run: $(PROJECT_NAME)
	@echo "[RUN] Running the program: ./$(PROJECT_NAME) $(ARGS)"
	@./$(PROJECT_NAME) $(ARGS)

clean:
	@echo "[CLEAN] Cleaning up build artifacts"
	@rm -f $(PROJECT_NAME) $(OBJS)
	@rm -rf build

.PHONY: clean run
