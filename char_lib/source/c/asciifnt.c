#include "sci_types.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

		

static const unsigned char FONT_ASCII_5X8[]=
{/*
    ASCII Font Data(0x20 -- 0x79)
    Format(Width * Height = 5 * 8), Lognitudinal:  
        5 byte per char. 
        the most left and most top pixel == the bit 0 of the first byte.
    Font Type : Small Fonts 
*/
0x00, 0x00, 0x00, 0x00, 0x00,  //   
0x00, 0x5C, 0x00, 0x00, 0x00,  // ! 
0x00, 0x0C, 0x0C, 0x00, 0x00,  // " 
0x00, 0x28, 0x7C, 0x7C, 0x28,  // # 
0x00, 0x58, 0xFC, 0x68, 0x00,  // $ 
0x00, 0x48, 0x20, 0x10, 0x48,  // % 
0x00, 0x28, 0x54, 0x28, 0x50,  // & 
0x00, 0x0C, 0x00, 0x00, 0x00,  // ' 
0x00, 0x78, 0x84, 0x00, 0x00,  // ( 
0x00, 0x84, 0x78, 0x00, 0x00,  // ) 
0x00, 0x0C, 0x0C, 0x00, 0x00,  // * 
0x00, 0x10, 0x38, 0x10, 0x00,  // + 
0x00, 0xC0, 0x00, 0x00, 0x00,  // , 
0x00, 0x10, 0x10, 0x00, 0x00,  // - 
0x00, 0x40, 0x00, 0x00, 0x00,  // . 
0x00, 0x70, 0x1C, 0x00, 0x00,  // / 
0x00, 0x38, 0x44, 0x38, 0x00,  // 0 
0x00, 0x08, 0x7C, 0x00, 0x00,  // 1 
0x00, 0x64, 0x54, 0x48, 0x00,  // 2 
0x00, 0x44, 0x54, 0x28, 0x00,  // 3 
0x00, 0x30, 0x28, 0x7C, 0x00,  // 4 
0x00, 0x5C, 0x54, 0x34, 0x00,  // 5 
0x00, 0x38, 0x54, 0x20, 0x00,  // 6 
0x00, 0x04, 0x74, 0x0C, 0x00,  // 7 
0x00, 0x28, 0x54, 0x28, 0x00,  // 8 
0x00, 0x18, 0x54, 0x38, 0x00,  // 9 
0x00, 0x50, 0x00, 0x00, 0x00,  // : 
0x00, 0xD0, 0x00, 0x00, 0x00,  // ; 
0x00, 0x20, 0x50, 0x00, 0x00,  // < 
0x00, 0x20, 0x20, 0x00, 0x00,  // = 
0x00, 0x50, 0x20, 0x00, 0x00,  // > 
0x00, 0x54, 0x08, 0x00, 0x00,  // ? 
0x00, 0x30, 0x48, 0xB4, 0xB4,  // @ 
0x00, 0x70, 0x2C, 0x2C, 0x70,  // A 
0x00, 0x7C, 0x54, 0x54, 0x28,  // B 
0x00, 0x38, 0x44, 0x44, 0x28,  // C 
0x00, 0x7C, 0x44, 0x44, 0x38,  // D 
0x00, 0x7C, 0x54, 0x44, 0x00,  // E 
0x00, 0x7C, 0x14, 0x04, 0x00,  // F 
0x00, 0x38, 0x44, 0x54, 0x74,  // G 
0x00, 0x7C, 0x10, 0x10, 0x7C,  // H 
0x00, 0x7C, 0x00, 0x00, 0x00,  // I 
0x00, 0x60, 0x40, 0x3C, 0x00,  // J 
0x00, 0x7C, 0x10, 0x28, 0x44,  // K 
0x00, 0x7C, 0x40, 0x40, 0x00,  // L 
0x7C, 0x08, 0x10, 0x08, 0x7C,  // M 
0x00, 0x7C, 0x08, 0x10, 0x7C,  // N 
0x00, 0x38, 0x44, 0x44, 0x38,  // O 
0x00, 0x7C, 0x14, 0x14, 0x08,  // P 
0x00, 0x38, 0x44, 0x24, 0x58,  // Q 
0x00, 0x7C, 0x14, 0x14, 0x68,  // R 
0x00, 0x48, 0x54, 0x54, 0x24,  // S 
0x00, 0x04, 0x7C, 0x04, 0x00,  // T 
0x00, 0x3C, 0x40, 0x40, 0x3C,  // U 
0x00, 0x1C, 0x60, 0x1C, 0x00,  // V 
0x00, 0x1C, 0x60, 0x18, 0x60,  // W 
0x00, 0x6C, 0x10, 0x6C, 0x00,  // X 
0x00, 0x0C, 0x70, 0x0C, 0x00,  // Y 
0x00, 0x64, 0x54, 0x4C, 0x00,  // Z 
0x00, 0xFC, 0x84, 0x00, 0x00,  // [ 
0x00, 0x1C, 0x70, 0x00, 0x00,  // \ 
0x00, 0x84, 0xFC, 0x00, 0x00,  // ] 
0x00, 0x08, 0x04, 0x08, 0x00,  // ^ 
0x80, 0x80, 0x80, 0x80, 0x00,  // _ 
0x00, 0x04, 0x08, 0x00, 0x00,  // ` 
0x00, 0x20, 0x50, 0x70, 0x00,  // a 
0x00, 0x7C, 0x50, 0x20, 0x00,  // b 
0x00, 0x20, 0x50, 0x50, 0x00,  // c 
0x00, 0x20, 0x50, 0x7C, 0x00,  // d 
0x00, 0x60, 0x50, 0x20, 0x00,  // e 
0x00, 0x78, 0x14, 0x00, 0x00,  // f 
0x00, 0x20, 0x50, 0xF0, 0x00,  // g 
0x00, 0x7C, 0x10, 0x60, 0x00,  // h 
0x00, 0x74, 0x00, 0x00, 0x00,  // i 
0x00, 0xF4, 0x00, 0x00, 0x00,  // j 
0x00, 0x7C, 0x20, 0x50, 0x00,  // k 
0x00, 0x7C, 0x00, 0x00, 0x00,  // l 
0x00, 0x70, 0x10, 0x70, 0x10,  // m 
0x00, 0x70, 0x10, 0x60, 0x00,  // n 
0x00, 0x70, 0x50, 0x70, 0x00,  // o 
0x00, 0xF0, 0x50, 0x20, 0x00,  // p 
0x00, 0x20, 0x50, 0xF0, 0x00,  // q 
0x00, 0x70, 0x10, 0x00, 0x00,  // r 
0x40, 0x70, 0x10, 0x00, 0x00,  // s 
0x00, 0x38, 0x50, 0x00, 0x00,  // t 
0x00, 0x30, 0x40, 0x70, 0x00,  // u 
0x00, 0x30, 0x40, 0x30, 0x00,  // v 
0x00, 0x70, 0x40, 0x40, 0x70,  // w 
0x00, 0x50, 0x20, 0x50, 0x00,  // x 
0x00, 0x90, 0x60, 0x10, 0x00,  // y 
0x00, 0x50, 0x70, 0x50, 0x00,  // z 
0x00, 0x30, 0xCC, 0x00, 0x00,  // { 
0x00, 0x7C, 0x00, 0x00, 0x00,  // | 
0x00, 0xCC, 0x30, 0x00, 0x00,  // } 
0x00, 0x08, 0x0C, 0x04, 0x00,  // ~ 
0x00, 0x7C, 0x00, 0x00, 0x00,  //  

};




