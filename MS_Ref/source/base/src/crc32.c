/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#include "crc32.h"

#ifdef BOOT_CRC
uint32 vds_crc32_table[256] = {0};
static uint8 gCrc8Table[256] = {0};
static uint8 gCrc32TableInited = 0;
static uint8 gCrc8TableInited = 0;
#else
const uint32 vds_crc32_table[256] = {
    0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL,
    0x076dc419L, 0x706af48fL, 0xe963a535L, 0x9e6495a3L,
    0x0edb8832L, 0x79dcb8a4L, 0xe0d5e91eL, 0x97d2d988L,
    0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L, 0x90bf1d91L,
    0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
    0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L,
    0x136c9856L, 0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL,
    0x14015c4fL, 0x63066cd9L, 0xfa0f3d63L, 0x8d080df5L,
    0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L, 0xa2677172L,
    0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
    0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L,
    0x32d86ce3L, 0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L,
    0x26d930acL, 0x51de003aL, 0xc8d75180L, 0xbfd06116L,
    0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L, 0xb8bda50fL,
    0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
    0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL,
    0x76dc4190L, 0x01db7106L, 0x98d220bcL, 0xefd5102aL,
    0x71b18589L, 0x06b6b51fL, 0x9fbfe4a5L, 0xe8b8d433L,
    0x7807c9a2L, 0x0f00f934L, 0x9609a88eL, 0xe10e9818L,
    0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
    0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL,
    0x6c0695edL, 0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L,
    0x65b0d9c6L, 0x12b7e950L, 0x8bbeb8eaL, 0xfcb9887cL,
    0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L, 0xfbd44c65L,
    0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
    0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL,
    0x4369e96aL, 0x346ed9fcL, 0xad678846L, 0xda60b8d0L,
    0x44042d73L, 0x33031de5L, 0xaa0a4c5fL, 0xdd0d7cc9L,
    0x5005713cL, 0x270241aaL, 0xbe0b1010L, 0xc90c2086L,
    0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
    0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L,
    0x59b33d17L, 0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL,
    0xedb88320L, 0x9abfb3b6L, 0x03b6e20cL, 0x74b1d29aL,
    0xead54739L, 0x9dd277afL, 0x04db2615L, 0x73dc1683L,
    0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
    0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L,
    0xf00f9344L, 0x8708a3d2L, 0x1e01f268L, 0x6906c2feL,
    0xf762575dL, 0x806567cbL, 0x196c3671L, 0x6e6b06e7L,
    0xfed41b76L, 0x89d32be0L, 0x10da7a5aL, 0x67dd4accL,
    0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
    0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L,
    0xd1bb67f1L, 0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL,
    0xd80d2bdaL, 0xaf0a1b4cL, 0x36034af6L, 0x41047a60L,
    0xdf60efc3L, 0xa867df55L, 0x316e8eefL, 0x4669be79L,
    0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
    0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL,
    0xc5ba3bbeL, 0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L,
    0xc2d7ffa7L, 0xb5d0cf31L, 0x2cd99e8bL, 0x5bdeae1dL,
    0x9b64c2b0L, 0xec63f226L, 0x756aa39cL, 0x026d930aL,
    0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
    0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L,
    0x92d28e9bL, 0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L,
    0x86d3d2d4L, 0xf1d4e242L, 0x68ddb3f8L, 0x1fda836eL,
    0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L, 0x18b74777L,
    0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
    0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L,
    0xa00ae278L, 0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L,
    0xa7672661L, 0xd06016f7L, 0x4969474dL, 0x3e6e77dbL,
    0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L, 0x37d83bf0L,
    0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
    0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L,
    0xbad03605L, 0xcdd70693L, 0x54de5729L, 0x23d967bfL,
    0xb3667a2eL, 0xc4614ab8L, 0x5d681b02L, 0x2a6f2b94L,
    0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL, 0x2d02ef8dL};
#endif

