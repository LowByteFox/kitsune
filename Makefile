CC = gcc
AR = ar
CFLAGS = -Wall -Wextra -std=c89 -D_GNU_SOURCE -Iinclude
LDFLAGS = 
INCLDIR = include
SRCDIR = src
OBJDIR = obj
DESTDIR ?= /usr/local

SRCS = src/kitsune/dynamic_iterator.c src/kitsune/crashtrace.c src/kitsune/strutils.c src/kitsune/hashes.c src/kitsune/coroutines.c src/kitsune/list.c src/kitsune/stack.c src/kitsune/allocator.c src/kitsune/alloc/basic.c src/kitsune/alloc/traced.c src/kitsune/alloc/hardened.c src/kitsune/queue.c src/kitsune/rc.c src/kitsune/generator.c src/kitsune/memutils.c src/kitsune/deque.c src/kitsune/str.c src/kitsune/utf8.c src/kitsune/map.c src/kitsune/vec.c src/kitsune/iterator.c 
OBJS = _obj/kitsune/dynamic_iterator.o _obj/kitsune/crashtrace.o _obj/kitsune/strutils.o _obj/kitsune/hashes.o _obj/kitsune/coroutines.o _obj/kitsune/list.o _obj/kitsune/stack.o _obj/kitsune/allocator.o _obj/kitsune/alloc/basic.o _obj/kitsune/alloc/traced.o _obj/kitsune/alloc/hardened.o _obj/kitsune/queue.o _obj/kitsune/rc.o _obj/kitsune/generator.o _obj/kitsune/memutils.o _obj/kitsune/deque.o _obj/kitsune/str.o _obj/kitsune/utf8.o _obj/kitsune/map.o _obj/kitsune/vec.o _obj/kitsune/iterator.o
LIB = lib/libkitsune.a

all: $(LIB)

$(LIB): $(OBJS)
	@mkdir -p lib
	$(AR) rcs $(LIB) $(OBJS)

_obj/kitsune/dynamic_iterator.o: src/kitsune/dynamic_iterator.c
	@mkdir -p _obj/kitsune
	$(CC) $(CFLAGS) -c src/kitsune/dynamic_iterator.c -o _obj/kitsune/dynamic_iterator.o

_obj/kitsune/crashtrace.o: src/kitsune/crashtrace.c
	@mkdir -p _obj/kitsune
	$(CC) $(CFLAGS) -c src/kitsune/crashtrace.c -o _obj/kitsune/crashtrace.o

_obj/kitsune/strutils.o: src/kitsune/strutils.c
	@mkdir -p _obj/kitsune
	$(CC) $(CFLAGS) -c src/kitsune/strutils.c -o _obj/kitsune/strutils.o

_obj/kitsune/hashes.o: src/kitsune/hashes.c
	@mkdir -p _obj/kitsune
	$(CC) $(CFLAGS) -c src/kitsune/hashes.c -o _obj/kitsune/hashes.o

_obj/kitsune/coroutines.o: src/kitsune/coroutines.c
	@mkdir -p _obj/kitsune
	$(CC) $(CFLAGS) -c src/kitsune/coroutines.c -o _obj/kitsune/coroutines.o

_obj/kitsune/list.o: src/kitsune/list.c
	@mkdir -p _obj/kitsune
	$(CC) $(CFLAGS) -c src/kitsune/list.c -o _obj/kitsune/list.o

_obj/kitsune/stack.o: src/kitsune/stack.c
	@mkdir -p _obj/kitsune
	$(CC) $(CFLAGS) -c src/kitsune/stack.c -o _obj/kitsune/stack.o

_obj/kitsune/allocator.o: src/kitsune/allocator.c
	@mkdir -p _obj/kitsune
	$(CC) $(CFLAGS) -c src/kitsune/allocator.c -o _obj/kitsune/allocator.o

_obj/kitsune/alloc/basic.o: src/kitsune/alloc/basic.c
	@mkdir -p _obj/kitsune/alloc
	$(CC) $(CFLAGS) -c src/kitsune/alloc/basic.c -o _obj/kitsune/alloc/basic.o

_obj/kitsune/alloc/traced.o: src/kitsune/alloc/traced.c
	@mkdir -p _obj/kitsune/alloc
	$(CC) $(CFLAGS) -c src/kitsune/alloc/traced.c -o _obj/kitsune/alloc/traced.o

_obj/kitsune/alloc/hardened.o: src/kitsune/alloc/hardened.c
	@mkdir -p _obj/kitsune/alloc
	$(CC) $(CFLAGS) -c src/kitsune/alloc/hardened.c -o _obj/kitsune/alloc/hardened.o

_obj/kitsune/queue.o: src/kitsune/queue.c
	@mkdir -p _obj/kitsune
	$(CC) $(CFLAGS) -c src/kitsune/queue.c -o _obj/kitsune/queue.o

_obj/kitsune/rc.o: src/kitsune/rc.c
	@mkdir -p _obj/kitsune
	$(CC) $(CFLAGS) -c src/kitsune/rc.c -o _obj/kitsune/rc.o

_obj/kitsune/generator.o: src/kitsune/generator.c
	@mkdir -p _obj/kitsune
	$(CC) $(CFLAGS) -c src/kitsune/generator.c -o _obj/kitsune/generator.o

_obj/kitsune/memutils.o: src/kitsune/memutils.c
	@mkdir -p _obj/kitsune
	$(CC) $(CFLAGS) -c src/kitsune/memutils.c -o _obj/kitsune/memutils.o

_obj/kitsune/deque.o: src/kitsune/deque.c
	@mkdir -p _obj/kitsune
	$(CC) $(CFLAGS) -c src/kitsune/deque.c -o _obj/kitsune/deque.o

_obj/kitsune/str.o: src/kitsune/str.c
	@mkdir -p _obj/kitsune
	$(CC) $(CFLAGS) -c src/kitsune/str.c -o _obj/kitsune/str.o

_obj/kitsune/utf8.o: src/kitsune/utf8.c
	@mkdir -p _obj/kitsune
	$(CC) $(CFLAGS) -c src/kitsune/utf8.c -o _obj/kitsune/utf8.o

_obj/kitsune/map.o: src/kitsune/map.c
	@mkdir -p _obj/kitsune
	$(CC) $(CFLAGS) -c src/kitsune/map.c -o _obj/kitsune/map.o

_obj/kitsune/vec.o: src/kitsune/vec.c
	@mkdir -p _obj/kitsune
	$(CC) $(CFLAGS) -c src/kitsune/vec.c -o _obj/kitsune/vec.o

_obj/kitsune/iterator.o: src/kitsune/iterator.c
	@mkdir -p _obj/kitsune
	$(CC) $(CFLAGS) -c src/kitsune/iterator.c -o _obj/kitsune/iterator.o

clean:
	rm -rf _obj lib bin *.core _install
