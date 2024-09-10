/* == Headers ============================================================= */

#include "ch32v003harsh.h"
#include "memmap.h"

/* == Macros ============================================================== */

/* == Types =============================================================== */

/* == Variables =========================================================== */

/* == Prototypes ========================================================== */

int main(void);
extern int puts(const char*);
void Boot_StartApp(void)  __attribute__((used));


/* == Functions =========================================================== */

int main(void)
{
    SystemInit();
    
    puts("Bootloader Init!");

    __disable_irq();
    Boot_StartApp();

    while(1);
}

/* ------------------------------------------------------------------------ */

void Boot_StartApp(void)
{
    // Get pointer to app start 
    // typedef void (*AppType)(void);
    // volatile AppType Execute = (AppType)(*((uint32_t*)(APP_MEMORY_START)));

    // Set stack pointer to app stack pointer address?
    // handle_reset will do this?
    
    // Jump to app start function using function pointer
    // Execute();
    __ASM volatile("j 0x1000");
}
