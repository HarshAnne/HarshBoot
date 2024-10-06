#include <stdint.h>
#include "ch32v003harsh.h"
#include "dfu.h"

/*

[4] = Start Address
[4] = End Address
[4] = Number of bytes
[1] = Byte 1
[2] = Byte 2
...
...

*/

#define FLASH_WRITE(addr, buf) \
        flash_unlock_fast(); \
        flash_erase_page(addr); \
        flash_write_page(addr, (uint32_t*)buf); \
        flash_lock_fast()


extern void *memset(void *dest, int c, unsigned int n);
extern int puts(const char*);
void flash_unlock_fast(void);
void flash_lock_fast(void);
void flash_erase_page(uint32_t addr);
uint8_t flash_write_page(const uint32_t addr, uint32_t* buf);

uint8_t dfu_buffer[64u];
uint8_t dfu_buf_idx;

void dfu_init(void)
{
    memset(dfu_buffer, 0x00, sizeof(dfu_buffer));
    dfu_buf_idx = 0u;
}

void dfu_main(void)
{
    uint32_t num_bytes = 0u;
    uint8_t count = 0u;
    uint32_t start_address;
    uint32_t current_address;


    current_address = 0U;
    start_address = 0U;
    num_bytes = 0u;
    
    // Read start address
    count = 4u;
    while(count)
    {
        if(USART1->STATR & USART_STATR_RXNE)
        {
            const uint8_t tmp = (uint8_t)(USART1->DATAR & 0x1FFu);
            // little endian
            start_address |= ((tmp) << (8*(4u - count)));
            count--;
        }
    }

    current_address = start_address;

    // Read num bytes
    count = 4u;
    while(count)
    {
        if(USART1->STATR & USART_STATR_RXNE)
        {
            // little endian
            num_bytes |= (USART1->DATAR & 0x1FFu) << (8*(4u - count));
            count--;
        }
    }

    if( (start_address < 0x08001000) ||
        (start_address >= 0x08004000) )
    {
        // puts("Invalid start address");
        return;
    }

    dfu_buf_idx = 0;

    while(num_bytes)
    {
        if(dfu_buf_idx >= sizeof(dfu_buffer))
        {
            FLASH_WRITE(current_address, dfu_buffer);
            current_address += 64U;
            dfu_buf_idx = 0;
            memset(dfu_buffer, 0, sizeof(dfu_buffer));
        }

        if(USART1->STATR & USART_STATR_RXNE)
        {
            // grab next byte
            dfu_buffer[dfu_buf_idx] = (uint8_t)(USART1->DATAR & 0x1FFu);
            num_bytes--;
            dfu_buf_idx++;
        }
    }

    // Process last page if there is data
    if(dfu_buf_idx > 0)
    {
        FLASH_WRITE(current_address,dfu_buffer);
        dfu_buf_idx = 0;
    }
}

void flash_unlock_fast(void)
{
    /* Authorize the FPEC of Bank1 Access */
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;

    /* Fast program mode unlock */
    FLASH->MODEKEYR = FLASH_KEY1;
    FLASH->MODEKEYR = FLASH_KEY2;
}

void flash_lock_fast(void)
{
    FLASH->CTLR |= CR_FAST_LOCK_Set;
    FLASH->CTLR |= CR_LOCK_Set;
}

void flash_erase_page(uint32_t addr)
{
    // Check the lock bit and fast lock in CTLR
    if((FLASH->CTLR & CR_LOCK_Set) ||
        (FLASH->CTLR & CR_FAST_LOCK_Set)) return;

    // Set the FTER bit in CTLR to enable fast page erase
    FLASH->CTLR |= CR_PAGE_ER;

    // Write the address to the flash addr 
    FLASH->ADDR = addr;

    // Set the STRT bit in CTLR to 1 to initiate erase
    FLASH->CTLR |= CR_STRT_Set;

    // Wait for BSY bit == 0 or EOP bit == 1 in STATR
    while(FLASH->STATR & SR_BSY);

    // Clear EOP bit
    
    // Query STATR to see if there is an error

    // End erase, set FTER bit in CTLR to 0.  
    FLASH->CTLR &= ~CR_PAGE_ER;
}

uint8_t flash_write_page(uint32_t addr, uint32_t* buf)
{
    uint8_t status = 0u;
    uint8_t num_words;
    uint32_t tmp_addr = addr;

    // Check the lock bit and fast lock in CTLR
    if((FLASH->CTLR & CR_LOCK_Set) ||
        (FLASH->CTLR & CR_FAST_LOCK_Set)) return 1u;

    // Set the FTPG bit to enable fast programming
    FLASH->CTLR |= CR_PAGE_PG;

    // Set the BUFRST bit to clear internal 64byte buf
    FLASH->CTLR |= CR_BUF_RST;

    // Wait for BSY bit to become 0
    while(FLASH->STATR & SR_BSY);

    // FLASH->STATR &= ~SR_EOP;

    num_words = 16u;
    while(num_words)
    {
        // Write 4bytes of data
        *(__IO uint32_t *)(addr) = *(uint32_t *)buf;

        // Set bufload bit 
        FLASH->CTLR |= CR_BUF_LOAD;

        // wait for BSY bit
        while(FLASH->STATR & SR_BSY);

        // if(!(FLASH->STATR & SR_EOP))
        // {
        //     return 1u;
        // }
        // FLASH->STATR &= ~SR_EOP;

        addr += 4u;
        buf += 1u;
        num_words -= 1u;
    }

    FLASH->CTLR |= CR_PAGE_PG;
    FLASH->ADDR = tmp_addr;
    FLASH->CTLR |= CR_STRT_Set;
    while(FLASH->STATR & SR_BSY);
    FLASH->CTLR &= ~CR_PAGE_PG;

    return status;
}
