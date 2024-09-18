## CONFIGURATION #############################################################

TARGET = harshapp

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
		-I../ \
		-I/usr/include/newlib

CFLAGS+=

LDFLAGS:=-L ../ -T harshapp.ld

## FILES #####################################################################

SRCFILES:= main.c
SYSFILES:= ../ch32v003harsh.c

## TARGETS ###################################################################

all : $(TARGET).bin $(TARGET).hex $(TARGET).elf harsh.bin

harsh.bin: ../build/harshboot.bin $(TARGET).bin
	cat $^ > $@

$(TARGET).bin : $(TARGET).elf
	$(OC) -O binary $< $@
	$(SZ) --target binary $@

$(TARGET).hex : $(TARGET).elf
	$(OC) -O ihex $< $@
	$(SZ) --target ihex $@

$(TARGET).elf : $(SRCFILES) $(SYSFILES)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(INCSRCS)
	$(OD) -S $@ > $(TARGET).lst
	$(OD) -t $@ > $(TARGET).map

clean :
	rm -rf $(TARGET).elf $(TARGET).bin $(TARGET).hex $(TARGET).lst $(TARGET).map harsh.bin