#ifdef BOOT_CRC
void crc_table_init(uint8 crc_width)
{
    unsigned short i;
    unsigned char j;
    unsigned int poly, value;
    unsigned int valid_bits = (2 << (crc_width)) - 1;

    poly = 0;
    if (crc_width == 32)
    {
        poly = 0xEDB88320;
    }
    else if (crc_width == 8)
    {
        poly = 0xE0;
    }

    for (i = 0; i < 256; i++)
    {
        value = i;
        for (j = 0; j < 8; j++)
        {
            if (value & 1)
                value = (value >> 1) ^ poly;
            else
                value = (value >> 1);
        }
        if (crc_width == 32)
        {
            vds_crc32_table[i] = value & valid_bits;
        }
        else if (crc_width == 8)
        {
            gCrc8Table[i] = (uint8)(value & valid_bits);
        }
    }
}
#endif
uint32 crc32Init(void)
{
#ifdef BOOT_CRC
    if (gCrc32TableInited == 0)
    {
        crc_table_init(32);
        gCrc32TableInited = 1;
    }
#endif
    return 0;
}
#if 0
uint32 crc32Update(uint32 crc, const void *p, size_t len)
{
   uint8 *pc = NULL;
    if (p == NULL || len == 0)
        return crc;

    crc32Init();
    crc ^= 0xffffffff;
    const uint8 *pc = (const uint8 *)p;
    while (len-- > 0)
        crc = vds_crc32_table[(crc ^ *pc++) & 0xff] ^ (crc >> 8);
    return crc ^ 0xffffffff;
}
#endif
uint32 crc32Calc(const void *p, size_t len)
{
    uint32 crc  = 0;
    uint8 *pc = NULL;
    if (p == NULL || len == 0)
        return 0;

    crc32Init();
    crc = 0 ^ 0xffffffff;
    pc = (const uint8 *)p;
    while (len-- > 0)
        crc = vds_crc32_table[(crc ^ *pc++) & 0xff] ^ (crc >> 8);
    return crc ^ 0xffffffff;
}
#if 0
uint32 crc32VariantInit(void)
{
#ifdef BOOT_CRC
    if (gCrc32TableInited == 0)
    {
        crc_table_init(32);
        gCrc32TableInited = 1;
    }
#endif
    return 0;
}

uint32 crc32VariantUpdate(uint32 crc, const void *p, size_t len)
{
    if (p == NULL || len == 0)
        return crc;

    crc32VariantInit();
    const uint8 *pc = (const uint8 *)p;
    while (len-- > 0)
        crc = vds_crc32_table[(crc ^ *pc++) & 0xff] ^ (crc >> 8);
    return crc;
}

uint32 crc32VariantCalc(const void *p, size_t len)
{
    if (p == NULL || len == 0)
        return 0;

    crc32VariantInit();
    uint32 crc = 0;
    const uint8 *pc = (const uint8 *)p;
    while (len-- > 0)
        crc = vds_crc32_table[(crc ^ *pc++) & 0xff] ^ (crc >> 8);
    return crc;
}

#ifndef BOOT_CRC
static const uint8 gCrc8Table[256] = {
    0x00, 0x91, 0xE3, 0x72, 0x07, 0x96, 0xE4, 0x75, 0x0E, 0x9F, 0xED, 0x7C,
    0x09, 0x98, 0xEA, 0x7B, 0x1C, 0x8D, 0xFF, 0x6E, 0x1B, 0x8A, 0xF8, 0x69,
    0x12, 0x83, 0xF1, 0x60, 0x15, 0x84, 0xF6, 0x67, 0x38, 0xA9, 0xDB, 0x4A,
    0x3F, 0xAE, 0xDC, 0x4D, 0x36, 0xA7, 0xD5, 0x44, 0x31, 0xA0, 0xD2, 0x43,
    0x24, 0xB5, 0xC7, 0x56, 0x23, 0xB2, 0xC0, 0x51, 0x2A, 0xBB, 0xC9, 0x58,
    0x2D, 0xBC, 0xCE, 0x5F, 0x70, 0xE1, 0x93, 0x02, 0x77, 0xE6, 0x94, 0x05,
    0x7E, 0xEF, 0x9D, 0x0C, 0x79, 0xE8, 0x9A, 0x0B, 0x6C, 0xFD, 0x8F, 0x1E,
    0x6B, 0xFA, 0x88, 0x19, 0x62, 0xF3, 0x81, 0x10, 0x65, 0xF4, 0x86, 0x17,
    0x48, 0xD9, 0xAB, 0x3A, 0x4F, 0xDE, 0xAC, 0x3D, 0x46, 0xD7, 0xA5, 0x34,
    0x41, 0xD0, 0xA2, 0x33, 0x54, 0xC5, 0xB7, 0x26, 0x53, 0xC2, 0xB0, 0x21,
    0x5A, 0xCB, 0xB9, 0x28, 0x5D, 0xCC, 0xBE, 0x2F, 0xE0, 0x71, 0x03, 0x92,
    0xE7, 0x76, 0x04, 0x95, 0xEE, 0x7F, 0x0D, 0x9C, 0xE9, 0x78, 0x0A, 0x9B,
    0xFC, 0x6D, 0x1F, 0x8E, 0xFB, 0x6A, 0x18, 0x89, 0xF2, 0x63, 0x11, 0x80,
    0xF5, 0x64, 0x16, 0x87, 0xD8, 0x49, 0x3B, 0xAA, 0xDF, 0x4E, 0x3C, 0xAD,
    0xD6, 0x47, 0x35, 0xA4, 0xD1, 0x40, 0x32, 0xA3, 0xC4, 0x55, 0x27, 0xB6,
    0xC3, 0x52, 0x20, 0xB1, 0xCA, 0x5B, 0x29, 0xB8, 0xCD, 0x5C, 0x2E, 0xBF,
    0x90, 0x01, 0x73, 0xE2, 0x97, 0x06, 0x74, 0xE5, 0x9E, 0x0F, 0x7D, 0xEC,
    0x99, 0x08, 0x7A, 0xEB, 0x8C, 0x1D, 0x6F, 0xFE, 0x8B, 0x1A, 0x68, 0xF9,
    0x82, 0x13, 0x61, 0xF0, 0x85, 0x14, 0x66, 0xF7, 0xA8, 0x39, 0x4B, 0xDA,
    0xAF, 0x3E, 0x4C, 0xDD, 0xA6, 0x37, 0x45, 0xD4, 0xA1, 0x30, 0x42, 0xD3,
    0xB4, 0x25, 0x57, 0xC6, 0xB3, 0x22, 0x50, 0xC1, 0xBA, 0x2B, 0x59, 0xC8,
    0xBD, 0x2C, 0x5E, 0xCF //
};
#endif

