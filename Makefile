GPP_PARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
AS_PARAMS = -f elf
LD_PARAMS = -T linker.ld -melf_i386

OBJECTS = \
	src/lib/io.o \
	src/loader.o \
	src/kernel.o \
	src/GlobalDescriptorTable.o \
	src/Port.o \
	src/interrupts/interrupts.o \
	src/interrupts/InterruptManager.o \
	src/interrupts/InterruptHandler.o \
	src/drivers/KeyboardDriver.o

all: kernel.iso

%.o: %.cpp
	g++ $(GPP_PARAMS) -c  $< -o $@ 

%.o: %.s
	nasm $(AS_PARAMS) $< -o $@ 

kernel.elf: linker.ld $(OBJECTS)
	ld $(LD_PARAMS)  -o $@ $(OBJECTS)

clean:
	rm -f $(OBJECTS)
	rm -f *.bin
	rm -f /build/kernel.iso

kernel.iso: kernel.elf
	cp kernel.elf build/iso/boot/
	rm -f kernel.elf
	genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -A os -input-charset utf8 -quiet -boot-info-table -o build/kernel.iso build/iso

	
