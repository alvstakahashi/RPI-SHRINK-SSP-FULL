CROSS= arm-none-eabi-

CC= $(CROSS)gcc
LD= $(CROSS)gcc
OBJCOPY= $(CROSS)objcopy
OBJDUMP= $(CROSS)objdump
SIZE= $(CROSS)size
READELF= $(CROSS)readelf

QEMU  = qemu-system-arm

STARTUP= startup.o
OBJS= main.o io.o
# systemcall
OBJS += syscalls.o

# RaspberryPi library
OBJS+= rpi_lib/rpi_init.o rpi_lib/gpio/gpio.o rpi_lib/timer/timer.o rpi_lib/delay/delay.o
OBJS+= rpi_lib/serial/serial.o
OBJS+= rpi_lib/serial/miniUart1.o
OBJS+= rpi_lib/bss/clearbss.o

#ssp liblary
OBJS+= ssp/kernel_cfg.o ssp/startup.o ssp/task.o ssp/task_manage.o
OBJS += arch/RPI/vector.o arch/RPI/qemu_timer.o

#ssp include
INCSSP = -I./ssp -I./include -I./arch/RPI -I./rpi_lib -I./rpi_lib/peripherals

#CFLAGS = -mcpu=arm1176jzf-s -mtune=arm1176jzf-s -mfloat-abi=softfp -O0 -ggdb -g
CFLAGS = -mcpu=cortex-a9  -mfloat-abi=softfp -O0 -ggdb -g

CFLAGS+=  -mlittle-endian $(INCSSP) $(INCDIR)
CFLAGS+= -nostartfiles
CFLAGS+= -ffreestanding
CFLAGS += -O0 -g -gdwarf-2
CFLAGS += -DRPI3 -DTOPPERS_USE_BCM2837

LDFLAGS = -Wl,-Map=kernel7.map

########################
all:	kernel7.img

kernel7.elf:	$(STARTUP) $(OBJS)
	$(LD) -static -nostartfiles -T kernel.lds $^ $(LDFLAGS) -o $@  $(ULIBDIR) $(ULIBS) $(LIB) -lc -lm -lgcc 
	$(OBJDUMP) -D kernel7.elf > kernel7.disas
	$(SIZE) kernel7.elf > kernel7.size
	$(READELF) -a kernel7.elf > kernel7.readelf

.SUFFIXES: .elf .img

.elf.img:
	$(OBJCOPY) -O binary $< $@
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
.S.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean::
	$(RM) -f *.o *.img *.elf *.disas *.size *.readelf */*.o */*/*.o 
	$(RM) -f tags *~

tags::
	ctags *.[chS]

runq:$(OBJFILE)
		$(QEMU) -M raspi2 -display none -serial null -serial stdio -kernel kernel7.elf

runx:$(OBJFILE)
		$(QEMU) -M raspi2 -display none -serial null -serial stdio -kernel kernel7.elf -d in_asm
