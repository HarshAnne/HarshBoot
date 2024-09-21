#include "image.h"
#include <stdint.h>
#include "memmap.h"
#include <stddef.h>
#include "ch32v003harsh.h"

static void __image_start(uint32_t addr);

__attribute__((noinline, used))
const image_header_t * image_get_header(image_slot_t slot)
{
    const image_header_t *header = NULL;

    if(IMAGE_SLOT_1 == slot)
    {
        // header = (const image_header_t *)__LOADERROM_START__;
        header = NULL;
    }
    else if (IMAGE_SLOT_2 == slot)
    {
        header = (const image_header_t*)&__APPROM_START__;
    }
    
    if(header && header->magic == IMAGE_MAGIC)
    {
        return header;
    }
    else
    {
        return NULL;
    }
}

uint8_t image_valid(const image_header_t *header)
{
    return 0;
}

__attribute__((noinline, used))
void image_start(const image_header_t *header)
{
    // Update vector table offset

    // Jump to program
    __image_start(header->vector_addr);
}

__attribute__((noinline, used))
void __image_start(uint32_t addr)
{
    __ASM volatile("jr a0");
}