static const uint8 FONT_ASCII_8X16[]=
{
    // Short Message Symbol(0x0--0x1F)
0xF8 ,0x07 ,0x04 ,0x08 ,0xE2 ,0x11 ,0x12 ,0x12 ,
0x12 ,0x12 ,0x22 ,0x09 ,0xFC ,0x0B ,0x00 ,0x00 ,    // 0x00 
0x40 ,0x18 ,0x40 ,0x14 ,0xF8 ,0x0F ,0x44 ,0x10 ,
0x44 ,0x10 ,0x48 ,0x08 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x01 
0x30 ,0x06 ,0x48 ,0x08 ,0xFC ,0x1F ,0x88 ,0x08 ,
0x30 ,0x07 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x02 
0x44 ,0x02 ,0x5C ,0x12 ,0xE0 ,0x1F ,0x5C ,0x12 ,
0x44 ,0x02 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x03 
0x80 ,0x07 ,0x40 ,0x09 ,0x48 ,0x09 ,0x44 ,0x09 ,
0x80 ,0x05 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x04 
0x80 ,0x07 ,0x44 ,0x09 ,0x48 ,0x09 ,0x40 ,0x09 ,
0x80 ,0x05 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x05 
0xC0 ,0x07 ,0x04 ,0x08 ,0x08 ,0x08 ,0xC0 ,0x07 ,
0x00 ,0x08 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x06 
0x00 ,0x00 ,0x04 ,0x00 ,0xC8 ,0x0F ,0x00 ,0x00 ,
0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x07 
0x80 ,0x07 ,0x44 ,0x08 ,0x48 ,0x08 ,0x40 ,0x08 ,
0x80 ,0x07 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x08 
0xF8 ,0x00 ,0x04 ,0x11 ,0x04 ,0x17 ,0x04 ,0x15 ,
0x04 ,0x09 ,0x88 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x09 
0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,
0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x0A 
0xE0 ,0x1B ,0x10 ,0x06 ,0x90 ,0x05 ,0x50 ,0x04 ,
0x30 ,0x04 ,0xEC ,0x03 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x0B 
0xC0 ,0x01 ,0x20 ,0x06 ,0xA0 ,0x03 ,0x60 ,0x02 ,
0x30 ,0x02 ,0xC0 ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x0C 
0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,
0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x0D 
0x08 ,0x0F ,0xD4 ,0x01 ,0x34 ,0x01 ,0xC8 ,0x01 ,
0x00 ,0x0F ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x0E 
0x88 ,0x06 ,0x54 ,0x09 ,0x54 ,0x09 ,0x88 ,0x0F ,
0x00 ,0x08 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x0F 
0x80 ,0x1F ,0x70 ,0x10 ,0x0C ,0x10 ,0x70 ,0x10 ,
0x80 ,0x1F ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x10 
0x00 ,0x10 ,0x00 ,0x10 ,0x00 ,0x10 ,0x00 ,0x10 ,
0x00 ,0x10 ,0x00 ,0x10 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x11 
0xE0 ,0x03 ,0x14 ,0x14 ,0xFC ,0x1F ,0x14 ,0x14 ,
0xE0 ,0x03 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x12 
0x04 ,0x10 ,0xFC ,0x1F ,0x04 ,0x10 ,0x04 ,0x00 ,
0x04 ,0x00 ,0x0C ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x13 
0x80 ,0x1F ,0x70 ,0x00 ,0x0C ,0x00 ,0x70 ,0x00 ,
0x80 ,0x1F ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x14 
0xF8 ,0x13 ,0x04 ,0x1C ,0x04 ,0x00 ,0x04 ,0x00 ,
0x04 ,0x1C ,0xF8 ,0x13 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x15 
0x04 ,0x10 ,0xFC ,0x1F ,0x04 ,0x00 ,0x04 ,0x00 ,
0xFC ,0x1F ,0x04 ,0x10 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x16 
0xFC ,0x01 ,0x00 ,0x12 ,0xFC ,0x1F ,0x00 ,0x12 ,
0xFC ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x17 
0x0C ,0x18 ,0x14 ,0x14 ,0x24 ,0x12 ,0x44 ,0x11 ,
0x84 ,0x10 ,0x0C ,0x18 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x18 
0xF8 ,0x0F ,0xC4 ,0x11 ,0x84 ,0x10 ,0x84 ,0x10 ,
0xC4 ,0x11 ,0xF8 ,0x0F ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x19 
0x0C ,0x18 ,0xC4 ,0x11 ,0x84 ,0x10 ,0x84 ,0x10 ,
0xC4 ,0x11 ,0x0C ,0x18 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x1A 
0x00 ,0x00 ,0x10 ,0x10 ,0xF8 ,0x1F ,0x00 ,0x10 ,
0x04 ,0x20 ,0x18 ,0x18 ,0xE0 ,0x07 ,0x00 ,0x00 ,    // 0x1B 
0x04 ,0x1F ,0xE4 ,0x01 ,0x1C ,0x01 ,0xFC ,0x1F ,
0x84 ,0x10 ,0x84 ,0x10 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x1C 
0x30 ,0x07 ,0x88 ,0x08 ,0xF8 ,0x07 ,0xF0 ,0x0F ,
0x88 ,0x08 ,0xF0 ,0x06 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x1D 
0xF8 ,0x1F ,0x44 ,0x04 ,0x44 ,0x08 ,0x44 ,0x08 ,
0x44 ,0x08 ,0xB8 ,0x07 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x1E 
0xE0 ,0x0F ,0x28 ,0x09 ,0x24 ,0x09 ,0x20 ,0x09 ,
0x20 ,0x08 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,    // 0x1F 


/*
    ASCII Font Data(0x20 -- 0x79)
    Format(Width * Height = 8 * 16), Lognitudinal:  
        16 byte per char. 
        the most left and most top pixel == the bit 0 of the first byte.
    Font Type : ���� 
*/
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x33, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x10, 0x00, 0x0C, 0x00, 0x06, 0x00, 0x10, 0x00, 0x0C, 0x00, 0x06, 0x00, 0x00, 0x00, 
0x40, 0x04, 0xC0, 0x3F, 0x78, 0x04, 0x40, 0x04, 0xC0, 0x3F, 0x78, 0x04, 0x40, 0x04, 0x00, 0x00, 
0x00, 0x00, 0x70, 0x18, 0x88, 0x20, 0xFC, 0xFF, 0x08, 0x21, 0x30, 0x1E, 0x00, 0x00, 0x00, 0x00, 
0xF0, 0x00, 0x08, 0x21, 0xF0, 0x1C, 0x00, 0x03, 0xE0, 0x1E, 0x18, 0x21, 0x00, 0x1E, 0x00, 0x00, 
0x00, 0x1E, 0xF0, 0x21, 0x08, 0x23, 0x88, 0x24, 0x70, 0x19, 0x00, 0x27, 0x00, 0x21, 0x00, 0x10, 
0x10, 0x00, 0x16, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x07, 0x18, 0x18, 0x04, 0x20, 0x02, 0x40, 0x00, 0x00, 
0x00, 0x00, 0x02, 0x40, 0x04, 0x20, 0x18, 0x18, 0xE0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x40, 0x02, 0x40, 0x02, 0x80, 0x01, 0xF0, 0x0F, 0x80, 0x01, 0x40, 0x02, 0x40, 0x02, 0x00, 0x00, 
0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0xF0, 0x1F, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 
0x00, 0x80, 0x00, 0xB0, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 
0x00, 0x00, 0x00, 0x30, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x60, 0x00, 0x18, 0x00, 0x06, 0x80, 0x01, 0x60, 0x00, 0x18, 0x00, 0x04, 0x00, 
0x00, 0x00, 0xE0, 0x0F, 0x10, 0x10, 0x08, 0x20, 0x08, 0x20, 0x10, 0x10, 0xE0, 0x0F, 0x00, 0x00, 
0x00, 0x00, 0x10, 0x20, 0x10, 0x20, 0xF8, 0x3F, 0x00, 0x20, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x70, 0x30, 0x08, 0x28, 0x08, 0x24, 0x08, 0x22, 0x88, 0x21, 0x70, 0x30, 0x00, 0x00, 
0x00, 0x00, 0x30, 0x18, 0x08, 0x20, 0x88, 0x20, 0x88, 0x20, 0x48, 0x11, 0x30, 0x0E, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x07, 0xC0, 0x04, 0x20, 0x24, 0x10, 0x24, 0xF8, 0x3F, 0x00, 0x24, 0x00, 0x00, 
0x00, 0x00, 0xF8, 0x19, 0x08, 0x21, 0x88, 0x20, 0x88, 0x20, 0x08, 0x11, 0x08, 0x0E, 0x00, 0x00, 
0x00, 0x00, 0xE0, 0x0F, 0x10, 0x11, 0x88, 0x20, 0x88, 0x20, 0x18, 0x11, 0x00, 0x0E, 0x00, 0x00, 
0x00, 0x00, 0x38, 0x00, 0x08, 0x00, 0x08, 0x3F, 0xC8, 0x00, 0x38, 0x00, 0x08, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x70, 0x1C, 0x88, 0x22, 0x08, 0x21, 0x08, 0x21, 0x88, 0x22, 0x70, 0x1C, 0x00, 0x00, 
0x00, 0x00, 0xE0, 0x00, 0x10, 0x31, 0x08, 0x22, 0x08, 0x22, 0x10, 0x11, 0xE0, 0x0F, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x30, 0xC0, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x01, 0x80, 0x02, 0x40, 0x04, 0x20, 0x08, 0x10, 0x10, 0x08, 0x20, 0x00, 0x00, 
0x40, 0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 0x04, 0x00, 0x00, 
0x00, 0x00, 0x08, 0x20, 0x10, 0x10, 0x20, 0x08, 0x40, 0x04, 0x80, 0x02, 0x00, 0x01, 0x00, 0x00, 
0x00, 0x00, 0x70, 0x00, 0x48, 0x00, 0x08, 0x30, 0x08, 0x36, 0x08, 0x01, 0xF0, 0x00, 0x00, 0x00, 
0xC0, 0x07, 0x30, 0x18, 0xC8, 0x27, 0x28, 0x24, 0xE8, 0x23, 0x10, 0x14, 0xE0, 0x0B, 0x00, 0x00, 
0x00, 0x20, 0x00, 0x3C, 0xC0, 0x23, 0x38, 0x02, 0xE0, 0x02, 0x00, 0x27, 0x00, 0x38, 0x00, 0x20, 
0x08, 0x20, 0xF8, 0x3F, 0x88, 0x20, 0x88, 0x20, 0x88, 0x20, 0x70, 0x11, 0x00, 0x0E, 0x00, 0x00, 
0xC0, 0x07, 0x30, 0x18, 0x08, 0x20, 0x08, 0x20, 0x08, 0x20, 0x08, 0x10, 0x38, 0x08, 0x00, 0x00, 
0x08, 0x20, 0xF8, 0x3F, 0x08, 0x20, 0x08, 0x20, 0x08, 0x20, 0x10, 0x10, 0xE0, 0x0F, 0x00, 0x00, 
0x08, 0x20, 0xF8, 0x3F, 0x88, 0x20, 0x88, 0x20, 0xE8, 0x23, 0x08, 0x20, 0x10, 0x18, 0x00, 0x00, 
0x08, 0x20, 0xF8, 0x3F, 0x88, 0x20, 0x88, 0x00, 0xE8, 0x03, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 
0xC0, 0x07, 0x30, 0x18, 0x08, 0x20, 0x08, 0x20, 0x08, 0x22, 0x38, 0x1E, 0x00, 0x02, 0x00, 0x00, 
0x08, 0x20, 0xF8, 0x3F, 0x08, 0x21, 0x00, 0x01, 0x00, 0x01, 0x08, 0x21, 0xF8, 0x3F, 0x08, 0x20, 
0x00, 0x00, 0x08, 0x20, 0x08, 0x20, 0xF8, 0x3F, 0x08, 0x20, 0x08, 0x20, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xC0, 0x00, 0x80, 0x08, 0x80, 0x08, 0x80, 0xF8, 0x7F, 0x08, 0x00, 0x08, 0x00, 0x00, 0x00, 
0x08, 0x20, 0xF8, 0x3F, 0x88, 0x20, 0xC0, 0x01, 0x28, 0x26, 0x18, 0x38, 0x08, 0x20, 0x00, 0x00, 
0x08, 0x20, 0xF8, 0x3F, 0x08, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x30, 0x00, 0x00, 
0x08, 0x20, 0xF8, 0x3F, 0xF8, 0x00, 0x00, 0x3F, 0xF8, 0x00, 0xF8, 0x3F, 0x08, 0x20, 0x00, 0x00, 
0x08, 0x20, 0xF8, 0x3F, 0x30, 0x20, 0xC0, 0x00, 0x00, 0x07, 0x08, 0x18, 0xF8, 0x3F, 0x08, 0x00, 
0xE0, 0x0F, 0x10, 0x10, 0x08, 0x20, 0x08, 0x20, 0x08, 0x20, 0x10, 0x10, 0xE0, 0x0F, 0x00, 0x00, 
0x08, 0x20, 0xF8, 0x3F, 0x08, 0x21, 0x08, 0x01, 0x08, 0x01, 0x08, 0x01, 0xF0, 0x00, 0x00, 0x00, 
0xE0, 0x0F, 0x10, 0x18, 0x08, 0x24, 0x08, 0x24, 0x08, 0x38, 0x10, 0x50, 0xE0, 0x4F, 0x00, 0x00, 
0x08, 0x20, 0xF8, 0x3F, 0x88, 0x20, 0x88, 0x00, 0x88, 0x03, 0x88, 0x0C, 0x70, 0x30, 0x00, 0x20, 
0x00, 0x00, 0x70, 0x38, 0x88, 0x20, 0x08, 0x21, 0x08, 0x21, 0x08, 0x22, 0x38, 0x1C, 0x00, 0x00, 
0x18, 0x00, 0x08, 0x00, 0x08, 0x20, 0xF8, 0x3F, 0x08, 0x20, 0x08, 0x00, 0x18, 0x00, 0x00, 0x00, 
0x08, 0x00, 0xF8, 0x1F, 0x08, 0x20, 0x00, 0x20, 0x00, 0x20, 0x08, 0x20, 0xF8, 0x1F, 0x08, 0x00, 
0x08, 0x00, 0x78, 0x00, 0x88, 0x07, 0x00, 0x38, 0x00, 0x0E, 0xC8, 0x01, 0x38, 0x00, 0x08, 0x00, 
0xF8, 0x03, 0x08, 0x3C, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x07, 0x08, 0x3C, 0xF8, 0x03, 0x00, 0x00, 
0x08, 0x20, 0x18, 0x30, 0x68, 0x2C, 0x80, 0x03, 0x80, 0x03, 0x68, 0x2C, 0x18, 0x30, 0x08, 0x20, 
0x08, 0x00, 0x38, 0x00, 0xC8, 0x20, 0x00, 0x3F, 0xC8, 0x20, 0x38, 0x00, 0x08, 0x00, 0x00, 0x00, 
0x10, 0x20, 0x08, 0x38, 0x08, 0x26, 0x08, 0x21, 0xC8, 0x20, 0x38, 0x20, 0x08, 0x18, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x7F, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x00, 0x00, 
0x00, 0x00, 0x0C, 0x00, 0x30, 0x00, 0xC0, 0x01, 0x00, 0x06, 0x00, 0x38, 0x00, 0xC0, 0x00, 0x00, 
0x00, 0x00, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0xFE, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 
0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 
0x00, 0x00, 0x02, 0x00, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x19, 0x80, 0x24, 0x80, 0x22, 0x80, 0x22, 0x80, 0x22, 0x00, 0x3F, 0x00, 0x20, 
0x08, 0x00, 0xF8, 0x3F, 0x00, 0x11, 0x80, 0x20, 0x80, 0x20, 0x00, 0x11, 0x00, 0x0E, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x0E, 0x00, 0x11, 0x80, 0x20, 0x80, 0x20, 0x80, 0x20, 0x00, 0x11, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x0E, 0x00, 0x11, 0x80, 0x20, 0x80, 0x20, 0x88, 0x10, 0xF8, 0x3F, 0x00, 0x20, 
0x00, 0x00, 0x00, 0x1F, 0x80, 0x22, 0x80, 0x22, 0x80, 0x22, 0x80, 0x22, 0x00, 0x13, 0x00, 0x00, 
0x00, 0x00, 0x80, 0x20, 0x80, 0x20, 0xF0, 0x3F, 0x88, 0x20, 0x88, 0x20, 0x88, 0x00, 0x18, 0x00, 
0x00, 0x00, 0x00, 0x6B, 0x80, 0x94, 0x80, 0x94, 0x80, 0x94, 0x80, 0x93, 0x80, 0x60, 0x00, 0x00, 
0x08, 0x20, 0xF8, 0x3F, 0x00, 0x21, 0x80, 0x00, 0x80, 0x00, 0x80, 0x20, 0x00, 0x3F, 0x00, 0x20, 
0x00, 0x00, 0x80, 0x20, 0x98, 0x20, 0x98, 0x3F, 0x00, 0x20, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0xC0, 0x00, 0x80, 0x80, 0x80, 0x98, 0x80, 0x98, 0x7F, 0x00, 0x00, 0x00, 0x00, 
0x08, 0x20, 0xF8, 0x3F, 0x00, 0x24, 0x00, 0x02, 0x80, 0x2D, 0x80, 0x30, 0x80, 0x20, 0x00, 0x00, 
0x00, 0x00, 0x08, 0x20, 0x08, 0x20, 0xF8, 0x3F, 0x00, 0x20, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 
0x80, 0x20, 0x80, 0x3F, 0x80, 0x20, 0x80, 0x00, 0x80, 0x3F, 0x80, 0x20, 0x80, 0x00, 0x00, 0x3F, 
0x80, 0x20, 0x80, 0x3F, 0x00, 0x21, 0x80, 0x00, 0x80, 0x00, 0x80, 0x20, 0x00, 0x3F, 0x00, 0x20, 
0x00, 0x00, 0x00, 0x1F, 0x80, 0x20, 0x80, 0x20, 0x80, 0x20, 0x80, 0x20, 0x00, 0x1F, 0x00, 0x00, 
0x80, 0x80, 0x80, 0xFF, 0x00, 0xA1, 0x80, 0x20, 0x80, 0x20, 0x00, 0x11, 0x00, 0x0E, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x0E, 0x00, 0x11, 0x80, 0x20, 0x80, 0x20, 0x80, 0xA0, 0x80, 0xFF, 0x00, 0x80, 
0x80, 0x20, 0x80, 0x20, 0x80, 0x3F, 0x00, 0x21, 0x80, 0x20, 0x80, 0x00, 0x80, 0x01, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x33, 0x80, 0x24, 0x80, 0x24, 0x80, 0x24, 0x80, 0x24, 0x80, 0x19, 0x00, 0x00, 
0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0xE0, 0x1F, 0x80, 0x20, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 
0x80, 0x00, 0x80, 0x1F, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x80, 0x10, 0x80, 0x3F, 0x00, 0x20, 
0x80, 0x00, 0x80, 0x01, 0x80, 0x0E, 0x00, 0x30, 0x00, 0x08, 0x80, 0x06, 0x80, 0x01, 0x80, 0x00, 
0x80, 0x0F, 0x80, 0x30, 0x00, 0x0C, 0x80, 0x03, 0x00, 0x0C, 0x80, 0x30, 0x80, 0x0F, 0x80, 0x00, 
0x00, 0x00, 0x80, 0x20, 0x80, 0x31, 0x00, 0x2E, 0x80, 0x0E, 0x80, 0x31, 0x80, 0x20, 0x00, 0x00, 
0x80, 0x80, 0x80, 0x81, 0x80, 0x8E, 0x00, 0x70, 0x00, 0x18, 0x80, 0x06, 0x80, 0x01, 0x80, 0x00, 
0x00, 0x00, 0x80, 0x21, 0x80, 0x30, 0x80, 0x2C, 0x80, 0x22, 0x80, 0x21, 0x80, 0x30, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x7C, 0x3F, 0x02, 0x40, 0x02, 0x40, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x02, 0x40, 0x02, 0x40, 0x7C, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x06, 0x00, 0x01, 0x00, 0x01, 0x00, 0x02, 0x00, 0x02, 0x00, 0x04, 0x00, 0x04, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

};

