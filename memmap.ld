MEMORY
{
	BOOTROM (rx) : ORIGIN = 0x00000000, LENGTH = 1K
	LOADERROM (rx): ORIGIN = 0x00000400, LENGTH = 3K
	APPROM (rx) : ORIGIN = 0x00001000, LENGTH = 12K
	RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 2K
}

__BOOTROM_START__ = ORIGIN(BOOTROM);
__BOOTROM_SIZE__ = LENGTH(BOOTROM);
__LOADERROM_START__ = ORIGIN(LOADERROM);
__LOADERROM_SIZE__ = LENGTH(LOADERROM);
__APPROM_START__ = ORIGIN(APPROM);
__APPROM_SIZE__ = LENGTH(APPROM);
