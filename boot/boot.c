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

    puts("Boot");

    for(image_slot_t i = IMAGE_SLOT_1; i < IMAGE_NUM_SLOTS; i++)
    {
        const image_header_t *header = image_get_header(i);

        if(header)
        {
            // image valid
            __enable_irq();
            image_start(header);
        }
    }

    while(1);
}


// void Boot_StartApp(uint32_t addr)
// {
//     __ASM volatile("jr a0");
// }