uint8 crc8RohcInit(void)
{
#ifdef BOOT_CRC
    if (gCrc8TableInited == 0)
    {
        crc_table_init(8);
        gCrc8TableInited = 1;
    }
#endif
    return 0xff;
}

uint8 crc8RohcUpdate(uint8 crc, const void *p, size_t size)
{
    if (p == NULL || size == 0)
        return crc;

    crc8RohcInit();
    const uint8 *pc = (const uint8 *)p;
    while (size-- > 0)
        crc = gCrc8Table[(crc ^ *pc++) & 0xff];
    return crc;
}

uint8 crc8RohcCalc(const void *p, size_t size)
{
    if (p == NULL || size == 0)
        return 0xff;

    crc8RohcInit();
    uint8 crc = 0xff;
    const uint8 *pc = (const uint8 *)p;
    while (size-- > 0)
        crc = gCrc8Table[(crc ^ *pc++) & 0xff];
    return crc;
}

uint16_t crc16NvCalc(const void *p, size_t len)
{
    const uint8 *pc = (const uint8 *)p;
    uint16_t crc = 0;
    uint16_t crc2 = 0;
    uint16_t c = 0;
    uint16_t j = 0;

    while (len-- > 0)
    {
        crc2 = 0;
        c = (crc ^ (*pc++)) & 0xff;
        for (j = 0; j < 8; j++)
        {
            if ((crc2 ^ c) & 0x0001)
            {
                crc2 = (crc2 >> 1) ^ 0xA001;
            }
            else
            {
                crc2 = crc2 >> 1;
            }
            c = c >> 1;
        }
        crc = (uint16_t)((crc >> 8) ^ crc2);
    }
    return crc;
}

#define CRC_16_POLYNOMIAL 0x1021
#define CRC_16_L_POLYNOMIAL 0x8000
#define CRC_16_L_SEED 0x80
#define CRC_16_L_OK 0x00

uint16_t crc16RomBslCalc(const void *p, unsigned len)
{
    const uint8 *ptr = (const uint8 *)p;
    uint16_t crc = 0;

    while (len-- > 0)
    {
        for (unsigned i = CRC_16_L_SEED; i != 0; i = i >> 1)
        {
            if ((crc & CRC_16_L_POLYNOMIAL) != 0)
            {
                crc = crc << 1;
                crc = crc ^ CRC_16_POLYNOMIAL;
            }
            else
            {
                crc = crc << 1;
            }
            if ((*ptr & i) != 0)
            {
                crc = crc ^ CRC_16_POLYNOMIAL;
            }
        }
        ptr++;
    }

    return (crc);
}

bool crc16RomBslCheck(const void *p, unsigned size)
{
    return crc16RomBslCalc(p, size) == CRC_16_L_OK;
}

uint16_t crc16FdlCalc(const uint16_t *src, unsigned len)
{
    uint32 sum = 0;

    while (len > 3)
    {
        sum += *src++;
        sum += *src++;
        len -= 4;
    }

    switch (len & 0x03)
    {
    case 2:
        sum += *src++;
        break;

    case 3:
        sum += *src++;
        sum += *((uint8 *)src);
        break;

    case 1:
        sum += *((uint8 *)src);
        break;

    default:
        break;
    }

    sum = (sum >> 16) + (sum & 0x0FFFF);
    sum += (sum >> 16);
    return (~sum);
}
#endif
