ENTRY( InterruptVector )

MEMORY
{
	FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 2K
	RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 2K
}

SECTIONS
{
    .init :
    { 
      _sinit = .;
      . = ALIGN(4);
      KEEP(*(SORT_NONE(.init)))
      . = ALIGN(4);
      _einit = .;
    } >FLASH AT>FLASH

    .text : 
    {
        . = ALIGN(4);
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
    } > FLASH AT > FLASH

    .dalign :
    {
      . = ALIGN(4);
      PROVIDE(_data_vma = .);
    } >RAM AT>FLASH  

    .dlalign :
    {
      . = ALIGN(4); 
      PROVIDE(_data_lma = .);
    } >FLASH AT>FLASH

    .data : 
    {
        . = ALIGN(4);
        *(.data)
        *(.data.*)
        PROVIDE( __global_pointer$ = . + 0x800 );
    } > RAM AT > FLASH

    .bss :
    {
        . = ALIGN(4);
        *(.bss)
        *(.bss*)
    } > RAM AT > FLASH
}

_sflash = ADDR(.text);
_eflash = _sflash + SIZEOF(.text);

_sdata = ADDR(.data);
_edata = _sdata + SIZEOF(.data);

_sbss = ADDR(.bss);
_ebss = _sbss + SIZEOF(.bss);

_eusrstack = ORIGIN(RAM) + LENGTH(RAM);
