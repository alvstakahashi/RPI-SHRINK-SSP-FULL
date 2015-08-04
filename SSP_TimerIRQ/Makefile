CROSS= arm-none-eabi-

CC= $(CROSS)gcc
LD= $(CROSS)gcc
OBJCOPY= $(CROSS)objcopy
OBJDUMP= $(CROSS)objdump
SIZE= $(CROSS)size
READELF= $(CROSS)readelf

STARTUP= startup.o
OBJS= main.o

# systemcall
OBJS += syscalls.o
OBJS += setup.o

# RaspberryPi library
OBJS+= rpi_lib/rpi_init.o rpi_lib/gpio/gpio.o rpi_lib/timer/timer.o rpi_lib/delay/delay.o
OBJS+= rpi_lib/serial/serial.o
OBJS+= rpi_lib/bss/clearbss.o

#ssp liblary
OBJS+= ssp/kernel_cfg.o ssp/startup.o ssp/task.o ssp/task_manage.o
OBJS+= ssp/alarm.o ssp/cyclic.o ssp/time_event.o ssp/time_manage.o

#ssp include
INCSSP = -I./ssp -I./include -I./arch/RPI

CFLAGS = -mcpu=arm1176jzf-s -mtune=arm1176jzf-s -mfloat-abi=softfp -O0 -ggdb -g

CFLAGS+=  -mlittle-endian $(INCSSP) $(INCDIR)
CFLAGS+= -nostartfiles
CFLAGS+= -ffreestanding
CFLAGS += -O0 -g -gdwarf-2
CFLAGS += -D$(RPITYPE)

########################
all:	rpi-micon.img

rpi-micon.elf:	$(STARTUP) $(OBJS)
	$(LD) -static -nostartfiles -T rpiMicon.lds $^ -o $@  $(ULIBDIR) $(ULIBS) $(LIB) -lc -lm -lgcc 
	$(OBJDUMP) -D rpi-micon.elf > rpi-micon.disas
	$(SIZE) rpi-micon.elf > rpi-micon.size
	$(READELF) -a rpi-micon.elf > rpi-micon.readelf

.SUFFIXES: .elf .img

.elf.img:
	$(OBJCOPY) -O binary $< $@
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
.S.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean::
	$(RM) -f *.o *.img *.elf */*.o */*/*.o 
	$(RM) -f tags *~

tags::
	ctags *.[chS]
