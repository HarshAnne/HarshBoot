## CONFIGURATION #############################################################

TARGET = harshboot

## TOOLS #####################################################################

CC = riscv64-unknown-elf-gcc
OC = riscv64-unknown-elf-objcopy
OD = riscv64-unknown-elf-objdump
SZ = riscv64-unknown-elf-size

## OPTIONS ###################################################################

CFLAGS:=-march=rv32emc \
		-mabi=ilp32e \
		-flto -ffunction-sections \
		-nostdlib \
		-Os

INCSRCS:=-I. \
		-I/usr/include/newlib

CFLAGS+=

LDFLAGS:=-T harshboot.ld

## FILES #####################################################################

SRCFILES:= main.c
SYSFILES:= ch32v003harsh.c

## TARGETS ###################################################################

all : $(TARGET).bin $(TARGET).hex $(TARGET).elf

$(TARGET).bin : $(TARGET).elf
	$(OC) -O binary $< $@ --pad-to 0x1000
	$(SZ) --target binary $@

$(TARGET).hex : $(TARGET).elf
	$(OC) -O ihex $< $@ --pad-to 0x1000
	$(SZ) --target ihex $@

$(TARGET).elf : $(SRCFILES) $(SYSFILES)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(INCSRCS)
	$(OD) -S $@ > $(TARGET).lst
	$(OD) -t $@ > $(TARGET).map

clean :
	rm -rf $(TARGET).elf $(TARGET).bin $(TARGET).hex $(TARGET).lst $(TARGET).map