const unsigned char FONT_ASCII_8X12[]=
{/*
    ASCII Font Data(0x20 -- 0x79)
    Format(Width * Height = 8 * 12), Lognitudinal:  
        16 byte per char. 
        the most left and most top pixel == the bit 0 of the first byte.
    Font Type : Terminal 
*/
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x7C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xD0, 0x01, 0x7C, 0x00, 0xD0, 0x01, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x98, 0x00, 0x24, 0x01, 0xFC, 0x03, 0x24, 0x01, 0xC8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x08, 0x00, 0x14, 0x00, 0xC8, 0x01, 0xB8, 0x00, 0x44, 0x01, 0x80, 0x00, 0x00, 0x00, 
0xC0, 0x00, 0x38, 0x01, 0x24, 0x01, 0x64, 0x01, 0x9C, 0x00, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 
0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xF0, 0x01, 0x08, 0x02, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x04, 0x04, 0x08, 0x02, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x14, 0x00, 0x0C, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x40, 0x00, 0x40, 0x00, 0xF0, 0x01, 0x40, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x40, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x80, 0x01, 0x70, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xF8, 0x01, 0x04, 0x01, 0x04, 0x01, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x08, 0x00, 0xFC, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x08, 0x01, 0x84, 0x01, 0x64, 0x01, 0x18, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x88, 0x00, 0x04, 0x01, 0x24, 0x01, 0xD8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x60, 0x00, 0x50, 0x00, 0x48, 0x00, 0xFC, 0x01, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xB0, 0x00, 0x1C, 0x01, 0x14, 0x01, 0xE4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xF8, 0x00, 0x14, 0x01, 0x14, 0x01, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x04, 0x00, 0xC4, 0x01, 0x34, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xD8, 0x00, 0x24, 0x01, 0x24, 0x01, 0xD8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xB8, 0x00, 0x44, 0x01, 0x44, 0x01, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x10, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x20, 0x00, 0x50, 0x00, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x50, 0x00, 0x50, 0x00, 0x50, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x88, 0x00, 0x50, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x08, 0x00, 0x04, 0x00, 0x64, 0x01, 0x24, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xF0, 0x01, 0x08, 0x02, 0xE4, 0x05, 0x14, 0x05, 0xE4, 0x05, 0x34, 0x05, 0x88, 0x02, 
0x00, 0x01, 0xE0, 0x00, 0x58, 0x00, 0x44, 0x00, 0x58, 0x00, 0xE0, 0x00, 0x00, 0x01, 0x00, 0x00, 
0x00, 0x00, 0xFC, 0x01, 0x24, 0x01, 0x24, 0x01, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xF8, 0x00, 0x04, 0x01, 0x04, 0x01, 0x04, 0x01, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xFC, 0x01, 0x04, 0x01, 0x04, 0x01, 0x04, 0x01, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xFC, 0x01, 0x24, 0x01, 0x24, 0x01, 0x24, 0x01, 0x24, 0x01, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xFC, 0x01, 0x24, 0x00, 0x24, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xF8, 0x00, 0x04, 0x01, 0x04, 0x01, 0x24, 0x01, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xFC, 0x01, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0xFC, 0x01, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xFC, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x80, 0x01, 0x00, 0x01, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xFC, 0x01, 0x20, 0x00, 0x50, 0x00, 0x88, 0x00, 0x04, 0x01, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xFC, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xFC, 0x01, 0x18, 0x00, 0xE0, 0x00, 0x00, 0x01, 0xE0, 0x00, 0x18, 0x00, 0xFC, 0x01, 0x00, 0x00, 
0x00, 0x00, 0xFC, 0x01, 0x18, 0x00, 0x20, 0x00, 0xC0, 0x00, 0xFC, 0x01, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xF8, 0x00, 0x04, 0x01, 0x04, 0x01, 0x04, 0x01, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xFC, 0x01, 0x24, 0x00, 0x24, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xF8, 0x00, 0x04, 0x01, 0x04, 0x01, 0x84, 0x01, 0x78, 0x01, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xFC, 0x01, 0x24, 0x00, 0x24, 0x00, 0x64, 0x00, 0x98, 0x01, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x98, 0x00, 0x24, 0x01, 0x24, 0x01, 0xC8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x04, 0x00, 0x04, 0x00, 0xFC, 0x01, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xFC, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x0C, 0x00, 0x30, 0x00, 0xC0, 0x00, 0x00, 0x01, 0xC0, 0x00, 0x30, 0x00, 0x0C, 0x00, 0x00, 0x00, 
0x0C, 0x00, 0xF0, 0x00, 0x00, 0x01, 0xF8, 0x00, 0x04, 0x00, 0xF8, 0x00, 0x00, 0x01, 0xF0, 0x00, 
0x04, 0x01, 0xD8, 0x00, 0x20, 0x00, 0xD8, 0x00, 0x04, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x04, 0x00, 0x18, 0x00, 0xE0, 0x01, 0x18, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x84, 0x01, 0x44, 0x01, 0x24, 0x01, 0x14, 0x01, 0x0C, 0x01, 0x04, 0x01, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xFC, 0x07, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x0C, 0x00, 0x70, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x04, 0x04, 0xFC, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x18, 0x00, 0x04, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x90, 0x01, 0x50, 0x01, 0x50, 0x01, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xFC, 0x01, 0x10, 0x01, 0x10, 0x01, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xE0, 0x00, 0x10, 0x01, 0x10, 0x01, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xE0, 0x00, 0x10, 0x01, 0x10, 0x01, 0xFC, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xE0, 0x00, 0x50, 0x01, 0x50, 0x01, 0x60, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x10, 0x00, 0xF8, 0x01, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xE0, 0x02, 0x10, 0x05, 0x10, 0x05, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xFC, 0x01, 0x10, 0x00, 0x10, 0x00, 0xE0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xF4, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xF4, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xFC, 0x01, 0x40, 0x00, 0xE0, 0x00, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xFC, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xF0, 0x01, 0x10, 0x00, 0x10, 0x00, 0xF0, 0x01, 0x10, 0x00, 0x10, 0x00, 0xE0, 0x01, 0x00, 0x00, 
0xF0, 0x01, 0x10, 0x00, 0x10, 0x00, 0xE0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xE0, 0x00, 0x10, 0x01, 0x10, 0x01, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xF0, 0x07, 0x10, 0x01, 0x10, 0x01, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xE0, 0x00, 0x10, 0x01, 0x10, 0x01, 0xF0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xF0, 0x01, 0x10, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x20, 0x01, 0x50, 0x01, 0x50, 0x01, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xF8, 0x01, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xF0, 0x00, 0x00, 0x01, 0x00, 0x01, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x10, 0x00, 0xE0, 0x00, 0x00, 0x01, 0xE0, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xF0, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x10, 0x01, 0xE0, 0x00, 0xE0, 0x00, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x10, 0x00, 0xE0, 0x04, 0x00, 0x03, 0xE0, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x90, 0x01, 0x50, 0x01, 0x30, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x40, 0x00, 0xBC, 0x07, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xFC, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x04, 0x04, 0xBC, 0x07, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x60, 0x00, 0x20, 0x00, 0x40, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xF8, 0x01, 0x08, 0x01, 0x08, 0x01, 0x08, 0x01, 0xF8, 0x01, 0x00, 0x00, 0x00, 0x00, 

};


