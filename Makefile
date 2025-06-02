CC = gcc
FLAGS = -g -Wall -Werror

SRC_DIR = ./src
BUILD_DIR = ./build
OUT_DIR = ./dist

main: server

server: $(BUILD_DIR)/main.o $(BUILD_DIR)/server.o $(BUILD_DIR)/conn_handler.o $(BUILD_DIR)/request.o $(BUILD_DIR)/response.o
	$(CC) $(FLAGS) $(BUILD_DIR)/main.o \
		$(BUILD_DIR)/server.o $(BUILD_DIR)/conn_handler.o \
		$(BUILD_DIR)/request.o $(BUILD_DIR)/response.o -o $(OUT_DIR)/chttp

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(FLAGS) -c $(SRC_DIR)/main.c -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/server.o: $(SRC_DIR)/server.c $(SRC_DIR)/server.h
	$(CC) $(FLAGS) -c $(SRC_DIR)/server.c -o $(BUILD_DIR)/server.o

$(BUILD_DIR)/conn_handler.o: $(SRC_DIR)/conn_handler.c $(SRC_DIR)/conn_handler.h
	$(CC) $(FLAGS) -c $(SRC_DIR)/conn_handler.c -o $(BUILD_DIR)/conn_handler.o

$(BUILD_DIR)/request.o: $(SRC_DIR)/request.c $(SRC_DIR)/request.h
	$(CC) $(FLAGS) -c $(SRC_DIR)/request.c -o $(BUILD_DIR)/request.o

$(BUILD_DIR)/response.o: $(SRC_DIR)/response.c $(SRC_DIR)/response.h
	$(CC) $(FLAGS) -c $(SRC_DIR)/response.c -o $(BUILD_DIR)/response.o

setup:
	mkdir -p $(BUILD_DIR) $(OUT_DIR)

clean:
	rm $(BUILD_DIR)/* $(OUT_DIR)/*
