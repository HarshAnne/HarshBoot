#ifndef MEMMAP_H
#define MEMMAP_H

/* == Headers ============================================================= */

#include <stdint.h>

/* == Macros ============================================================== */

#define BOOT_MEMORY_START   0x00000000UL
#define APP_MEMORY_START    0x00001000UL

/* == Types =============================================================== */

/* == Variables =========================================================== */

extern uint32_t * _sbss;
extern uint32_t * _ebss;
extern uint32_t * _data_lma;
extern uint32_t * _data_vma;
extern uint32_t * _sdata;
extern uint32_t * _edata;
extern uint32_t * _sflash;
extern uint32_t * _eflash;

/* == Prototypes ========================================================== */


#endif /* MEMMAP_H */