static const uint8 FONT_ASCII_8X8[]=
{/*
    ASCII Font Data(0x20 -- 0x79)
    Format(Width * Height = 8 * 8), Lognitudinal:  
        8 byte per char. 
        the most left and most top pixel == the bit 0 of the first byte.
    Font Type : Arial 
*/
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x2E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x3C, 0x16, 0x34, 0x1E, 0x00, 0x00, 0x00, 0x00, 
0x30, 0x7E, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x04, 0x0A, 0x34, 0x1C, 0x2A, 0x10, 0x00, 0x00, 
0x30, 0x2E, 0x2A, 0x16, 0x20, 0x00, 0x00, 0x00, 
0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x3C, 0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x62, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x04, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x08, 0x1C, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x38, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x3E, 0x22, 0x22, 0x3E, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x04, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x22, 0x32, 0x2A, 0x26, 0x00, 0x00, 0x00, 0x00, 
0x22, 0x22, 0x2A, 0x3E, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x18, 0x3C, 0x10, 0x00, 0x00, 0x00, 0x00, 
0x2C, 0x2E, 0x2A, 0x1A, 0x00, 0x00, 0x00, 0x00, 
0x1C, 0x2A, 0x2A, 0x3A, 0x00, 0x00, 0x00, 0x00, 
0x02, 0x3A, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x36, 0x2A, 0x2A, 0x36, 0x00, 0x00, 0x00, 0x00, 
0x2E, 0x2A, 0x2A, 0x1C, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x08, 0x14, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x14, 0x14, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x02, 0x02, 0x2A, 0x06, 0x00, 0x00, 0x00, 0x00, 
0x3C, 0x7E, 0x66, 0x7A, 0x66, 0x3C, 0x00, 0x00, 
0x20, 0x1C, 0x0A, 0x1C, 0x20, 0x00, 0x00, 0x00, 
0x00, 0x3E, 0x2A, 0x3E, 0x00, 0x00, 0x00, 0x00, 
0x1C, 0x22, 0x22, 0x14, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x3E, 0x22, 0x1C, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x3E, 0x2A, 0x2A, 0x2A, 0x00, 0x00, 0x00, 
0x00, 0x3E, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x1C, 0x22, 0x22, 0x2A, 0x1C, 0x00, 0x00, 0x00, 
0x00, 0x3E, 0x08, 0x3E, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x30, 0x20, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x3E, 0x08, 0x14, 0x22, 0x00, 0x00, 0x00, 
0x00, 0x3E, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 
0x3E, 0x1E, 0x20, 0x1E, 0x3E, 0x00, 0x00, 0x00, 
0x00, 0x3E, 0x1C, 0x3E, 0x00, 0x00, 0x00, 0x00, 
0x1C, 0x22, 0x22, 0x1C, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x3E, 0x0A, 0x0E, 0x00, 0x00, 0x00, 0x00, 
0x1C, 0x22, 0x32, 0x3C, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x3E, 0x0A, 0x1E, 0x20, 0x00, 0x00, 0x00, 
0x16, 0x2A, 0x2A, 0x2A, 0x14, 0x00, 0x00, 0x00, 
0x02, 0x02, 0x3E, 0x02, 0x02, 0x00, 0x00, 0x00, 
0x00, 0x3E, 0x20, 0x3E, 0x00, 0x00, 0x00, 0x00, 
0x02, 0x1C, 0x20, 0x1C, 0x02, 0x00, 0x00, 0x00, 
0x1E, 0x20, 0x1C, 0x02, 0x1C, 0x20, 0x1E, 0x00, 
0x22, 0x14, 0x08, 0x14, 0x22, 0x00, 0x00, 0x00, 
0x02, 0x04, 0x38, 0x04, 0x02, 0x00, 0x00, 0x00, 
0x32, 0x2A, 0x26, 0x22, 0x00, 0x00, 0x00, 0x00, 
0x7E, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x06, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x42, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x0C, 0x02, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 
0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x34, 0x2C, 0x2C, 0x3C, 0x00, 0x00, 0x00, 0x00, 
0x3E, 0x24, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x18, 0x24, 0x24, 0x24, 0x00, 0x00, 0x00, 0x00, 
0x3C, 0x24, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x3C, 0x2C, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x04, 0x3C, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x7C, 0x64, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x3E, 0x04, 0x04, 0x38, 0x00, 0x00, 0x00, 0x00, 
0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x3E, 0x18, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x3C, 0x04, 0x3C, 0x04, 0x38, 0x00, 0x00, 0x00, 
0x3C, 0x04, 0x04, 0x38, 0x00, 0x00, 0x00, 0x00, 
0x18, 0x24, 0x24, 0x18, 0x00, 0x00, 0x00, 0x00, 
0x7C, 0x24, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x3C, 0x24, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x3C, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x28, 0x2C, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x1C, 0x20, 0x20, 0x3C, 0x00, 0x00, 0x00, 0x00, 
0x3C, 0x20, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x1C, 0x38, 0x1C, 0x20, 0x1C, 0x00, 0x00, 0x00, 
0x24, 0x18, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x4C, 0x70, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x24, 0x3C, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x08, 0x76, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x76, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x08, 0x18, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x3E, 0x22, 0x3E, 0x00, 0x00, 0x00, 0x00, 

};

