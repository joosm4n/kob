
CC := gcc
C_STD := -std=c99

CCXX := g++
CXX_STD := -std=c++11

FLAGS := -ggdb -Wall -Wextra -Wno-unused-parameter
RELEASE_FLAGS := -O3 -Wall -Wextra -Wno-unused-parameter

BUILD_DIR := ./build
EXAMPLE := kob_example

.PHONY: c cpp release

c: $(BUILD_DIR)/kob_c
cpp: $(BUILD_DIR)/kob_cpp
c-release: $(BUILD_DIR)/kob_c_release
cpp-release: $(BUILD_DIR)/kob_cpp_release

default: c
build:
	mkdir -p build

$(BUILD_DIR)/kob_c: $(EXAMPLE).c kob.h build
	$(CC) $(FLAGS) $(C_STD) $(EXAMPLE).c -o $(BUILD_DIR)/kob_c

$(BUILD_DIR)/kob_cpp: $(EXAMPLE).cpp kob.h build
	$(CCXX) $(FLAGS) $(CXX_STD) $(EXAMPLE).cpp -o $(BUILD_DIR)/kob_cpp

$(BUILD_DIR)/kob_c_release: $(EXAMPLE).c kob.h build
	$(CC) $(RELEASE_FLAGS) $(C_STD) $(EXAMPLE).c -o $(BUILD_DIR)/kob_c_release

$(BUILD_DIR)/kob_cpp_release: $(EXAMPLE).cpp kob.h build
	$(CCXX) $(RELEASE_FLAGS) $(CXX_STD) $(EXAMPLE).cpp -o $(BUILD_DIR)/kob_cpp_release


