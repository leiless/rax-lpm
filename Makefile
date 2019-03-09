#
# Created 190309 lynnl
#

CC?=gcc
CFLAGS+=-std=c99 -Wall -Wextra -Werror
SOURCES=$(wildcard *.c)
SOURCES+=rax/rax.c
EXECUTABLE=test_lpm
RM=rm

all: debug

release: CFLAGS += -O2
release: $(SOURCES)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $(EXECUTABLE)

rax.o: rax/rax.h

debug: CPPFLAGS += -g -DDEBUG
debug: CFLAGS += -O0
debug: $(SOURCES)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $(EXECUTABLE)

clean:
	$(RM) -rf *.o *.dSYM $(EXECUTABLE)

.PHONY: all debug release clean

