## CONFIGURATION #############################################################

TARGET = boot

## TOOLS #####################################################################

CC = @riscv64-unknown-elf-gcc
OC = @riscv64-unknown-elf-objcopy
OD = @riscv64-unknown-elf-objdump
SZ = @riscv64-unknown-elf-size

## PATHS## ###################################################################

BUILDDIR = build
LIBSDIR = ../libs

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

INCSRCS:=-I/usr/include/newlib \
		-I$(LIBSDIR)/ch32v003 \
		-I$(LIBSDIR)/image \
		-I. \
		-I../

CFLAGS+=

LDFLAGS:=-T boot.ld \
		-L ../

## FILES #####################################################################

SRCFILES:= boot.c
SYSFILES:= $(LIBSDIR)/ch32v003/ch32v003harsh.c
SYSFILES+= $(LIBSDIR)/image/image.c

## TARGETS ###################################################################
.PHONY: all

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
	@mkdir -p build

clean :
	@rm -rf build
