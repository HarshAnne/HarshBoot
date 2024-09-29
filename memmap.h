#ifndef MEMMAP_H
#define MEMMAP_H

/* == Headers ============================================================= */

#include <stdint.h>

/* == Macros ============================================================== */

/* == Types =============================================================== */

/* == Variables =========================================================== */

extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _data_lma;
extern uint32_t _data_vma;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _stext;
extern uint32_t _etext;

extern uint32_t __BOOTROM_START__;
extern uint32_t __BOOTFROM_SIZE__;
extern uint32_t __LOADERROM_START__;
extern uint32_t __LOADERROM_SIZE__;
extern uint32_t __APPROM_START__; 
extern uint32_t __APPROM_SIZE__;

/* == Prototypes ========================================================== */


#endif /* MEMMAP_H */