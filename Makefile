CC = g++
LD = g++
CFLAGS = -g -Wall -Wextra
TARGET = CAPCOM

INCLUDES += -I/usr/local/include/libbbbkit
LDFLAGS += -L/usr/local/lib -lbbbkit

BIN_DIR = ./bin
BUILD_DIR = ./build
SRC_DIR = ./src

SRC_LIST = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_LIST = $(SRC_LIST:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

mkdir_if_necessary = @mkdir -p $(@D)

all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJ_LIST)
	$(mkdir_if_necessary)
	$(LD) $(OBJ_LIST) $(LDFLAGS) -o $@

$(OBJ_LIST): $(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(mkdir_if_necessary)
	$(CC) $(CFLAGS) -c $(INCLUDES) $< -o $@

.PHONY: clean
clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR)

