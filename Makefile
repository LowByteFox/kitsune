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

SRC_DIR = src
INC_DIR = include
LIB_DIR = lib
OBJ_DIR = obj
DESTDIR ?= /usr/local

LIB_NAME = kitsune
LIB_VERSION = 0.1.0

SRCS = $(shell find $(SRC_DIR)/kitsune -type f -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/kitsune/%.c,$(OBJ_DIR)/kitsune/%.o,$(SRCS))
LIB = $(LIB_DIR)/lib$(LIB_NAME).a
LIBRT = $(LIB_DIR)/lib$(LIB_NAME)_rt.a

TEST_SOURCES = $(wildcard tests/*.c)
TEST_TARGETS := $(patsubst tests/%.c,%,$(TEST_SOURCES))
TEST_BIN_DIR := bin/tests

all: $(LIB) $(LIBRT)

test: $(addprefix $(TEST_BIN_DIR)/,$(TEST_TARGETS))

$(LIB): $(OBJS) | $(LIB_DIR)
	ar rcs $@ $^

$(LIBRT): $(OBJ_DIR)/kitsune_rt/runtime.o
	ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(TEST_BIN_DIR)/%: tests/%.c
	@mkdir -p $(TEST_BIN_DIR)
	if [ "$$(basename $@)" = "coro" ]; then \
		$(CC) $(CFLAGS) -Wl,-export-dynamic -I$(INC_DIR) -Llib -lkitsune -lkitsune_rt $< -o $@; \
	else\
		$(CC) $(CFLAGS) -Wl,-export-dynamic -I$(INC_DIR) -Llib -lkitsune $< -o $@; \
	fi;

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR) $(TEST_BIN_DIR) *.core _install

install: all
	install -d $(DESTDIR)/lib
	cp -r lib/ $(DESTDIR)/lib/
	install -m 755 -d $(DESTDIR)/include/kitsune
	cp -r include/kitsune $(DESTDIR)/include/

.PHONY: all test clean
