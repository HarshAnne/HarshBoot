#include "ch32v003harsh.h"
#include "memmap.h"
#include "image.h"
#include <stdint.h>

int main(void);
extern int puts(const char*);
void Boot_StartApp(uint32_t addr) __attribute__((noinline, used));

int main(void)
{
    SystemInit();
    __disable_irq();

    puts("Boot Init!");

    for(image_slot_t i = IMAGE_SLOT_1; i < IMAGE_NUM_SLOTS; i++)
    {
        const image_header_t *header = image_get_header(IMAGE_SLOT_2);

        if(header)
        {
            // image valid
            // __enable_irq();
            image_start(header);
        }
    }

    // __enable_irq();
    // Boot_StartApp(0x1400);

    while(1);
}


void Boot_StartApp(uint32_t addr)
{
    __ASM volatile("jr a0");
}
