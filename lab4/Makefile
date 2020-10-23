
CC = gcc
LD = gcc
CFLAGS = -std=c89 -pedantic-errors -Wall -Werror -O2
LDFLAGS =

BUILDPATH = build
COMMON_SOURCES = linked_list.c
HEADERS = linked_list.h
TARGETS = lab4 lab4_hof

SOURCES = $(COMMON_SOURCES) $(TARGETS:%=%.c)
OBJECTS = $(SOURCES:%.c=$(BUILDPATH)/%.o)

.PHONY: all build clean
.SUFFIXES:

all: build

clean:
	rm -rf $(BUILDPATH)

build: $(TARGETS)

%.c:

$(OBJECTS): $(BUILDPATH)/%.o : %.c $(HEADERS)
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGETS): % : $(BUILDPATH)/%.o $(COMMON_SOURCES:%.c=$(BUILDPATH)/%.o)
	$(LD) -o $@ $^ $(LDFLAGS)
