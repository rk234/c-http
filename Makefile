CC = gcc
FLAGS = -g -Wall

SRC_DIR = ./src
BUILD_DIR = ./build
OUT_DIR = ./dist

main: server

server: $(BUILD_DIR)/main.o $(BUILD_DIR)/server.o
	$(CC) $(FLAGS) $(BUILD_DIR)/main.o $(BUILD_DIR)/server.o -o $(OUT_DIR)/server

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(FLAGS) -c $(SRC_DIR)/main.c -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/server.o: $(SRC_DIR)/server.c $(SRC_DIR)/server.h
	$(CC) $(FLAGS) -c $(SRC_DIR)/server.c -o $(BUILD_DIR)/server.o

setup:
	mkdir $(BUILD_DIR) $(OUT_DIR)

clean:
	rm $(BUILD_DIR)/* $(OUT_DIR)/*
