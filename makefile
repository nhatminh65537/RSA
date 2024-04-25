CC = gcc
FLAGS = -g

BUILD_DIR = ./build
SRC_DIR = ./source
HDR_DIR = ./header

# SRCS = $(shell dir $(SRC_DIR))
SRCS = main.c cmd.c int256.c prgvar.c ui.c uilib.c
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS = $(patsubst %,$(HDR_DIR)/%,$(shell dir $(HDR_DIR)))

all: main launch
	@echo $(DEPS)

main: $(OBJS)
	$(CC) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	mkdir -p $(dir $@)
	$(CC) $(FLAGS) -c $< -o $@

launch: $(BUILD_DIR)/launch.o
	$(CC) -o $@ $^

$(BUILD_DIR)\launch.o: $(SRC_DIR)/launch.c
	mkdir -p $(dir $@)
	$(CC) $(FLAGS) -c $< -o $@

# .PHONY: clean
# clean:
# 	rm -r $(BUILD_DIR)
