CC = cc
CFLAGS = -Wall -Wextra -Werror -std=c89

SRC_DIR = src
INC_DIR = include
LIB_DIR = lib
OBJ_DIR = obj

LIB_NAME = kitsune
LIB_VERSION = 0.1.0

SRCS = $(shell find $(SRC_DIR) -type f -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
LIB = $(LIB_DIR)/lib$(LIB_NAME).a

TEST_SOURCES = $(wildcard tests/*.c)
TEST_TARGETS := $(patsubst tests/%.c,%,$(TEST_SOURCES))
TEST_BIN_DIR := bin/tests

all: $(LIB)
	@echo $(TEST_TARGETS)

test: $(addprefix $(TEST_BIN_DIR)/,$(TEST_TARGETS))

$(LIB): $(OBJS) | $(LIB_DIR)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(TEST_BIN_DIR)/%: tests/%.c
	@mkdir -p $(TEST_BIN_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -Llib -lkitsune $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR)

.PHONY: all test clean
