CC = clang
CFLAGS = -Wno-unused-command-line-argument -O0 -g -Wall -Wextra -Werror -std=c89 -march=native

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), FreeBSD)
	CFLAGS += -lexecinfo
else ifeq ($(UNAME_S), OpenBSD)
	CFLAGS += -lexecinfo
else ifeq ($(UNAME_S), NetBSD)
	CFLAGS += -lexecinfo
endif

ifdef ENABLE_RT
	CFLAGS += -DENABLE_RT
endif

SRC_DIR = src
INC_DIR = include
LIB_DIR = lib
OBJ_DIR = obj
DESTDIR ?= /usr/local

LIB_NAME = kitsune
LIB_VERSION = 0.1.0

SRCS = $(shell find $(SRC_DIR) -type f -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
LIB = $(LIB_DIR)/lib$(LIB_NAME).a

TEST_SOURCES = $(wildcard tests/*.c)
TEST_TARGETS := $(patsubst tests/%.c,%,$(TEST_SOURCES))
TEST_BIN_DIR := bin/tests

all: $(LIB)

test: $(addprefix $(TEST_BIN_DIR)/,$(TEST_TARGETS))

$(LIB): $(OBJS) | $(LIB_DIR)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(TEST_BIN_DIR)/%: tests/%.c
	@mkdir -p $(TEST_BIN_DIR)
	$(CC) $(CFLAGS) -Wl,-export-dynamic -I$(INC_DIR) -Llib -lkitsune $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR) $(TEST_BIN_DIR)

install: all
	install -d $(DESTDIR)/lib
	cp lib/libkitsune.a $(DESTDIR)/lib/
	install -m 755 -d $(DESTDIR)/include/kitsune
	cp -r include/* $(DESTDIR)/include/kitsune/

.PHONY: all test clean
