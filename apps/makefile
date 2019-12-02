SRCS := $(wildcard *.c)
BINS := $(SRCS:%.c=%)
OBJS := $(SRCS:%.c=%.o)
ELFS := $(SRCS:%.c=%.elf)

LIBSRCS := $(wildcard libc/*.c)
LIBOBJS := $(LIBSRCS:%.c=%.o)

libc/%.o: libc/%.c
	gcc -m32 -ffreestanding -c -o $@ $<

libc/%.o: libc/%.s
	nasm -felf32 -o $@ $<

crt0.o: crt0.s
	nasm -felf32 -o $@ $<

gfm.o: gfm.s
	nasm -felf32 -o $@ $<
 
%: %.elf
	objcopy -O binary $< $@

%.elf: %.o
	ld -melf_i386 -nostdlib -Tlinker.ld -o $@ $< gfm.o crt0.o $(LIBOBJS)

%.o: %.c
	gcc -m32 -ffreestanding -c -o $@ $<

all: gfm.o crt0.o $(LIBOBJS) $(OBJS) $(ELFS) $(BINS)
	nasm -fbin init.asm -o init
	cp init $(BINS) ../root/HengstOS/System32/

clean:
	rm $(LIBOBJS) $(OBJS) $(BINS) $(ELFS) init crt0.o || true