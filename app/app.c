#include "ch32v003harsh.h"
#include "memmap.h"
#include "image.h"

extern void InterruptVector();

int main(void);
extern int puts(const char*);

image_header_t header __attribute__((section(".image_header"), used)) = {
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

    puts("App");

    while(1);
}
