INCLUDE memmap.ld

ENTRY( InterruptVector )

SECTIONS
{
    .init :
    { 
      _sinit = .;
      . = ALIGN(4);
      KEEP(*(SORT_NONE(.init)))
      . = ALIGN(4);
      _einit = .;
    } >BOOTROM AT>BOOTROM

    .text : 
    {
        . = ALIGN(4);
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
    } > BOOTROM AT > BOOTROM

    .dalign :
    {
      . = ALIGN(4);
      PROVIDE(_data_vma = .);
    } >RAM AT>BOOTROM  

    .dlalign :
    {
      . = ALIGN(4); 
      PROVIDE(_data_lma = .);
    } >BOOTROM AT>BOOTROM

    .data : 
    {
      . = ALIGN(4);
      *(.data)
      *(.data.*)
      PROVIDE( __global_pointer$ = . + 0x800 );
    } > RAM AT > BOOTROM

    .fill :
    {
      FILL(0xFF);
      . = ORIGIN(BOOTROM) + LENGTH(BOOTROM) - 4;
      LONG(0xEFBEADDE)
    } > BOOTROM AT > BOOTROM

    .bss :
    {
        . = ALIGN(4);
        *(.bss)
        *(.bss*)
    } > RAM AT > BOOTROM
}

_sflash = ADDR(.text);
_eflash = _sflash + SIZEOF(.text);

_sdata = ADDR(.data);
_edata = _sdata + SIZEOF(.data);

_sbss = ADDR(.bss);
_ebss = _sbss + SIZEOF(.bss);

_eusrstack = ORIGIN(RAM) + LENGTH(RAM);
