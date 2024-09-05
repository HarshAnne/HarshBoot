ENTRY( main )

MEMORY
{
	FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 16K
	RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 2K
}

SECTIONS
{
    .text : 
    {
        . = ALIGN(4);
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
    } > FLASH AT> FLASH

    .data : 
    {
        . = ALIGN(4);
        *(.data)
        *(.sdata)
    } > RAM AT> FLASH

    .bss :
    {
        . = ALIGN(4);
        *(.bss)
        *(.bss*)
    } > RAM AT> FLASH
}

__sflash = ADDR(.text);
__eflash = __sflash + SIZEOF(.text);

__sdata = ADDR(.data);
__edata = __sdata + SIZEOF(.data);

__sbss = ADDR(.bss);
__ebss = __sbss + SIZEOF(.bss);

__stack = ORIGIN(RAM) + LENGTH(RAM);
