#include "ch32v003harsh.h"

int main(void);
void SystemReset_StartMode(uint32_t Mode);

int main(void)
{
    SystemInit();
    SystemReset_StartMode(Start_Mode_USER);
    NVIC_SystemReset();
}

inline void SystemReset_StartMode(uint32_t Mode)
{
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;

    FLASH->BOOT_MODEKEYR = FLASH_KEY1;
    FLASH->BOOT_MODEKEYR = FLASH_KEY2;

    FLASH->STATR &= ~(1<<14);
    if(Mode == Start_Mode_BOOT){
        FLASH->STATR |= (1<<14);
    }

    FLASH->CTLR |= CR_LOCK_Set;
}