const uint8 *GetAscii_8x16_bmp(uint32 wch)
{
    uint32   pos;    
        
    //wch -= 0x20;
    pos = (wch < 0x80) ?  (wch << 4)  : 0;
    
    return &FONT_ASCII_8X16[ pos ];
}

//
// Get ASCII Font Data(Width * Height = 8 * 16)
//
void GetAscii16_Data(uint8 ch, uint8 *buf)
{
    uint32 	pos;
    uint8   *data_ptr;

    if( ch & 0x80 )
    {
        ch = ' ';
    }

    pos      = (uint32)ch; // - 0x20;
    pos    <<= 4; //pos *= 16;
    data_ptr = (uint8 *)&FONT_ASCII_8X16[pos];
    
    for( pos = 0; pos < 16; pos++ )
    {
        *buf++ = *data_ptr++;
    }
}


//
// Get ASCII Font Data(Width * Height = 8 * 12)
//
void GetAscii12_Data(uint8 ch, uint8 *buf)
{
    uint32 	pos;
    uint8   *data_ptr;

    if( (ch < 0x20) || (ch & 0x80) )
    {
        ch = ' ';
    }

    pos      = (uint32)ch - 0x20;
    pos    <<= 4; //pos *= 16;
    data_ptr = (uint8 *)&FONT_ASCII_8X12[pos];
    
    for( pos = 0; pos < 16; pos++ )
    {
        *buf++ = *data_ptr++;
    }
}


