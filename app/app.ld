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
    . = ALIGN(1024);
    _sinit = .;
    KEEP(*(SORT_NONE(.init)))
    . = ALIGN(4);
    _einit = .;
  } >APPROM AT>APPROM

  .text : 
  {
      . = ALIGN(4);
      PROVIDE(_stext = .);
      *(.text)
      *(.text.*)
      *(.rodata)
      *(.rodata*)
      . = ALIGN(4);
      PROVIDE(_etext = .);
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
    PROVIDE(_sdata = .);
    *(.data .data.*)
    . = ALIGN(8);
    PROVIDE( __global_pointer$ = . + 0x800 );
    *(.sdata .sdata.*)
    *(.sdata2*)
    . = ALIGN(8);
    *(.srodata.cst16)
    *(.srodata.cst8)
    *(.srodata.cst4)
    *(.srodata.cst2)
    *(.srodata .srodata.*)
    . = ALIGN(4);
    PROVIDE(_edata = .);
  } > RAM AT > APPROM

  .bss :
  {
    . = ALIGN(4);
    PROVIDE( _sbss = .);
    *(.sbss*)
    *(.gnu.linkonce.sb.*)
    *(.bss*)
    *(.gnu.linkonce.b.*)    
    *(COMMON*)
    . = ALIGN(4);
    PROVIDE( _ebss = .);
  } > RAM AT > APPROM

  PROVIDE(_end = _ebss);
  PROVIDE(end = .);

  PROVIDE(_eusrstack = ORIGIN(RAM) + LENGTH(RAM));
}
