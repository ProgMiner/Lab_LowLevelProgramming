
CC = gcc
LD = gcc
CFLAGS = -std=c89 -pedantic-errors -Wall -Werror -g -O0 # -O2
LDFLAGS =

BUILDPATH = build
SOURCES = mem.c mem_debug.c
HEADERS = mem.h mem_debug.h
TARGET = lab7

OBJECTS = $(SOURCES:%.c=$(BUILDPATH)/%.o)

.PHONY: all build clean
.SUFFIXES:

all: build

clean:
	@-rm -vrf $(BUILDPATH) 2> /dev/null; true
	@-rm -v $(TARGET) 2> /dev/null; true

build: $(TARGET)

%.c:

$(OBJECTS): $(BUILDPATH)/%.o : %.c $(HEADERS)
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS)
