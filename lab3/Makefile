
CC = gcc
LD = gcc
CFLAGS = -std=c89 -pedantic-errors -Wall -Werror -O2
LDFLAGS = -lm

BUILDPATH = build

SOURCES = scalar.c primes.c
OBJECTS = $(SOURCES:%.c=$(BUILDPATH)/%.o)
TARGETS = $(SOURCES:%.c=%)

.PHONY: all build
.SUFFIXES:

all: build

build: | $(TARGETS)

%.c:

$(OBJECTS): $(BUILDPATH)/%.o : %.c
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGETS): % : $(BUILDPATH)/%.o
	$(LD) -o $@ $^ $(LDFLAGS)
