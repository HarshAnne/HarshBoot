#include <stdint.h>

#define IMAGE_MAGIC 0xCAFE

typedef enum
{
    IMAGE_SLOT_1 = 1,
    IMAGE_SLOT_2 = 2,
    IMAGE_NUM_SLOTS
} image_slot_t;

typedef struct __attribute__((packed))
{
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
} image_version_t;

typedef struct __attribute__((packed))
{
    uint16_t magic;
    uint32_t vector_addr;
    uint16_t header_version;
    uint16_t crc;
    uint16_t image_size;
    uint8_t image_type;
    image_version_t version;
    uint32_t reserved;
    uint8_t git_hash[8];
} image_header_t;

const image_header_t * image_get_header(image_slot_t slot);
uint8_t image_valid(const image_header_t *header);
void image_start(const image_header_t *header);
