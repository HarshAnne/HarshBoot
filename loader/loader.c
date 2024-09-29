#include "ch32v003harsh.h"
#include "memmap.h"
#include "image.h"
#include "dfu.h"

#define LOADER_TIMEOUT_MS 5000

int main(void);
extern void *memset(void *dest, int c, unsigned int n);
extern int memcmp(const void *vl, const void *vr, unsigned int n);
extern int puts(const char*);
extern void InterruptVector();
void Loader_StartApp(void);
void Loader_Init(void);
void Loader_DeInit(void);
uint8_t Loader_RemainInBoot(void);

uint32_t loaderTimeMs = 0;
uint8_t buf[6u] = {0};
uint8_t buf_idx = 0;
const uint8_t pass[] = "harsh\n";
uint8_t program_buffer[64u];
uint8_t boot_app = 0u;

image_header_t header __attribute__((section(".image_header"), used)) = 
{
    .magic = IMAGE_MAGIC,
    .vector_addr = (uint32_t)&InterruptVector,
    .crc = 0,
    .image_size = 0,
    .image_type = 0,
    .version = 0,
    .reserved = 0,
    .git_hash = 0,
};

int main(void)
{
    SystemInit();
    __disable_irq();

    puts("Loader");

    Loader_Init();

    while(Loader_RemainInBoot())
    {
        if(SYSTICK_SR_CNTIF & SysTick->SR)
        {
            loaderTimeMs++;
            SysTick->SR = 0;
        }

        // Char available, read it
        if(USART1->STATR & USART_STATR_RXNE)
        {
            char tmp = USART1->DATAR & 0x1FFU;

            buf[buf_idx++] = tmp;

            if(buf_idx >= sizeof(buf))
            {
                buf_idx = 0U;
                // for(uint8_t i = 0; i < 5; i++)
                // {
                //     while( !(USART1->STATR & USART_FLAG_TC));
	            //     USART1->DATAR = buf[i];
                // }
                if(0 == memcmp(pass, buf, sizeof(buf)))
                {
                    puts("Unlocked");

                    dfu_init();
                    dfu_main();

                    boot_app = 1u;
                }
            }

            // while( !(USART1->STATR & USART_FLAG_TC));
	        // USART1->DATAR = buf_idx+48U;
        }

        // Check valid password
        // If valid, go to dfu mode

    }   

    Loader_DeInit();

    Loader_StartApp();

    while(1);
}

void Loader_Init(void)
{
    loaderTimeMs = 0;
    (void)memset(buf, 0, sizeof(buf));
    buf_idx = 0;

    // Setup Systick for ms
    SysTick->CNT = 0;
    SysTick->CMP = 48000; // 1ms

    SysTick->CTLR = SYSTICK_CTLR_STRE | SYSTICK_CTLR_STCLK; 

    // Enable Systick
    SysTick->CTLR |= SYSTICK_CTLR_STE;
}

void Loader_DeInit(void)
{
    // Deinit Systick
    SysTick->CTLR = 0;
    SysTick->SR = 0;
    SysTick->CNT = 0;
    SysTick->CMP = 0;

    // Deinit usart

}

uint8_t Loader_RemainInBoot(void)
{
    uint8_t result = 1U;

    if( (LOADER_TIMEOUT_MS <= loaderTimeMs) ||
        (boot_app == 1u) )
    {
        result = 0U;
    }

    return result;
}

void Loader_StartApp()
{
    const image_header_t *header = image_get_header(IMAGE_SLOT_2);

    if(header)
    {
        __enable_irq();
        image_start(header);
    }
    else
    {
        puts("No app img");
    }
}