const uint8 *GetAscii_8x8_bmp(uint32 wch)
{
    uint32  pos;
    
    wch -= 0x20;
    pos  = ( wch < (0x80 - 0x20)) ?  wch << 3 : 0;
    return &FONT_ASCII_8X8[pos];
}

//
// Get ASCII Font Data(Width * Height = 8 * 8)
//
void GetAscii8_Data(uint8 ch, uint8 *buf)
{
    uint32 	pos;
    uint8  *data_ptr;

    if( (ch < 0x20) || (ch & 0x80) )
    {
        ch = ' ';
    }

    pos      = (uint32)ch - 0x20;
    pos    <<= 3;  //pos *= 8;
    data_ptr = (uint8 *)&FONT_ASCII_8X8[pos];
    
    for( pos = 0; pos < 8; pos++ )
    {
        *buf++ = *data_ptr++;
    }
}

//
//  Get ASCII Font Data(Width * Height = 5 * 8)
//
void GetAscii5X8_Data(uint8 ch, uint8 *buf)
{
    uint32  pos;
    uint8  *data_ptr;
    
    if( (ch < 0x20) || (ch & 0x80) )
    {
        ch = ' ';
    }
    
    pos = (uint32)ch - 0x20;
    pos *= 5;
    
    data_ptr = (uint8 *)&FONT_ASCII_5X8[pos];
    for( pos = 0; pos < 5; pos++ )
    {
        *buf++ = *data_ptr++;
    }  
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
