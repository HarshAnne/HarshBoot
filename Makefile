## CONFIGURATION #############################################################

TARGET = Blink

## TOOLS #####################################################################

CC = riscv64-unknown-elf-gcc
OC = riscv64-unknown-elf-objcopy
OD = riscv64-unknown-elf-objdump

## OPTIONS ###################################################################

CFLAGS:=-march=rv32emc \
		-mabi=ilp32e \
		-flto -ffunction-sections \
		-nostdlib \
		#-Os

INCSRCS:=-I./src

CFLAGS+=

LDFLAGS:=-T harshboot.ld

## FILES #####################################################################

SRCFILES:= main.c startup.c

## TARGETS ###################################################################

all : $(TARGET).bin $(TARGET).hex $(TARGET).elf

$(TARGET).bin : $(TARGET).elf
	$(OC) -O binary $< $@

$(TARGET).hex : $(TARGET).elf
	$(OC) -O ihex $< $@

$(TARGET).elf : $(SRCFILES) $(SYSFILES)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(INCSRCS)
	$(OD) -S $@ > $(TARGET).lst
	$(OD) -t $@ > $(TARGET).map

clean :
	rm -rf $(TARGET).elf $(TARGET).bin $(TARGET).hex $(TARGET).lst $(TARGET).map tests.elf
