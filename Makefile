GPP_PARAMS = -m32 -Isrc -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
AS_PARAMS = -f elf
LD_PARAMS = -T linker.ld -melf_i386

OBJECTS = \
	src/Quartz/kprintf.o \
	src/Quartz/loader.o \
	src/Quartz/kernel.o \
	src/Quartz/core/GlobalDescriptorTable.o \
	src/Quartz/core/Port.o \
	src/Quartz/interrupts/interrupts.o \
	src/Quartz/interrupts/InterruptManager.o \
	src/Quartz/interrupts/InterruptHandler.o \
	src/Quartz/drivers/Driver.o \
	src/Quartz/drivers/KeyboardDriver.o \
	src/Quartz/drivers/MouseDriver.o \
	src/Quartz/drivers/PeripheralComponentInterconnect.o \
	src/Quartz/drivers/VGADriver.o \
	src/Quartz/heap/MemoryManager.o

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

	
