
AS = nasm
CC = gcc
LD = gcc
ASFLAGS = -felf64 -g -O0
CFLAGS = -std=c89 -pedantic-errors -Wall -Werror -I../lab6 -fPIC -g -O0 # -O2
LDFLAGS = -shared

BUILDPATH = build
CSOURCES = sepia.c
ASOURCES = do_sepia.asm
HEADERS =
TARGET = sepia.so

COBJECTS = $(CSOURCES:%.c=$(BUILDPATH)/%.o)
AOBJECTS = $(ASOURCES:%.asm=$(BUILDPATH)/%.o)

.PHONY: all build clean
.SUFFIXES:

all: build

clean:
	@rm -vrf $(BUILDPATH) 2> /dev/null; true
	@rm -v parser.h parser.c lexer.c $(TARGET) 2> /dev/null; true

build: $(TARGET)

%.asm:

%.c:

$(AOBJECTS): $(BUILDPATH)/%.o : %.asm $(HEADERS)
	@mkdir -p $(@D)
	$(AS) -o $@ $< $(ASFLAGS)

$(COBJECTS): $(BUILDPATH)/%.o : %.c $(HEADERS)
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(COBJECTS) $(AOBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS)
