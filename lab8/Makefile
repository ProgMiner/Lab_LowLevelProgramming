
AS = nasm
CC = gcc
LD = gcc
ASFLAGS = -felf64 -g -O0
CFLAGS = -std=c89 -pedantic-errors -Wall -Werror -I../lab6 -fPIC -g -O0 # -O2
LDFLAGS = -shared

BUILDPATH = build
ASSOURCES = sepia.asm
CSOURCES = sepia_naive.c
HEADERS =
TARGET = sepia.so

ASOBJECTS = $(ASSOURCES:%=$(BUILDPATH)/%.o)
COBJECTS = $(CSOURCES:%=$(BUILDPATH)/%.o)
OBJECTS = $(ASOBJECTS) $(COBJECTS)

.PHONY: all build clean
.SUFFIXES:

all: build

clean:
	@rm -vrf $(BUILDPATH) 2> /dev/null; true
	@rm -v $(TARGET) 2> /dev/null; true

build: $(TARGET)

%.asm:

%.c:

$(ASOBJECTS): $(BUILDPATH)/%.o : %
	@mkdir -p $(@D)
	$(AS) -o $@ $< $(ASFLAGS)

$(COBJECTS): $(BUILDPATH)/%.o : % $(HEADERS)
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS)
