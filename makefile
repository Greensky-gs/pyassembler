CC=gcc
FLAGS=-Wall -Wextra
SANTS=-fsanitize=address,undefined

SRC_DIR=src
BUILD_DIR=build
BIN_DIR=bin

NAME=main.uwu

SOURCES_REG=$(wildcard $(SRC_DIR)/*.c)
SOURCES_DIR=$(wildcard $(SRC_DIR)/*/)
SOURCES_IND=$(wildcard $(SRC_DIR)/*/*.c)

SOURCES=$(SOURCES_REG) $(SOURCES_IND)

BUILD_DIRS=$(SOURCES_DIR:$(SRC_DIR)/%/=$(BUILD_DIR)/%/)
OBJECTS=$(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

DISABLE_VERBOSE?=0
QUIET=0
ifeq ($(DISABLE_VERBOSE),1)
	QUIET=-DDISABLE_VERBOSE=1
else
	QUIET=
endif

all: $(BUILD_DIR) $(BUILD_DIRS) $(BIN_DIR)/$(NAME)

$(BIN_DIR)/$(NAME): $(BIN_DIR) $(OBJECTS)
	$(CC) $(OBJECTS) $(FLAGS) $(QUIET) $(SANTS) -o $(BIN_DIR)/$(NAME)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(FLAGS) $^ -o $@ -c $(QUIET)

$(BUILD_DIRS): $(BUILD_DIR)
	mkdir -p "$@"

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(BIN_DIR)

launch:
	./$(BIN_DIR)/$(NAME)

cleanbuild:
	make FLAGS= DISABLE_VERBOSE=$(DISABLE_VERBOSE)

full:
	make clean
	make
	make launch
