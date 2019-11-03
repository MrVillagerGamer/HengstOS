SRCS := $(wildcard *.c)
BINS := $(SRCS:%.c=%)
OBJS := $(SRCS:%.c=%.o)
ELFS := $(SRCS:%.c=%.elf)

crt0.o: crt0.s
	nasm -felf32 -o $@ $<
	
%: %.elf
	objcopy -O binary $< $@

%.elf: %.o
	ld -melf_i386 -nostdlib -Tlinker.ld -o $@ $< crt0.o

%.o: %.c
	gcc -m32 -ffreestanding -c -o $@ $<

all: crt0.o $(OBJS) $(ELFS) $(BINS)
	nasm -fbin init.asm -o init
	sudo cp init $(BINS) ../root/HengstOS/System32/

clean:
	rm $(OBJS) $(BINS) $(ELFS) init crt0.o || true