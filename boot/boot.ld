INCLUDE memmap.ld

ENTRY( InterruptVector )

SECTIONS
{
  .init :
  { 
    . = ALIGN(4);
    _sinit = .;
    KEEP(*(SORT_NONE(.init)))
    . = ALIGN(4);
    _einit = .;
  } >BOOTROM AT>BOOTROM

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
  } > RAM AT > BOOTROM

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
  } > RAM AT > BOOTROM

  .fill :
  {
    FILL(0xFF);
    . = ORIGIN(BOOTROM) + LENGTH(BOOTROM) - 4;
    LONG(0xEFBEADDE)
  } > BOOTROM AT > BOOTROM

  PROVIDE(_end = _ebss);
  PROVIDE(end = .);

  PROVIDE(_eusrstack = ORIGIN(RAM) + LENGTH(RAM));
}
