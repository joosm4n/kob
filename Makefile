
CC := gcc
CCXX := g++

FLAGS := -ggdb -Wall -Wextra -pedantic -Wno-unused 
BUILD_DIR := build

$(BUILD_DIR)/kob_c: kob.c kob.h
	$(CC) $(FLAGS) kob.c -o $(BUILD_DIR)/kob_c

$(BUILD_DIR)/kob_cpp: kob.cpp kob.h
	$(CCXX) $(FLAGS) kob.cpp -o $(BUILD_DIR)/kob_cpp
