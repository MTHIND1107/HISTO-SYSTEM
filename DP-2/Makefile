CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -I$(INC_DIR) -I../common/inc
LDFLAGS =

SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj
BIN_DIR = bin
COMMON_OBJ_DIR = ../common/obj

TARGET = $(BIN_DIR)/DP-2
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
COMMON_OBJECTS = $(wildcard $(COMMON_OBJ_DIR)/*.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS) $(COMMON_OBJECTS) | $(BIN_DIR)
	$(CC) $(OBJECTS) $(COMMON_OBJECTS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)