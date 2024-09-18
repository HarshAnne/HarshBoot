## CONFIGURATION #############################################################

TARGET = harshboot_1920B

## TOOLS #####################################################################

CC = riscv64-unknown-elf-gcc
OC = riscv64-unknown-elf-objcopy
OD = riscv64-unknown-elf-objdump
SZ = riscv64-unknown-elf-size

## PATHS## ###################################################################

BUILDDIR = build

## FILES #####################################################################

ELF    = $(BUILDDIR)/$(TARGET).elf
BIN    = $(BUILDDIR)/$(TARGET).bin
HEX    = $(BUILDDIR)/$(TARGET).hex
MAP    = $(BUILDDIR)/$(TARGET).map
LST    = $(BUILDDIR)/$(TARGET).lst

## OPTIONS ###################################################################

CFLAGS:=-march=rv32emc \
		-mabi=ilp32e \
		-flto -ffunction-sections \
		-nostdlib \
		-Os

INCSRCS:=-I. \
		-I/usr/include/newlib

CFLAGS+=

LDFLAGS:=-T harshboot_1920B.ld

## FILES #####################################################################

SRCFILES:= harshboot_1920B.c
SYSFILES:= ch32v003harsh.c

## TARGETS ###################################################################

all : $(BIN) $(HEX) $(ELF) dirs

$(BIN) : $(ELF)
	$(OC) -O binary $< $@
	$(SZ) --target binary $@

$(HEX) : $(ELF)
	$(OC) -O ihex $< $@
	$(SZ) --target ihex $@

$(ELF) : $(SRCFILES) $(SYSFILES) dirs
	$(CC) -o $@ $(SRCFILES) $(SYSFILES) $(CFLAGS) $(LDFLAGS) $(INCSRCS)
	$(OD) -S $@ > $(LST)
	$(OD) -t $@ > $(MAP)

dirs:
	mkdir -p build

clean :
	rm -rf build