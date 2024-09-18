INCLUDE memmap.ld

ENTRY( InterruptVector )

SECTIONS
{
    .image_header :
    {
      KEEP(*(.image_header))
    } > APPROM AT > APPROM

    .init :
    { 
      _sinit = .;
      . = ALIGN(1024);
      KEEP(*(SORT_NONE(.init)))
      . = ALIGN(4);
      _einit = .;
    } >APPROM AT>APPROM

    .text : 
    {
        . = ALIGN(4);
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
    } > APPROM AT > APPROM

    .dalign :
    {
      . = ALIGN(4);
      PROVIDE(_data_vma = .);
    } >RAM AT>APPROM  

    .dlalign :
    {
      . = ALIGN(4); 
      PROVIDE(_data_lma = .);
    } >APPROM AT>APPROM

    .data : 
    {
        . = ALIGN(4);
        *(.data)
        *(.data.*)
        PROVIDE( __global_pointer$ = . + 0x800 );
    } > RAM AT > APPROM

    .bss :
    {
        . = ALIGN(4);
        *(.bss)
        *(.bss*)
    } > RAM AT > APPROM
}

_sflash = ADDR(.text);
_eflash = _sflash + SIZEOF(.text);

_sdata = ADDR(.data);
_edata = _sdata + SIZEOF(.data);

_sbss = ADDR(.bss);
_ebss = _sbss + SIZEOF(.bss);

_eusrstack = ORIGIN(RAM) + LENGTH(RAM);
