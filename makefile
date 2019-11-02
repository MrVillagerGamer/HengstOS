OBJECTS=arch/i386/init.c.o arch/i386/mseg.c.o kern/kern.c.o kern/boot.s.o \
	arch/i386/ints.c.o arch/i386/ints.s.o arch/i386/port.c.o arch/i386/user.c.o \
	arch/i386/term.c.o arch/i386/task.s.o arch/i386/task.c.o arch/i386/pci.c.o \
	arch/i386/sys.s.o arch/i386/sys.c.o kern/proc.c.o fs/vfs.c.o fs/ramdisk.c.o \
	drivers/input/ps2_kbd.c.o drivers/input/ps2_kbd.s.o
%.c.o: %.c
	gcc -D__INIX_ARCH_I386__ -m32 -ffreestanding -I. -c -o $@ $<

%.s.o: %.s
	nasm -felf32 -o $@ $<

all: $(OBJECTS)
	ld -melf_i386 -Tlinker.ld -nostdlib -o iso/boot/kernel.elf $(OBJECTS)
	cd root && tar -cvf ../iso/boot/initrd.tar "HengstOS" && cd ..
	grub-mkrescue iso -o cdrom.iso
	
clean:
	rm $(OBJECTS) || true
	rm iso/boot/kernel.elf || true
	rm iso/boot/initrd.tar || true
