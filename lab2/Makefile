
AS = nasm
ASFLAGS = -felf64 # -g

LD = ld
LDFLAGS =

BUILDPATH = build
SOURCES = lib.asm dict.asm main.asm
INCLUDES = colon.inc words.inc
TARGET = dict

OBJECTS = $(SOURCES:%.asm=$(BUILDPATH)/%.o)

.PHONY: all build clean run

all: build

clean:
	rm -rf $(BUILDPATH)

run:
	"$(TARGET)" $(ARGS)

build: $(TARGET)

%.asm:

$(BUILDPATH)/%.o: %.asm $(INCLUDES)
	mkdir -p $(dir $@)
	$(AS) -o $@ $< $(ASFLAGS)

$(TARGET): $(OBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS)
