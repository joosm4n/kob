
CC := gcc
C_STD := -std=gnu17

CCXX := g++
CXX_STD := -std=gnu++17

FLAGS := -ggdb -Wall -Wextra -pedantic -Wno-unused
BUILD_DIR := ./build
EXAMPLE := kob_example

.PHONY: c cpp

c: $(BUILD_DIR)/kob_c
cpp: $(BUILD_DIR)/kob_cpp

default: c
build:
	mkdir -p build

$(BUILD_DIR)/kob_c: $(EXAMPLE).c kob.h build
	$(CC) $(FLAGS) $(C_STD) $(EXAMPLE).c -o $(BUILD_DIR)/kob_c

$(BUILD_DIR)/kob_cpp: $(EXAMPLE).cpp kob.h build
	$(CCXX) $(FLAGS) $(CXX_STD) $(EXAMPLE).cpp -o $(BUILD_DIR)/kob_cpp
