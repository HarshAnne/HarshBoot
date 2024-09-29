INCLUDE memmap.ld

ENTRY( InterruptVector )

SECTIONS
{
  .image_header :
  {
    KEEP(*(.image_header))
  } > LOADERROM AT > LOADERROM
  
  .init :
  { 
    . = ALIGN(4);
    _sinit = .;
    KEEP(*(SORT_NONE(.init)))
    . = ALIGN(4);
    _einit = .;
  } >LOADERROM AT>LOADERROM

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
  } > LOADERROM AT > LOADERROM

  .dalign :
  {
    . = ALIGN(4);
    PROVIDE(_data_vma = .);
  } >RAM AT>LOADERROM  

  .dlalign :
  {
    . = ALIGN(4); 
    PROVIDE(_data_lma = .);
  } >LOADERROM AT>LOADERROM

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
  } > RAM AT > LOADERROM

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
  } > RAM AT > LOADERROM

  .fill :
  {
    FILL(0xFF);
    . = ORIGIN(LOADERROM) + LENGTH(LOADERROM) - 4;
    LONG(0xEFBEADDE)
  } > LOADERROM AT > LOADERROM

  PROVIDE(_end = _ebss);
  PROVIDE(end = .);

  PROVIDE(_eusrstack = ORIGIN(RAM) + LENGTH(RAM));
}

