/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
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


#ifndef _RF_REGISTERS_H_
#define _RF_REGISTERS_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'rf_registers'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// RF_REGISTERS_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_RF_REG_BASE            0x05030000
#else
#define REG_RF_REG_BASE            0x50030000
#endif

typedef volatile struct
{
    REG32 Reserved_00000000[128];               //0x00000000
    REG32                          int_clear0;                   //0x00000200
    REG32                          int_clear1;                   //0x00000204
    REG32                          int2tmcu0;                    //0x00000208
    REG32                          int2tmcu1;                    //0x0000020C
    REG32 Reserved_00000210[26];                //0x00000210
    REG32                          afc_freq_rxpll1;              //0x00000278
    REG32                          afc_freq_trxpll2;             //0x0000027C
    REG32                          afc_freq_txpll;               //0x00000280
    REG32                          afc_freq_bbpll1;              //0x00000284
    REG32                          afc_freq_bbpll12;             //0x00000288
    REG32                          afc_freq_bbpll2;              //0x0000028C
    REG32                          afc_freq_offset_mode;         //0x00000290
    REG32 Reserved_00000294[11];                //0x00000294
    REG32                          rxsdm_reg1;                   //0x000002C0
    REG32                          rxsdm_reg2;                   //0x000002C4
    REG32                          rxsdm_reg3;                   //0x000002C8
    REG32                          rxsdm_reg4;                   //0x000002CC
    REG32                          rxsdm_reg5;                   //0x000002D0
    REG32 Reserved_000002D4;                    //0x000002D4
    REG32                          freq_offset_ini_rx_reg1;      //0x000002D8
    REG32                          freq_offset_ini_rx_reg2;      //0x000002DC
    REG32                          txsdm_reg1;                   //0x000002E0
    REG32                          txsdm_reg4;                   //0x000002E4
    REG32                          txsdm_reg5;                   //0x000002E8
    REG32 Reserved_000002EC;                    //0x000002EC
    REG32                          freq_offset_ini_tx_reg1;      //0x000002F0
    REG32                          freq_offset_ini_bbpll1_reg1;  //0x000002F4
    REG32                          freq_offset_ini_bbpll1_reg2;  //0x000002F8
    REG32                          freq_offset_ini_bbpll2_reg1;  //0x000002FC
    REG32                          bbpll1_reg1;                  //0x00000300
    REG32                          bbpll1_reg2;                  //0x00000304
    REG32                          bbpll1_reg3;                  //0x00000308
    REG32                          bbpll1_reg5;                  //0x0000030C
    REG32                          bbpll1_reg6;                  //0x00000310
    REG32                          bbpll1_reg7;                  //0x00000314
    REG32                          bbpll1_reg8;                  //0x00000318
    REG32                          bbpll1_reg9;                  //0x0000031C
    REG32                          bbpll1_rega;                  //0x00000320
    REG32                          bbpll1_regb;                  //0x00000324
    REG32                          bbpll1_regd;                  //0x00000328
    REG32                          bbpll1_regf;                  //0x0000032C
    REG32                          bbpll2_reg1;                  //0x00000330
    REG32                          bbpll2_reg2;                  //0x00000334
    REG32                          bbpll2_reg3;                  //0x00000338
    REG32                          bbpll2_reg5;                  //0x0000033C
    REG32                          bbpll2_reg6;                  //0x00000340
    REG32                          bbpll2_reg7;                  //0x00000344
    REG32                          bbpll2_reg8;                  //0x00000348
    REG32                          bbpll2_reg9;                  //0x0000034C
    REG32                          bbpll2_rega;                  //0x00000350
    REG32                          bbpll2_regb;                  //0x00000354
    REG32                          bbpll2_regd;                  //0x00000358
    REG32                          bbpll2_regf;                  //0x0000035C
    REG32                          rxpll_cal_reg1;               //0x00000360
    REG32                          rxpll_cal_reg2;               //0x00000364
    REG32                          rxpll_cal_reg3;               //0x00000368
    REG32                          rxpll_cal_reg4;               //0x0000036C
    REG32                          txpll_cal_reg1;               //0x00000370
    REG32                          txpll_cal_reg2;               //0x00000374
    REG32                          txpll_cal_reg3;               //0x00000378
    REG32                          txpll_cal_reg4;               //0x0000037C
    REG32                          gpio_reg;                     //0x00000380
    REG32 Reserved_00000384[15];                //0x00000384
    REG32                          strobe_reg;                   //0x000003C0
    REG32                          pa_ctrl_reg1;                 //0x000003C4
    REG32                          pa_ctrl_reg2;                 //0x000003C8
    REG32                          pa_ctrl_reg3;                 //0x000003CC
    REG32                          pa_ctrl_reg4;                 //0x000003D0
    REG32 Reserved_000003D4[11];                //0x000003D4
    REG32                          ana_ctrl_reg0;                //0x00000400
    REG32                          ana_ctrl_reg1;                //0x00000404
    REG32                          ana_ctrl_reg2;                //0x00000408
    REG32                          ana_ctrl_reg3;                //0x0000040C
    REG32                          ana_ctrl_reg4;                //0x00000410
    REG32                          ana_ctrl_reg5;                //0x00000414
    REG32                          ana_ctrl_reg6;                //0x00000418
    REG32                          ana_ctrl_reg7;                //0x0000041C
    REG32                          ana_ctrl_reg8;                //0x00000420
    REG32                          ana_ctrl_reg9;                //0x00000424
    REG32                          ana_ctrl_rega;                //0x00000428
    REG32                          ana_ctrl_regb;                //0x0000042C
    REG32                          ana_ctrl_regc;                //0x00000430
    REG32                          ana_ctrl_regd;                //0x00000434
    REG32                          ana_ctrl_rege;                //0x00000438
    REG32                          ana_ctrl_regf;                //0x0000043C
    REG32                          ana_ctrl_reg10;               //0x00000440
    REG32                          ana_ctrl_reg11;               //0x00000444
    REG32                          ana_ctrl_reg12;               //0x00000448
    REG32                          ana_ctrl_reg13;               //0x0000044C
    REG32                          ana_ctrl_reg14;               //0x00000450
    REG32                          ana_ctrl_reg15;               //0x00000454
    REG32                          ana_ctrl_reg16;               //0x00000458
    REG32                          ana_ctrl_reg17;               //0x0000045C
    REG32                          ana_ctrl_reg18;               //0x00000460
    REG32                          ana_ctrl_reg19;               //0x00000464
    REG32                          ana_ctrl_reg1a;               //0x00000468
    REG32                          ana_ctrl_reg1b;               //0x0000046C
    REG32                          ana_ctrl_reg1c;               //0x00000470
    REG32                          ana_ctrl_reg1d;               //0x00000474
    REG32                          ana_ctrl_reg1e;               //0x00000478
    REG32                          ana_ctrl_reg1f;               //0x0000047C
    REG32                          ana_ctrl_reg20;               //0x00000480
    REG32                          ana_ctrl_reg21;               //0x00000484
    REG32                          ana_ctrl_reg22;               //0x00000488
    REG32                          ana_ctrl_reg23;               //0x0000048C
    REG32                          ana_ctrl_reg24;               //0x00000490
    REG32                          ana_ctrl_reg25;               //0x00000494
    REG32                          ana_ctrl_reg26;               //0x00000498
    REG32                          ana_ctrl_reg27;               //0x0000049C
    REG32                          ana_ctrl_reg28;               //0x000004A0
    REG32                          ana_ctrl_reg29;               //0x000004A4
    REG32                          ana_ctrl_reg2a;               //0x000004A8
    REG32                          ana_ctrl_reg2b;               //0x000004AC
    REG32                          ana_ctrl_reg2c;               //0x000004B0
    REG32                          ana_ctrl_reg2d;               //0x000004B4
    REG32                          ana_ctrl_reg2e;               //0x000004B8
    REG32                          ana_ctrl_reg2f;               //0x000004BC
    REG32                          ana_ctrl_reg30;               //0x000004C0
    REG32                          ana_ctrl_reg31;               //0x000004C4
    REG32                          ana_ctrl_reg32;               //0x000004C8
    REG32                          ana_ctrl_reg33;               //0x000004CC
    REG32                          ana_ctrl_reg34;               //0x000004D0
    REG32                          ana_ctrl_reg35;               //0x000004D4
    REG32                          ana_ctrl_reg36;               //0x000004D8
    REG32                          ana_ctrl_reg37;               //0x000004DC
    REG32 Reserved_000004E0[72];                //0x000004E0
    REG32                          control_rf_reg0;              //0x00000600
    REG32                          control_rf_reg1;              //0x00000604
    REG32                          control_rf_reg2;              //0x00000608
    REG32                          control_rf_reg3;              //0x0000060C
    REG32                          control_rf_reg6;              //0x00000610
    REG32                          clk_gen_reg0;                 //0x00000614
    REG32                          clk_gen_reg1;                 //0x00000618
    REG32                          clk_gen_reg2;                 //0x0000061C
    REG32 Reserved_00000620[120];               //0x00000620
    REG32                          chip_id_reg0;                 //0x00000800
    REG32                          chip_id_reg1;                 //0x00000804
    REG32                          chip_id_reg2;                 //0x00000808
    REG32                          mean_dcccal_i_reg0;           //0x0000080C
    REG32                          mean_dcccal_i_reg1;           //0x00000810
    REG32                          mean_dcccal_q_reg;            //0x00000814
    REG32                          revid_reg;                    //0x00000818
    REG32 Reserved_0000081C[25];                //0x0000081C
    REG32                          ana_rd_reg0;                  //0x00000880
    REG32                          ana_rd_reg1;                  //0x00000884
    REG32                          ana_rd_reg2;                  //0x00000888
    REG32                          ana_rd_reg3;                  //0x0000088C
    REG32 Reserved_00000890[220];               //0x00000890
    REG32                          pa_ramp_reg0;                 //0x00000C00
    REG32                          pa_ramp_reg1;                 //0x00000C04
    REG32                          pa_ramp_reg2;                 //0x00000C08
    REG32                          pa_ramp_reg3;                 //0x00000C0C
    REG32                          pa_ramp_reg4;                 //0x00000C10
    REG32                          pa_ramp_reg5;                 //0x00000C14
    REG32                          pa_ramp_reg6;                 //0x00000C18
    REG32                          pa_ramp_reg7;                 //0x00000C1C
    REG32                          pa_ramp_reg10;                //0x00000C20
    REG32                          pa_ramp_reg11;                //0x00000C24
    REG32                          pa_ramp_reg12;                //0x00000C28
    REG32                          pa_ramp_reg13;                //0x00000C2C
    REG32                          pa_ramp_reg14;                //0x00000C30
    REG32                          pa_ramp_reg15;                //0x00000C34
    REG32                          pa_ramp_reg16;                //0x00000C38
    REG32                          pa_ramp_reg17;                //0x00000C3C
    REG32                          pa_ramp_reg20;                //0x00000C40
    REG32                          pa_ramp_reg21;                //0x00000C44
    REG32                          pa_ramp_reg22;                //0x00000C48
    REG32                          pa_ramp_reg23;                //0x00000C4C
    REG32                          pa_ramp_reg24;                //0x00000C50
    REG32                          pa_ramp_reg25;                //0x00000C54
    REG32                          pa_ramp_reg26;                //0x00000C58
    REG32                          pa_ramp_reg27;                //0x00000C5C
    REG32                          pa_ramp_reg30;                //0x00000C60
    REG32                          pa_ramp_reg31;                //0x00000C64
    REG32                          pa_ramp_reg32;                //0x00000C68
    REG32                          pa_ramp_reg33;                //0x00000C6C
    REG32                          pa_ramp_reg34;                //0x00000C70
    REG32                          pa_ramp_reg35;                //0x00000C74
    REG32                          pa_ramp_reg36;                //0x00000C78
    REG32                          pa_ramp_reg37;                //0x00000C7C
    REG32                          pa_ramp_reg40;                //0x00000C80
    REG32                          pa_ramp_reg41;                //0x00000C84
    REG32                          pa_ramp_reg42;                //0x00000C88
    REG32                          pa_ramp_reg43;                //0x00000C8C
    REG32                          pa_ramp_reg44;                //0x00000C90
    REG32                          pa_ramp_reg45;                //0x00000C94
    REG32                          pa_ramp_reg46;                //0x00000C98
    REG32                          pa_ramp_reg47;                //0x00000C9C
    REG32                          pa_ramp_reg50;                //0x00000CA0
    REG32                          pa_ramp_reg51;                //0x00000CA4
    REG32                          pa_ramp_reg52;                //0x00000CA8
    REG32                          pa_ramp_reg53;                //0x00000CAC
    REG32                          pa_ramp_reg54;                //0x00000CB0
    REG32                          pa_ramp_reg55;                //0x00000CB4
    REG32                          pa_ramp_reg56;                //0x00000CB8
    REG32                          pa_ramp_reg57;                //0x00000CBC
    REG32                          pa_ramp_reg60;                //0x00000CC0
    REG32                          pa_ramp_reg61;                //0x00000CC4
    REG32                          pa_ramp_reg62;                //0x00000CC8
    REG32                          pa_ramp_reg63;                //0x00000CCC
    REG32                          pa_ramp_reg64;                //0x00000CD0
    REG32                          pa_ramp_reg65;                //0x00000CD4
    REG32                          pa_ramp_reg66;                //0x00000CD8
    REG32                          pa_ramp_reg67;                //0x00000CDC
    REG32                          pa_ramp_reg70;                //0x00000CE0
    REG32                          pa_ramp_reg71;                //0x00000CE4
    REG32                          pa_ramp_reg72;                //0x00000CE8
    REG32                          pa_ramp_reg73;                //0x00000CEC
    REG32                          pa_ramp_reg74;                //0x00000CF0
    REG32                          pa_ramp_reg75;                //0x00000CF4
    REG32                          pa_ramp_reg76;                //0x00000CF8
    REG32                          pa_ramp_reg77;                //0x00000CFC
    REG32                          pa_ramp_reg80;                //0x00000D00
    REG32                          pa_ramp_reg81;                //0x00000D04
    REG32                          pa_ramp_reg82;                //0x00000D08
    REG32                          pa_ramp_reg83;                //0x00000D0C
    REG32                          pa_ramp_reg84;                //0x00000D10
    REG32                          pa_ramp_reg85;                //0x00000D14
    REG32                          pa_ramp_reg86;                //0x00000D18
    REG32                          pa_ramp_reg87;                //0x00000D1C
    REG32                          pa_ramp_reg90;                //0x00000D20
    REG32                          pa_ramp_reg91;                //0x00000D24
    REG32                          pa_ramp_reg92;                //0x00000D28
    REG32                          pa_ramp_reg93;                //0x00000D2C
    REG32                          pa_ramp_reg94;                //0x00000D30
    REG32                          pa_ramp_reg95;                //0x00000D34
    REG32                          pa_ramp_reg96;                //0x00000D38
    REG32                          pa_ramp_reg97;                //0x00000D3C
    REG32                          pa_ramp_rega0;                //0x00000D40
    REG32                          pa_ramp_rega1;                //0x00000D44
    REG32                          pa_ramp_rega2;                //0x00000D48
    REG32                          pa_ramp_rega3;                //0x00000D4C
    REG32                          pa_ramp_rega4;                //0x00000D50
    REG32                          pa_ramp_rega5;                //0x00000D54
    REG32                          pa_ramp_rega6;                //0x00000D58
    REG32                          pa_ramp_rega7;                //0x00000D5C
    REG32                          pa_ramp_regb0;                //0x00000D60
    REG32                          pa_ramp_regb1;                //0x00000D64
    REG32                          pa_ramp_regb2;                //0x00000D68
    REG32                          pa_ramp_regb3;                //0x00000D6C
    REG32                          pa_ramp_regb4;                //0x00000D70
    REG32                          pa_ramp_regb5;                //0x00000D74
    REG32                          pa_ramp_regb6;                //0x00000D78
    REG32                          pa_ramp_regb7;                //0x00000D7C
    REG32                          pa_ramp_regc0;                //0x00000D80
    REG32                          pa_ramp_regc1;                //0x00000D84
    REG32                          pa_ramp_regc2;                //0x00000D88
    REG32                          pa_ramp_regc3;                //0x00000D8C
    REG32                          pa_ramp_regc4;                //0x00000D90
    REG32                          pa_ramp_regc5;                //0x00000D94
    REG32                          pa_ramp_regc6;                //0x00000D98
    REG32                          pa_ramp_regc7;                //0x00000D9C
    REG32                          pa_ramp_regd0;                //0x00000DA0
    REG32                          pa_ramp_regd1;                //0x00000DA4
    REG32                          pa_ramp_regd2;                //0x00000DA8
    REG32                          pa_ramp_regd3;                //0x00000DAC
    REG32                          pa_ramp_regd4;                //0x00000DB0
    REG32                          pa_ramp_regd5;                //0x00000DB4
    REG32                          pa_ramp_regd6;                //0x00000DB8
    REG32                          pa_ramp_regd7;                //0x00000DBC
    REG32                          pa_ramp_rege0;                //0x00000DC0
    REG32                          pa_ramp_rege1;                //0x00000DC4
    REG32                          pa_ramp_rege2;                //0x00000DC8
    REG32                          pa_ramp_rege3;                //0x00000DCC
    REG32                          pa_ramp_rege4;                //0x00000DD0
    REG32                          pa_ramp_rege5;                //0x00000DD4
    REG32                          pa_ramp_rege6;                //0x00000DD8
    REG32                          pa_ramp_rege7;                //0x00000DDC
    REG32                          pa_ramp_regf0;                //0x00000DE0
    REG32                          pa_ramp_regf1;                //0x00000DE4
    REG32                          pa_ramp_regf2;                //0x00000DE8
    REG32                          pa_ramp_regf3;                //0x00000DEC
    REG32                          pa_ramp_regf4;                //0x00000DF0
    REG32                          pa_ramp_regf5;                //0x00000DF4
    REG32                          pa_ramp_regf6;                //0x00000DF8
    REG32                          pa_ramp_regf7;                //0x00000DFC
    REG32                          pa_ramp_reg100;               //0x00000E00
    REG32                          pa_ramp_reg101;               //0x00000E04
    REG32                          pa_ramp_reg102;               //0x00000E08
    REG32                          pa_ramp_reg103;               //0x00000E0C
    REG32                          pa_ramp_reg104;               //0x00000E10
    REG32                          pa_ramp_reg105;               //0x00000E14
    REG32                          pa_ramp_reg106;               //0x00000E18
    REG32                          pa_ramp_reg107;               //0x00000E1C
    REG32                          pa_ramp_reg110;               //0x00000E20
    REG32                          pa_ramp_reg111;               //0x00000E24
    REG32                          pa_ramp_reg112;               //0x00000E28
    REG32                          pa_ramp_reg113;               //0x00000E2C
    REG32                          pa_ramp_reg114;               //0x00000E30
    REG32                          pa_ramp_reg115;               //0x00000E34
    REG32                          pa_ramp_reg116;               //0x00000E38
    REG32                          pa_ramp_reg117;               //0x00000E3C
    REG32                          pa_ramp_reg120;               //0x00000E40
    REG32                          pa_ramp_reg121;               //0x00000E44
    REG32                          pa_ramp_reg122;               //0x00000E48
    REG32                          pa_ramp_reg123;               //0x00000E4C
    REG32                          pa_ramp_reg124;               //0x00000E50
    REG32                          pa_ramp_reg125;               //0x00000E54
    REG32                          pa_ramp_reg126;               //0x00000E58
    REG32                          pa_ramp_reg127;               //0x00000E5C
    REG32                          pa_ramp_reg130;               //0x00000E60
    REG32                          pa_ramp_reg131;               //0x00000E64
    REG32                          pa_ramp_reg132;               //0x00000E68
    REG32                          pa_ramp_reg133;               //0x00000E6C
    REG32                          pa_ramp_reg134;               //0x00000E70
    REG32                          pa_ramp_reg135;               //0x00000E74
    REG32                          pa_ramp_reg136;               //0x00000E78
    REG32                          pa_ramp_reg137;               //0x00000E7C
    REG32                          pa_ramp_reg140;               //0x00000E80
    REG32                          pa_ramp_reg141;               //0x00000E84
    REG32                          pa_ramp_reg142;               //0x00000E88
    REG32                          pa_ramp_reg143;               //0x00000E8C
    REG32                          pa_ramp_reg144;               //0x00000E90
    REG32                          pa_ramp_reg145;               //0x00000E94
    REG32                          pa_ramp_reg146;               //0x00000E98
    REG32                          pa_ramp_reg147;               //0x00000E9C
    REG32                          pa_ramp_reg150;               //0x00000EA0
    REG32                          pa_ramp_reg151;               //0x00000EA4
    REG32                          pa_ramp_reg152;               //0x00000EA8
    REG32                          pa_ramp_reg153;               //0x00000EAC
    REG32                          pa_ramp_reg154;               //0x00000EB0
    REG32                          pa_ramp_reg155;               //0x00000EB4
    REG32                          pa_ramp_reg156;               //0x00000EB8
    REG32                          pa_ramp_reg157;               //0x00000EBC
    REG32                          pa_ramp_reg160;               //0x00000EC0
    REG32                          pa_ramp_reg161;               //0x00000EC4
    REG32                          pa_ramp_reg162;               //0x00000EC8
    REG32                          pa_ramp_reg163;               //0x00000ECC
    REG32                          pa_ramp_reg164;               //0x00000ED0
    REG32                          pa_ramp_reg165;               //0x00000ED4
    REG32                          pa_ramp_reg166;               //0x00000ED8
    REG32                          pa_ramp_reg167;               //0x00000EDC
    REG32                          pa_ramp_reg170;               //0x00000EE0
    REG32                          pa_ramp_reg171;               //0x00000EE4
    REG32                          pa_ramp_reg172;               //0x00000EE8
    REG32                          pa_ramp_reg173;               //0x00000EEC
    REG32                          pa_ramp_reg174;               //0x00000EF0
    REG32                          pa_ramp_reg175;               //0x00000EF4
    REG32                          pa_ramp_reg176;               //0x00000EF8
    REG32                          pa_ramp_reg177;               //0x00000EFC
    REG32                          pa_on_h_reg;                  //0x00000F00
    REG32 Reserved_00000F04[63];                //0x00000F04
    REG32                          sys_ctrl_reg_20;              //0x00001000
    REG32                          sys_ctrl_reg_22;              //0x00001004
    REG32                          sys_ctrl_reg_24;              //0x00001008
    REG32                          sys_ctrl_reg_26;              //0x0000100C
    REG32                          sys_ctrl_reg_28;              //0x00001010
    REG32                          sys_ctrl_reg_2a;              //0x00001014
    REG32                          sys_ctrl_reg_2c;              //0x00001018
    REG32                          sys_ctrl_reg_2e;              //0x0000101C
    REG32                          sys_ctrl_reg_210;             //0x00001020
    REG32                          sys_ctrl_reg_212;             //0x00001024
    REG32                          sys_ctrl_reg_214;             //0x00001028
    REG32                          sys_ctrl_reg_216;             //0x0000102C
    REG32                          sys_ctrl_reg_218;             //0x00001030
    REG32                          sys_ctrl_reg_21a;             //0x00001034
    REG32                          sys_ctrl_reg_21c;             //0x00001038
    REG32                          sys_ctrl_reg_21e;             //0x0000103C
    REG32                          dlpf_ctrl_reg;                //0x00001040
} HWP_RF_REGISTERS_T;

#define hwp_rfReg                  ((HWP_RF_REGISTERS_T*) REG_ACCESS_ADDRESS(REG_RF_REG_BASE))


//int_clear0
typedef union {
    REG32 v;
    struct {
        REG32 int_ctrl_bit_0 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_INT_CLEAR0_T;

//int_clear1
typedef union {
    REG32 v;
    struct {
        REG32 int_ctrl_bit_1 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_INT_CLEAR1_T;

//int2tmcu0
typedef union {
    REG32 v;
    struct {
        REG32 irq0 : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_INT2TMCU0_T;

//int2tmcu1
typedef union {
    REG32 v;
    struct {
        REG32 irq1 : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_INT2TMCU1_T;

//afc_freq_rxpll1
typedef union {
    REG32 v;
    struct {
        REG32 freq_offset_rxpll1 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_AFC_FREQ_RXPLL1_T;

//afc_freq_trxpll2
typedef union {
    REG32 v;
    struct {
        REG32 freq_offset_txpll2 : 8; // [7:0]
        REG32 freq_offset_rxpll2 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_AFC_FREQ_TRXPLL2_T;

//afc_freq_txpll
typedef union {
    REG32 v;
    struct {
        REG32 freq_offset_txpll1 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_AFC_FREQ_TXPLL_T;

//afc_freq_bbpll1
typedef union {
    REG32 v;
    struct {
        REG32 freq_offset_bbpll11 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_AFC_FREQ_BBPLL1_T;

//afc_freq_bbpll12
typedef union {
    REG32 v;
    struct {
        REG32 freq_offset_bbpll22 : 8; // [7:0]
        REG32 freq_offset_bbpll12 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_AFC_FREQ_BBPLL12_T;

//afc_freq_bbpll2
typedef union {
    REG32 v;
    struct {
        REG32 freq_offset_bbpll21 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_AFC_FREQ_BBPLL2_T;

//afc_freq_offset_mode
typedef union {
    REG32 v;
    struct {
        REG32 freq_offset_mode_rxpll : 1; // [0]
        REG32 freq_offset_mode_txpll : 1; // [1]
        REG32 freq_offset_mode_bbpll1 : 1; // [2]
        REG32 freq_offset_mode_bbpll2 : 1; // [3]
        REG32 __31_4 : 28;
    } b;
} REG_RF_REGISTERS_AFC_FREQ_OFFSET_MODE_T;

//rxsdm_reg1
typedef union {
    REG32 v;
    struct {
        REG32 dll_mode_rxsdm : 2; // [1:0]
        REG32 clk_former_edge_rxsdm : 1; // [2]
        REG32 dither_bypass_rxsdm : 1; // [3]
        REG32 __31_4 : 28;
    } b;
} REG_RF_REGISTERS_RXSDM_REG1_T;

//rxsdm_reg2
typedef union {
    REG32 v;
    struct {
        REG32 freq_rxsdm0 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_RXSDM_REG2_T;

//rxsdm_reg3
typedef union {
    REG32 v;
    struct {
        REG32 freq_rxsdm1 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_RXSDM_REG3_T;

//rxsdm_reg4
typedef union {
    REG32 v;
    struct {
        REG32 freq_former_bypass_rxsdm : 1; // [0]
        REG32 freq_former_shift_ct_rxsdm : 3; // [3:1]
        REG32 int_dec_sel_rxsdm : 2; // [5:4]
        REG32 resetn_rxsdm : 1; // [6]
        REG32 tx_rx_rxsdm : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_RF_REGISTERS_RXSDM_REG4_T;

//rxsdm_reg5
typedef union {
    REG32 v;
    struct {
        REG32 reser_sdm_rxsdm : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_RF_REGISTERS_RXSDM_REG5_T;

//freq_offset_ini_rx_reg1
typedef union {
    REG32 v;
    struct {
        REG32 freq_offset_ini_rxpll1 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_FREQ_OFFSET_INI_RX_REG1_T;

//freq_offset_ini_rx_reg2
typedef union {
    REG32 v;
    struct {
        REG32 freq_offset_ini_rxpll2 : 8; // [7:0]
        REG32 freq_offset_ini_txpll2 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_FREQ_OFFSET_INI_RX_REG2_T;

//txsdm_reg1
typedef union {
    REG32 v;
    struct {
        REG32 dll_mode_txsdm : 2; // [1:0]
        REG32 clk_former_edge_txsdm : 1; // [2]
        REG32 dither_bypass_txsdm : 1; // [3]
        REG32 dll_update_txsdm : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_RF_REGISTERS_TXSDM_REG1_T;

//txsdm_reg4
typedef union {
    REG32 v;
    struct {
        REG32 freq_former_bypass_txsdm : 1; // [0]
        REG32 freq_former_shift_ct_txsdm : 3; // [3:1]
        REG32 int_dec_sel_txsdm : 2; // [5:4]
        REG32 resetn_txsdm : 1; // [6]
        REG32 tx_rx_txsdm : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_RF_REGISTERS_TXSDM_REG4_T;

//txsdm_reg5
typedef union {
    REG32 v;
    struct {
        REG32 reser_sdm_txsdm : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_RF_REGISTERS_TXSDM_REG5_T;

//freq_offset_ini_tx_reg1
typedef union {
    REG32 v;
    struct {
        REG32 freq_offset_ini_txpll1 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_FREQ_OFFSET_INI_TX_REG1_T;

//freq_offset_ini_bbpll1_reg1
typedef union {
    REG32 v;
    struct {
        REG32 freq_offset_ini_bbpll11 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_FREQ_OFFSET_INI_BBPLL1_REG1_T;

//freq_offset_ini_bbpll1_reg2
typedef union {
    REG32 v;
    struct {
        REG32 freq_offset_ini_bbpll12 : 8; // [7:0]
        REG32 freq_offset_ini_bbpll22 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_FREQ_OFFSET_INI_BBPLL1_REG2_T;

//freq_offset_ini_bbpll2_reg1
typedef union {
    REG32 v;
    struct {
        REG32 freq_offset_ini_bbpll21 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_FREQ_OFFSET_INI_BBPLL2_REG1_T;

//bbpll1_reg1
typedef union {
    REG32 v;
    struct {
        REG32 pll_vreg_bit_rx : 4; // [3:0]
        REG32 pll_cp_bit_rx : 3; // [6:4]
        REG32 pll_filter_ibit_rx : 3; // [9:7]
        REG32 pll_cpaux_bit_rx : 3; // [12:10]
        REG32 __31_13 : 19;
    } b;
} REG_RF_REGISTERS_BBPLL1_REG1_T;

//bbpll1_reg2
typedef union {
    REG32 v;
    struct {
        REG32 pu_pll_reg_rx : 1; // [0]
        REG32 pu_pll_dr_rx : 1; // [1]
        REG32 pll_sdm_clk_sel_nor_rx : 1; // [2]
        REG32 pll_sdm_clk_sel_rst_rx : 1; // [3]
        REG32 pll_sdm_clk_test_en_rx : 1; // [4]
        REG32 pll_test_en_rx : 1; // [5]
        REG32 pll_low_test_rx : 1; // [6]
        REG32 pll_high_test_rx : 1; // [7]
        REG32 pll_refmulti2_en_rx : 1; // [8]
        REG32 pll_reser_rx : 4; // [12:9]
        REG32 __31_13 : 19;
    } b;
} REG_RF_REGISTERS_BBPLL1_REG2_T;

//bbpll1_reg3
typedef union {
    REG32 v;
    struct {
        REG32 pll_reser_dig_1_rx : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_BBPLL1_REG3_T;

//bbpll1_reg5
typedef union {
    REG32 v;
    struct {
        REG32 pll_sdm_freq_rx1 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_BBPLL1_REG5_T;

//bbpll1_reg6
typedef union {
    REG32 v;
    struct {
        REG32 pll_sdm_freq_rx0 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_BBPLL1_REG6_T;

//bbpll1_reg7
typedef union {
    REG32 v;
    struct {
        REG32 pll_sdm_resetn_reg_rx : 1; // [0]
        REG32 pll_sdm_resetn_dr_rx : 1; // [1]
        REG32 ss_squre_tri_sel_rx : 1; // [2]
        REG32 ss_en_rx : 1; // [3]
        REG32 dither_bypass_rx : 1; // [4]
        REG32 int_dec_sel_rx : 3; // [7:5]
        REG32 reser_sdm_rx : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_BBPLL1_REG7_T;

//bbpll1_reg8
typedef union {
    REG32 v;
    struct {
        REG32 pll_ss_peri_ct_rx : 8; // [7:0]
        REG32 pll_ss_devi_ct_rx : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_BBPLL1_REG8_T;

//bbpll1_reg9
typedef union {
    REG32 v;
    struct {
        REG32 pll_reser_dig_2_rx : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_BBPLL1_REG9_T;

//bbpll1_rega
typedef union {
    REG32 v;
    struct {
        REG32 clk_gen_en_reg_rx : 1; // [0]
        REG32 pll_clkout_en_reg_rx : 4; // [4:1]
        REG32 pll_clk_adc_dfe_en_reg_rx : 1; // [5]
        REG32 pll_clk_adc_en_reg_rx : 1; // [6]
        REG32 pll_clk_adc_sel_reg_rx : 2; // [8:7]
        REG32 __11_9 : 3;
        REG32 sdmclk_sel_time_sel_rx : 2; // [13:12]
        REG32 sdm_reset_time_sel_rx : 2; // [15:14]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_BBPLL1_REGA_T;

//bbpll1_regb
typedef union {
    REG32 v;
    struct {
        REG32 __9_0 : 10;
        REG32 pll_lock_steady_rx : 1; // [10], read only
        REG32 pll_clk_ready_rx : 1; // [11], read only
        REG32 pll_sdm_clk_sel_rx : 1; // [12], read only
        REG32 pll_sdm_resetn_rx : 1; // [13], read only
        REG32 pll_lock_rx : 1; // [14], read only
        REG32 pu_pll_rx : 1; // [15], read only
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_BBPLL1_REGB_T;

//bbpll1_regd
typedef union {
    REG32 v;
    struct {
        REG32 lock_counter_sel_rx : 2; // [1:0]
        REG32 pll_clkout_en_counter_sel_rx : 2; // [3:2]
        REG32 vco_reset_dis_rx : 1; // [4]
        REG32 resetn_spll_rx : 1; // [5]
        REG32 __31_6 : 26;
    } b;
} REG_RF_REGISTERS_BBPLL1_REGD_T;

//bbpll1_regf
typedef union {
    REG32 v;
    struct {
        REG32 pll_clk_ad_sel0_rx : 1; // [0]
        REG32 pll_clk_ad_sel1_rx : 1; // [1]
        REG32 pll_mod23_bb_rx : 1; // [2]
        REG32 pll_mod23_bb2_rx : 1; // [3]
        REG32 pll_cnt1_rx : 5; // [8:4]
        REG32 pll_cnt2_rx : 5; // [13:9]
        REG32 __31_14 : 18;
    } b;
} REG_RF_REGISTERS_BBPLL1_REGF_T;

//bbpll2_reg1
typedef union {
    REG32 v;
    struct {
        REG32 pll_vreg_bit_tx : 4; // [3:0]
        REG32 pll_cp_bit_tx : 3; // [6:4]
        REG32 pll_filter_ibit_tx : 3; // [9:7]
        REG32 pll_cpaux_bit_tx : 3; // [12:10]
        REG32 __31_13 : 19;
    } b;
} REG_RF_REGISTERS_BBPLL2_REG1_T;

//bbpll2_reg2
typedef union {
    REG32 v;
    struct {
        REG32 pu_pll_reg_tx : 1; // [0]
        REG32 pu_pll_dr_tx : 1; // [1]
        REG32 pll_sdm_clk_sel_nor_tx : 1; // [2]
        REG32 pll_sdm_clk_sel_rst_tx : 1; // [3]
        REG32 pll_sdm_clk_test_en_tx : 1; // [4]
        REG32 pll_test_en_tx : 1; // [5]
        REG32 pll_low_test_tx : 1; // [6]
        REG32 pll_high_test_tx : 1; // [7]
        REG32 pll_refmulti2_en_tx : 1; // [8]
        REG32 pll_reser_tx : 4; // [12:9]
        REG32 __31_13 : 19;
    } b;
} REG_RF_REGISTERS_BBPLL2_REG2_T;

//bbpll2_reg3
typedef union {
    REG32 v;
    struct {
        REG32 pll_reser_dig_1_tx : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_BBPLL2_REG3_T;

//bbpll2_reg5
typedef union {
    REG32 v;
    struct {
        REG32 pll_sdm_freq_tx1 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_BBPLL2_REG5_T;

//bbpll2_reg6
typedef union {
    REG32 v;
    struct {
        REG32 pll_sdm_freq_tx0 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_BBPLL2_REG6_T;

//bbpll2_reg7
typedef union {
    REG32 v;
    struct {
        REG32 pll_sdm_resetn_reg_tx : 1; // [0]
        REG32 pll_sdm_resetn_dr_tx : 1; // [1]
        REG32 ss_squre_tri_sel_tx : 1; // [2]
        REG32 ss_en_tx : 1; // [3]
        REG32 dither_bypass_tx : 1; // [4]
        REG32 int_dec_sel_tx : 3; // [7:5]
        REG32 reser_sdm_tx : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_BBPLL2_REG7_T;

//bbpll2_reg8
typedef union {
    REG32 v;
    struct {
        REG32 pll_ss_peri_ct_tx : 8; // [7:0]
        REG32 pll_ss_devi_ct_tx : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_BBPLL2_REG8_T;

//bbpll2_reg9
typedef union {
    REG32 v;
    struct {
        REG32 pll_reser_dig_2_tx : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_BBPLL2_REG9_T;

//bbpll2_rega
typedef union {
    REG32 v;
    struct {
        REG32 clk_gen_en_reg_tx : 1; // [0]
        REG32 pll_clkout_en_reg_tx : 4; // [4:1]
        REG32 pll_clk_adc_dfe_en_reg_tx : 1; // [5]
        REG32 pll_clk_adc_en_reg_tx : 1; // [6]
        REG32 pll_clk_adc_sel_reg_tx : 2; // [8:7]
        REG32 __11_9 : 3;
        REG32 sdmclk_sel_time_sel_tx : 2; // [13:12]
        REG32 sdm_reset_time_sel_tx : 2; // [15:14]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_BBPLL2_REGA_T;

//bbpll2_regb
typedef union {
    REG32 v;
    struct {
        REG32 __9_0 : 10;
        REG32 pll_lock_steady_tx : 1; // [10], read only
        REG32 pll_clk_ready_tx : 1; // [11], read only
        REG32 pll_sdm_clk_sel_tx : 1; // [12], read only
        REG32 pll_sdm_resetn_tx : 1; // [13], read only
        REG32 pll_lock_tx : 1; // [14], read only
        REG32 pu_pll_tx : 1; // [15], read only
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_BBPLL2_REGB_T;

//bbpll2_regd
typedef union {
    REG32 v;
    struct {
        REG32 lock_counter_sel_tx : 2; // [1:0]
        REG32 pll_clkout_en_counter_sel_tx : 2; // [3:2]
        REG32 vco_reset_dis_tx : 1; // [4]
        REG32 resetn_spll_tx : 1; // [5]
        REG32 __31_6 : 26;
    } b;
} REG_RF_REGISTERS_BBPLL2_REGD_T;

//bbpll2_regf
typedef union {
    REG32 v;
    struct {
        REG32 pll_clk_ad_sel0_tx : 1; // [0]
        REG32 pll_clk_ad_sel1_tx : 1; // [1]
        REG32 pll_mod23_bb_tx : 1; // [2]
        REG32 pll_mod23_bb2_tx : 1; // [3]
        REG32 pll_cnt1_tx : 5; // [8:4]
        REG32 pll_cnt2_tx : 5; // [13:9]
        REG32 __31_14 : 18;
    } b;
} REG_RF_REGISTERS_BBPLL2_REGF_T;

//rxpll_cal_reg1
typedef union {
    REG32 v;
    struct {
        REG32 reg_90_bit0 : 8; // [7:0]
        REG32 reg_90_bit1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_RXPLL_CAL_REG1_T;

//rxpll_cal_reg2
typedef union {
    REG32 v;
    struct {
        REG32 reg_91_bit6to0 : 7; // [6:0]
        REG32 reg_91_bit70 : 1; // [7]
        REG32 reg_91_bit15to8 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_RXPLL_CAL_REG2_T;

//rxpll_cal_reg3
typedef union {
    REG32 v;
    struct {
        REG32 reg_92_bit0 : 8; // [7:0]
        REG32 reg_92_bit1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_RXPLL_CAL_REG3_T;

//rxpll_cal_reg4
typedef union {
    REG32 v;
    struct {
        REG32 rxpll_vco_bits : 11; // [10:0], read only
        REG32 rxpll_cal_ready : 1; // [11], read only
        REG32 rxpll_cnt_enable : 1; // [12], read only
        REG32 rxpll_cal_enable : 1; // [13], read only
        REG32 __31_14 : 18;
    } b;
} REG_RF_REGISTERS_RXPLL_CAL_REG4_T;

//txpll_cal_reg1
typedef union {
    REG32 v;
    struct {
        REG32 reg_98_bit0 : 8; // [7:0]
        REG32 reg_98_bit1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_TXPLL_CAL_REG1_T;

//txpll_cal_reg2
typedef union {
    REG32 v;
    struct {
        REG32 reg_99_bit6to0 : 7; // [6:0]
        REG32 reg_99_bit70 : 1; // [7]
        REG32 reg_99_bit15to8 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_TXPLL_CAL_REG2_T;

//txpll_cal_reg3
typedef union {
    REG32 v;
    struct {
        REG32 reg_9a_bit0 : 8; // [7:0]
        REG32 reg_9a_bit1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_TXPLL_CAL_REG3_T;

//txpll_cal_reg4
typedef union {
    REG32 v;
    struct {
        REG32 txpll_vco_bits : 11; // [10:0], read only
        REG32 txpll_cal_ready : 1; // [11], read only
        REG32 txpll_cnt_enable : 1; // [12], read only
        REG32 txpll_cal_enable : 1; // [13], read only
        REG32 __31_14 : 18;
    } b;
} REG_RF_REGISTERS_TXPLL_CAL_REG4_T;

//gpio_reg
typedef union {
    REG32 v;
    struct {
        REG32 gpio_reg_out : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_GPIO_REG_T;

//strobe_reg
typedef union {
    REG32 v;
    struct {
        REG32 time_sel_after_rampd : 8; // [7:0]
        REG32 slot_number_reg : 2; // [9:8]
        REG32 slot_number_dr : 1; // [10]
        REG32 strobe : 1; // [11]
        REG32 multi_slot_sel : 2; // [13:12]
        REG32 strobe_sel : 1; // [14]
        REG32 __31_15 : 17;
    } b;
} REG_RF_REGISTERS_STROBE_REG_T;

//pa_ctrl_reg1
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curve_sel_0 : 4; // [3:0]
        REG32 ramp_curve_sel_1 : 4; // [7:4]
        REG32 ramp_curve_sel_2 : 4; // [11:8]
        REG32 ramp_curve_sel_3 : 4; // [15:12]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_CTRL_REG1_T;

//pa_ctrl_reg2
typedef union {
    REG32 v;
    struct {
        REG32 ramp_mult_factor_0 : 8; // [7:0]
        REG32 ramp_mult_factor_1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_CTRL_REG2_T;

//pa_ctrl_reg3
typedef union {
    REG32 v;
    struct {
        REG32 ramp_mult_factor_2 : 8; // [7:0]
        REG32 ramp_mult_factor_3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_CTRL_REG3_T;

//pa_ctrl_reg4
typedef union {
    REG32 v;
    struct {
        REG32 ramp_dac_din_dr : 1; // [0]
        REG32 ramp_dac_din_reg : 10; // [10:1]
        REG32 __31_11 : 21;
    } b;
} REG_RF_REGISTERS_PA_CTRL_REG4_T;

//ana_ctrl_reg0
typedef union {
    REG32 v;
    struct {
        REG32 reg_100_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG0_T;

//ana_ctrl_reg1
typedef union {
    REG32 v;
    struct {
        REG32 reg_101_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG1_T;

//ana_ctrl_reg2
typedef union {
    REG32 v;
    struct {
        REG32 reg_102_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG2_T;

//ana_ctrl_reg3
typedef union {
    REG32 v;
    struct {
        REG32 reg_103_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG3_T;

//ana_ctrl_reg4
typedef union {
    REG32 v;
    struct {
        REG32 reg_104_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG4_T;

//ana_ctrl_reg5
typedef union {
    REG32 v;
    struct {
        REG32 reg_105_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG5_T;

//ana_ctrl_reg6
typedef union {
    REG32 v;
    struct {
        REG32 reg_106_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG6_T;

//ana_ctrl_reg7
typedef union {
    REG32 v;
    struct {
        REG32 reg_107_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG7_T;

//ana_ctrl_reg8
typedef union {
    REG32 v;
    struct {
        REG32 reg_108_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG8_T;

//ana_ctrl_reg9
typedef union {
    REG32 v;
    struct {
        REG32 reg_109_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG9_T;

//ana_ctrl_rega
typedef union {
    REG32 v;
    struct {
        REG32 reg_10a_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REGA_T;

//ana_ctrl_regb
typedef union {
    REG32 v;
    struct {
        REG32 reg_10b_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REGB_T;

//ana_ctrl_regc
typedef union {
    REG32 v;
    struct {
        REG32 reg_10c_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REGC_T;

//ana_ctrl_regd
typedef union {
    REG32 v;
    struct {
        REG32 reg_10d_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REGD_T;

//ana_ctrl_rege
typedef union {
    REG32 v;
    struct {
        REG32 reg_10e_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REGE_T;

//ana_ctrl_regf
typedef union {
    REG32 v;
    struct {
        REG32 reg_10f_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REGF_T;

//ana_ctrl_reg10
typedef union {
    REG32 v;
    struct {
        REG32 reg_110_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG10_T;

//ana_ctrl_reg11
typedef union {
    REG32 v;
    struct {
        REG32 reg_111_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG11_T;

//ana_ctrl_reg12
typedef union {
    REG32 v;
    struct {
        REG32 reg_112_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG12_T;

//ana_ctrl_reg13
typedef union {
    REG32 v;
    struct {
        REG32 reg_113_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG13_T;

//ana_ctrl_reg14
typedef union {
    REG32 v;
    struct {
        REG32 reg_114_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG14_T;

//ana_ctrl_reg15
typedef union {
    REG32 v;
    struct {
        REG32 reg_115_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG15_T;

//ana_ctrl_reg16
typedef union {
    REG32 v;
    struct {
        REG32 reg_116_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG16_T;

//ana_ctrl_reg17
typedef union {
    REG32 v;
    struct {
        REG32 reg_117_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG17_T;

//ana_ctrl_reg18
typedef union {
    REG32 v;
    struct {
        REG32 reg_118_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG18_T;

//ana_ctrl_reg19
typedef union {
    REG32 v;
    struct {
        REG32 reg_119_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG19_T;

//ana_ctrl_reg1a
typedef union {
    REG32 v;
    struct {
        REG32 reg_11a_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG1A_T;

//ana_ctrl_reg1b
typedef union {
    REG32 v;
    struct {
        REG32 reg_11b_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG1B_T;

//ana_ctrl_reg1c
typedef union {
    REG32 v;
    struct {
        REG32 reg_11c_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG1C_T;

//ana_ctrl_reg1d
typedef union {
    REG32 v;
    struct {
        REG32 reg_11d_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG1D_T;

//ana_ctrl_reg1e
typedef union {
    REG32 v;
    struct {
        REG32 reg_11e_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG1E_T;

//ana_ctrl_reg1f
typedef union {
    REG32 v;
    struct {
        REG32 reg_11f_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG1F_T;

//ana_ctrl_reg20
typedef union {
    REG32 v;
    struct {
        REG32 reg_120_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG20_T;

//ana_ctrl_reg21
typedef union {
    REG32 v;
    struct {
        REG32 reg_121_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG21_T;

//ana_ctrl_reg22
typedef union {
    REG32 v;
    struct {
        REG32 reg_122_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG22_T;

//ana_ctrl_reg23
typedef union {
    REG32 v;
    struct {
        REG32 reg_123_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG23_T;

//ana_ctrl_reg24
typedef union {
    REG32 v;
    struct {
        REG32 reg_124_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG24_T;

//ana_ctrl_reg25
typedef union {
    REG32 v;
    struct {
        REG32 reg_125_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG25_T;

//ana_ctrl_reg26
typedef union {
    REG32 v;
    struct {
        REG32 reg_126_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG26_T;

//ana_ctrl_reg27
typedef union {
    REG32 v;
    struct {
        REG32 reg_127_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG27_T;

//ana_ctrl_reg28
typedef union {
    REG32 v;
    struct {
        REG32 reg_128_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG28_T;

//ana_ctrl_reg29
typedef union {
    REG32 v;
    struct {
        REG32 reg_129_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG29_T;

//ana_ctrl_reg2a
typedef union {
    REG32 v;
    struct {
        REG32 reg_12a_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG2A_T;

//ana_ctrl_reg2b
typedef union {
    REG32 v;
    struct {
        REG32 reg_12b_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG2B_T;

//ana_ctrl_reg2c
typedef union {
    REG32 v;
    struct {
        REG32 reg_12c_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG2C_T;

//ana_ctrl_reg2d
typedef union {
    REG32 v;
    struct {
        REG32 reg_12d_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG2D_T;

//ana_ctrl_reg2e
typedef union {
    REG32 v;
    struct {
        REG32 reg_12e_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG2E_T;

//ana_ctrl_reg2f
typedef union {
    REG32 v;
    struct {
        REG32 reg_12f_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG2F_T;

//ana_ctrl_reg30
typedef union {
    REG32 v;
    struct {
        REG32 reg_130_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG30_T;

//ana_ctrl_reg31
typedef union {
    REG32 v;
    struct {
        REG32 reg_131_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG31_T;

//ana_ctrl_reg32
typedef union {
    REG32 v;
    struct {
        REG32 reg_132_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG32_T;

//ana_ctrl_reg33
typedef union {
    REG32 v;
    struct {
        REG32 reg_133_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG33_T;

//ana_ctrl_reg34
typedef union {
    REG32 v;
    struct {
        REG32 reg_134_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG34_T;

//ana_ctrl_reg35
typedef union {
    REG32 v;
    struct {
        REG32 reg_135_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG35_T;

//ana_ctrl_reg36
typedef union {
    REG32 v;
    struct {
        REG32 reg_136_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG36_T;

//ana_ctrl_reg37
typedef union {
    REG32 v;
    struct {
        REG32 reg_137_bit : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_CTRL_REG37_T;

//control_rf_reg0
typedef union {
    REG32 v;
    struct {
        REG32 dccal_i_enable_reg : 1; // [0]
        REG32 dccal_q_enable_reg : 1; // [1]
        REG32 rx_tx_rf : 1; // [2]
        REG32 __31_3 : 29;
    } b;
} REG_RF_REGISTERS_CONTROL_RF_REG0_T;

//control_rf_reg1
typedef union {
    REG32 v;
    struct {
        REG32 polarity_q : 1; // [0]
        REG32 polarity_i : 1; // [1]
        REG32 thermo_cal_enable : 1; // [2]
        REG32 gpio_auxclk_en_reg : 1; // [3]
        REG32 gpio_auxclk_en_dr : 1; // [4]
        REG32 aux_buf_delay : 1; // [5]
        REG32 gpio_auxclk_en_nodelay : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_RF_REGISTERS_CONTROL_RF_REG1_T;

//control_rf_reg2
typedef union {
    REG32 v;
    struct {
        REG32 cnt_max_dccal_i : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_CONTROL_RF_REG2_T;

//control_rf_reg3
typedef union {
    REG32 v;
    struct {
        REG32 cnt_max_dccal_q : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_RF_REGISTERS_CONTROL_RF_REG3_T;

//control_rf_reg6
typedef union {
    REG32 v;
    struct {
        REG32 chip_tx_mode_reg : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_RF_REGISTERS_CONTROL_RF_REG6_T;

//clk_gen_reg0
typedef union {
    REG32 v;
    struct {
        REG32 freq_clk_div_0 : 3; // [2:0]
        REG32 freq_clk_div_1 : 3; // [5:3]
        REG32 freq_clk_div_2 : 3; // [8:6]
        REG32 freq_clk_div_3 : 3; // [11:9]
        REG32 freq_clk_div_4 : 3; // [14:12]
        REG32 __31_15 : 17;
    } b;
} REG_RF_REGISTERS_CLK_GEN_REG0_T;

//clk_gen_reg1
typedef union {
    REG32 v;
    struct {
        REG32 freq_clk_div_5 : 3; // [2:0]
        REG32 freq_clk_div_6 : 3; // [5:3]
        REG32 freq_clk_div_7 : 3; // [8:6]
        REG32 __31_9 : 23;
    } b;
} REG_RF_REGISTERS_CLK_GEN_REG1_T;

//clk_gen_reg2
typedef union {
    REG32 v;
    struct {
        REG32 enable_clk_div : 8; // [7:0]
        REG32 inv_clk_div : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_CLK_GEN_REG2_T;

//chip_id_reg0
typedef union {
    REG32 v;
    struct {
        REG32 chip_id0 : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_CHIP_ID_REG0_T;

//chip_id_reg1
typedef union {
    REG32 v;
    struct {
        REG32 chip_id1 : 8; // [7:0], read only
        REG32 __31_8 : 24;
    } b;
} REG_RF_REGISTERS_CHIP_ID_REG1_T;

//chip_id_reg2
typedef union {
    REG32 v;
    struct {
        REG32 revision_id : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_CHIP_ID_REG2_T;

//mean_dcccal_i_reg0
typedef union {
    REG32 v;
    struct {
        REG32 mean_dccal_i0 : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_MEAN_DCCCAL_I_REG0_T;

//mean_dcccal_i_reg1
typedef union {
    REG32 v;
    struct {
        REG32 mean_dccal_i1 : 1; // [0], read only
        REG32 __31_1 : 31;
    } b;
} REG_RF_REGISTERS_MEAN_DCCCAL_I_REG1_T;

//mean_dcccal_q_reg
typedef union {
    REG32 v;
    struct {
        REG32 mean_dccal_q : 9; // [8:0], read only
        REG32 __31_9 : 23;
    } b;
} REG_RF_REGISTERS_MEAN_DCCCAL_Q_REG_T;

//revid_reg
typedef union {
    REG32 v;
    struct {
        REG32 revid_rx : 4; // [3:0], read only
        REG32 revid_tx : 4; // [7:4], read only
        REG32 __31_8 : 24;
    } b;
} REG_RF_REGISTERS_REVID_REG_T;

//ana_rd_reg0
typedef union {
    REG32 v;
    struct {
        REG32 reg_221_bit : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_RD_REG0_T;

//ana_rd_reg1
typedef union {
    REG32 v;
    struct {
        REG32 reg_222_bit : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_RD_REG1_T;

//ana_rd_reg2
typedef union {
    REG32 v;
    struct {
        REG32 reg_223_bit : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_RD_REG2_T;

//ana_rd_reg3
typedef union {
    REG32 v;
    struct {
        REG32 reg_224_bit : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_ANA_RD_REG3_T;

//pa_ramp_reg0
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv0_p0 : 8; // [7:0]
        REG32 ramp_curv0_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG0_T;

//pa_ramp_reg1
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv0_p2 : 8; // [7:0]
        REG32 ramp_curv0_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG1_T;

//pa_ramp_reg2
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv0_p4 : 8; // [7:0]
        REG32 ramp_curv0_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG2_T;

//pa_ramp_reg3
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv0_p6 : 8; // [7:0]
        REG32 ramp_curv0_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG3_T;

//pa_ramp_reg4
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv0_p8 : 8; // [7:0]
        REG32 ramp_curv0_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG4_T;

//pa_ramp_reg5
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv0_pa : 8; // [7:0]
        REG32 ramp_curv0_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG5_T;

//pa_ramp_reg6
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv0_pc : 8; // [7:0]
        REG32 ramp_curv0_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG6_T;

//pa_ramp_reg7
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv0_pe : 8; // [7:0]
        REG32 ramp_curv0_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG7_T;

//pa_ramp_reg10
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv1_p0 : 8; // [7:0]
        REG32 ramp_curv1_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG10_T;

//pa_ramp_reg11
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv1_p2 : 8; // [7:0]
        REG32 ramp_curv1_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG11_T;

//pa_ramp_reg12
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv1_p4 : 8; // [7:0]
        REG32 ramp_curv1_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG12_T;

//pa_ramp_reg13
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv1_p6 : 8; // [7:0]
        REG32 ramp_curv1_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG13_T;

//pa_ramp_reg14
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv1_p8 : 8; // [7:0]
        REG32 ramp_curv1_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG14_T;

//pa_ramp_reg15
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv1_pa : 8; // [7:0]
        REG32 ramp_curv1_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG15_T;

//pa_ramp_reg16
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv1_pc : 8; // [7:0]
        REG32 ramp_curv1_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG16_T;

//pa_ramp_reg17
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv1_pe : 8; // [7:0]
        REG32 ramp_curv1_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG17_T;

//pa_ramp_reg20
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv2_p0 : 8; // [7:0]
        REG32 ramp_curv2_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG20_T;

//pa_ramp_reg21
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv2_p2 : 8; // [7:0]
        REG32 ramp_curv2_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG21_T;

//pa_ramp_reg22
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv2_p4 : 8; // [7:0]
        REG32 ramp_curv2_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG22_T;

//pa_ramp_reg23
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv2_p6 : 8; // [7:0]
        REG32 ramp_curv2_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG23_T;

//pa_ramp_reg24
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv2_p8 : 8; // [7:0]
        REG32 ramp_curv2_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG24_T;

//pa_ramp_reg25
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv2_pa : 8; // [7:0]
        REG32 ramp_curv2_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG25_T;

//pa_ramp_reg26
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv2_pc : 8; // [7:0]
        REG32 ramp_curv2_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG26_T;

//pa_ramp_reg27
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv2_pe : 8; // [7:0]
        REG32 ramp_curv2_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG27_T;

//pa_ramp_reg30
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv3_p0 : 8; // [7:0]
        REG32 ramp_curv3_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG30_T;

//pa_ramp_reg31
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv3_p2 : 8; // [7:0]
        REG32 ramp_curv3_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG31_T;

//pa_ramp_reg32
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv3_p4 : 8; // [7:0]
        REG32 ramp_curv3_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG32_T;

//pa_ramp_reg33
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv3_p6 : 8; // [7:0]
        REG32 ramp_curv3_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG33_T;

//pa_ramp_reg34
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv3_p8 : 8; // [7:0]
        REG32 ramp_curv3_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG34_T;

//pa_ramp_reg35
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv3_pa : 8; // [7:0]
        REG32 ramp_curv3_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG35_T;

//pa_ramp_reg36
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv3_pc : 8; // [7:0]
        REG32 ramp_curv3_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG36_T;

//pa_ramp_reg37
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv3_pe : 8; // [7:0]
        REG32 ramp_curv3_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG37_T;

//pa_ramp_reg40
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv4_p0 : 8; // [7:0]
        REG32 ramp_curv4_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG40_T;

//pa_ramp_reg41
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv4_p2 : 8; // [7:0]
        REG32 ramp_curv4_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG41_T;

//pa_ramp_reg42
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv4_p4 : 8; // [7:0]
        REG32 ramp_curv4_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG42_T;

//pa_ramp_reg43
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv4_p6 : 8; // [7:0]
        REG32 ramp_curv4_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG43_T;

//pa_ramp_reg44
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv4_p8 : 8; // [7:0]
        REG32 ramp_curv4_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG44_T;

//pa_ramp_reg45
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv4_pa : 8; // [7:0]
        REG32 ramp_curv4_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG45_T;

//pa_ramp_reg46
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv4_pc : 8; // [7:0]
        REG32 ramp_curv4_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG46_T;

//pa_ramp_reg47
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv4_pe : 8; // [7:0]
        REG32 ramp_curv4_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG47_T;

//pa_ramp_reg50
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv5_p0 : 8; // [7:0]
        REG32 ramp_curv5_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG50_T;

//pa_ramp_reg51
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv5_p2 : 8; // [7:0]
        REG32 ramp_curv5_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG51_T;

//pa_ramp_reg52
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv5_p4 : 8; // [7:0]
        REG32 ramp_curv5_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG52_T;

//pa_ramp_reg53
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv5_p6 : 8; // [7:0]
        REG32 ramp_curv5_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG53_T;

//pa_ramp_reg54
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv5_p8 : 8; // [7:0]
        REG32 ramp_curv5_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG54_T;

//pa_ramp_reg55
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv5_pa : 8; // [7:0]
        REG32 ramp_curv5_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG55_T;

//pa_ramp_reg56
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv5_pc : 8; // [7:0]
        REG32 ramp_curv5_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG56_T;

//pa_ramp_reg57
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv5_pe : 8; // [7:0]
        REG32 ramp_curv5_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG57_T;

//pa_ramp_reg60
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv6_p0 : 8; // [7:0]
        REG32 ramp_curv6_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG60_T;

//pa_ramp_reg61
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv6_p2 : 8; // [7:0]
        REG32 ramp_curv6_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG61_T;

//pa_ramp_reg62
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv6_p4 : 8; // [7:0]
        REG32 ramp_curv6_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG62_T;

//pa_ramp_reg63
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv6_p6 : 8; // [7:0]
        REG32 ramp_curv6_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG63_T;

//pa_ramp_reg64
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv6_p8 : 8; // [7:0]
        REG32 ramp_curv6_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG64_T;

//pa_ramp_reg65
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv6_pa : 8; // [7:0]
        REG32 ramp_curv6_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG65_T;

//pa_ramp_reg66
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv6_pc : 8; // [7:0]
        REG32 ramp_curv6_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG66_T;

//pa_ramp_reg67
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv6_pe : 8; // [7:0]
        REG32 ramp_curv6_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG67_T;

//pa_ramp_reg70
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv7_p0 : 8; // [7:0]
        REG32 ramp_curv7_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG70_T;

//pa_ramp_reg71
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv7_p2 : 8; // [7:0]
        REG32 ramp_curv7_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG71_T;

//pa_ramp_reg72
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv7_p4 : 8; // [7:0]
        REG32 ramp_curv7_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG72_T;

//pa_ramp_reg73
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv7_p6 : 8; // [7:0]
        REG32 ramp_curv7_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG73_T;

//pa_ramp_reg74
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv7_p8 : 8; // [7:0]
        REG32 ramp_curv7_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG74_T;

//pa_ramp_reg75
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv7_pa : 8; // [7:0]
        REG32 ramp_curv7_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG75_T;

//pa_ramp_reg76
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv7_pc : 8; // [7:0]
        REG32 ramp_curv7_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG76_T;

//pa_ramp_reg77
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv7_pe : 8; // [7:0]
        REG32 ramp_curv7_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG77_T;

//pa_ramp_reg80
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv8_p0 : 8; // [7:0]
        REG32 ramp_curv8_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG80_T;

//pa_ramp_reg81
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv8_p2 : 8; // [7:0]
        REG32 ramp_curv8_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG81_T;

//pa_ramp_reg82
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv8_p4 : 8; // [7:0]
        REG32 ramp_curv8_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG82_T;

//pa_ramp_reg83
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv8_p6 : 8; // [7:0]
        REG32 ramp_curv8_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG83_T;

//pa_ramp_reg84
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv8_p8 : 8; // [7:0]
        REG32 ramp_curv8_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG84_T;

//pa_ramp_reg85
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv8_pa : 8; // [7:0]
        REG32 ramp_curv8_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG85_T;

//pa_ramp_reg86
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv8_pc : 8; // [7:0]
        REG32 ramp_curv8_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG86_T;

//pa_ramp_reg87
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv8_pe : 8; // [7:0]
        REG32 ramp_curv8_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG87_T;

//pa_ramp_reg90
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv9_p0 : 8; // [7:0]
        REG32 ramp_curv9_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG90_T;

//pa_ramp_reg91
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv9_p2 : 8; // [7:0]
        REG32 ramp_curv9_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG91_T;

//pa_ramp_reg92
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv9_p4 : 8; // [7:0]
        REG32 ramp_curv9_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG92_T;

//pa_ramp_reg93
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv9_p6 : 8; // [7:0]
        REG32 ramp_curv9_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG93_T;

//pa_ramp_reg94
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv9_p8 : 8; // [7:0]
        REG32 ramp_curv9_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG94_T;

//pa_ramp_reg95
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv9_pa : 8; // [7:0]
        REG32 ramp_curv9_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG95_T;

//pa_ramp_reg96
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv9_pc : 8; // [7:0]
        REG32 ramp_curv9_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG96_T;

//pa_ramp_reg97
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv9_pe : 8; // [7:0]
        REG32 ramp_curv9_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG97_T;

//pa_ramp_rega0
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curva_p0 : 8; // [7:0]
        REG32 ramp_curva_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGA0_T;

//pa_ramp_rega1
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curva_p2 : 8; // [7:0]
        REG32 ramp_curva_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGA1_T;

//pa_ramp_rega2
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curva_p4 : 8; // [7:0]
        REG32 ramp_curva_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGA2_T;

//pa_ramp_rega3
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curva_p6 : 8; // [7:0]
        REG32 ramp_curva_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGA3_T;

//pa_ramp_rega4
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curva_p8 : 8; // [7:0]
        REG32 ramp_curva_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGA4_T;

//pa_ramp_rega5
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curva_pa : 8; // [7:0]
        REG32 ramp_curva_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGA5_T;

//pa_ramp_rega6
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curva_pc : 8; // [7:0]
        REG32 ramp_curva_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGA6_T;

//pa_ramp_rega7
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curva_pe : 8; // [7:0]
        REG32 ramp_curva_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGA7_T;

//pa_ramp_regb0
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvb_p0 : 8; // [7:0]
        REG32 ramp_curvb_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGB0_T;

//pa_ramp_regb1
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvb_p2 : 8; // [7:0]
        REG32 ramp_curvb_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGB1_T;

//pa_ramp_regb2
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvb_p4 : 8; // [7:0]
        REG32 ramp_curvb_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGB2_T;

//pa_ramp_regb3
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvb_p6 : 8; // [7:0]
        REG32 ramp_curvb_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGB3_T;

//pa_ramp_regb4
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvb_p8 : 8; // [7:0]
        REG32 ramp_curvb_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGB4_T;

//pa_ramp_regb5
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvb_pa : 8; // [7:0]
        REG32 ramp_curvb_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGB5_T;

//pa_ramp_regb6
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvb_pc : 8; // [7:0]
        REG32 ramp_curvb_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGB6_T;

//pa_ramp_regb7
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvb_pe : 8; // [7:0]
        REG32 ramp_curvb_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGB7_T;

//pa_ramp_regc0
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvc_p0 : 8; // [7:0]
        REG32 ramp_curvc_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGC0_T;

//pa_ramp_regc1
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvc_p2 : 8; // [7:0]
        REG32 ramp_curvc_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGC1_T;

//pa_ramp_regc2
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvc_p4 : 8; // [7:0]
        REG32 ramp_curvc_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGC2_T;

//pa_ramp_regc3
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvc_p6 : 8; // [7:0]
        REG32 ramp_curvc_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGC3_T;

//pa_ramp_regc4
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvc_p8 : 8; // [7:0]
        REG32 ramp_curvc_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGC4_T;

//pa_ramp_regc5
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvc_pa : 8; // [7:0]
        REG32 ramp_curvc_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGC5_T;

//pa_ramp_regc6
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvc_pc : 8; // [7:0]
        REG32 ramp_curvc_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGC6_T;

//pa_ramp_regc7
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvc_pe : 8; // [7:0]
        REG32 ramp_curvc_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGC7_T;

//pa_ramp_regd0
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvd_p0 : 8; // [7:0]
        REG32 ramp_curvd_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGD0_T;

//pa_ramp_regd1
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvd_p2 : 8; // [7:0]
        REG32 ramp_curvd_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGD1_T;

//pa_ramp_regd2
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvd_p4 : 8; // [7:0]
        REG32 ramp_curvd_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGD2_T;

//pa_ramp_regd3
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvd_p6 : 8; // [7:0]
        REG32 ramp_curvd_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGD3_T;

//pa_ramp_regd4
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvd_p8 : 8; // [7:0]
        REG32 ramp_curvd_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGD4_T;

//pa_ramp_regd5
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvd_pa : 8; // [7:0]
        REG32 ramp_curvd_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGD5_T;

//pa_ramp_regd6
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvd_pc : 8; // [7:0]
        REG32 ramp_curvd_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGD6_T;

//pa_ramp_regd7
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvd_pe : 8; // [7:0]
        REG32 ramp_curvd_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGD7_T;

//pa_ramp_rege0
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curve_p0 : 8; // [7:0]
        REG32 ramp_curve_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGE0_T;

//pa_ramp_rege1
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curve_p2 : 8; // [7:0]
        REG32 ramp_curve_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGE1_T;

//pa_ramp_rege2
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curve_p4 : 8; // [7:0]
        REG32 ramp_curve_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGE2_T;

//pa_ramp_rege3
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curve_p6 : 8; // [7:0]
        REG32 ramp_curve_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGE3_T;

//pa_ramp_rege4
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curve_p8 : 8; // [7:0]
        REG32 ramp_curve_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGE4_T;

//pa_ramp_rege5
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curve_pa : 8; // [7:0]
        REG32 ramp_curve_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGE5_T;

//pa_ramp_rege6
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curve_pc : 8; // [7:0]
        REG32 ramp_curve_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGE6_T;

//pa_ramp_rege7
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curve_pe : 8; // [7:0]
        REG32 ramp_curve_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGE7_T;

//pa_ramp_regf0
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvf_p0 : 8; // [7:0]
        REG32 ramp_curvf_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGF0_T;

//pa_ramp_regf1
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvf_p2 : 8; // [7:0]
        REG32 ramp_curvf_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGF1_T;

//pa_ramp_regf2
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvf_p4 : 8; // [7:0]
        REG32 ramp_curvf_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGF2_T;

//pa_ramp_regf3
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvf_p6 : 8; // [7:0]
        REG32 ramp_curvf_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGF3_T;

//pa_ramp_regf4
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvf_p8 : 8; // [7:0]
        REG32 ramp_curvf_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGF4_T;

//pa_ramp_regf5
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvf_pa : 8; // [7:0]
        REG32 ramp_curvf_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGF5_T;

//pa_ramp_regf6
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvf_pc : 8; // [7:0]
        REG32 ramp_curvf_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGF6_T;

//pa_ramp_regf7
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curvf_pe : 8; // [7:0]
        REG32 ramp_curvf_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REGF7_T;

//pa_ramp_reg100
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv10_p0 : 8; // [7:0]
        REG32 ramp_curv10_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG100_T;

//pa_ramp_reg101
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv10_p2 : 8; // [7:0]
        REG32 ramp_curv10_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG101_T;

//pa_ramp_reg102
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv10_p4 : 8; // [7:0]
        REG32 ramp_curv10_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG102_T;

//pa_ramp_reg103
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv10_p6 : 8; // [7:0]
        REG32 ramp_curv10_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG103_T;

//pa_ramp_reg104
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv10_p8 : 8; // [7:0]
        REG32 ramp_curv10_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG104_T;

//pa_ramp_reg105
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv10_pa : 8; // [7:0]
        REG32 ramp_curv10_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG105_T;

//pa_ramp_reg106
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv10_pc : 8; // [7:0]
        REG32 ramp_curv10_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG106_T;

//pa_ramp_reg107
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv10_pe : 8; // [7:0]
        REG32 ramp_curv10_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG107_T;

//pa_ramp_reg110
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv11_p0 : 8; // [7:0]
        REG32 ramp_curv11_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG110_T;

//pa_ramp_reg111
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv11_p2 : 8; // [7:0]
        REG32 ramp_curv11_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG111_T;

//pa_ramp_reg112
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv11_p4 : 8; // [7:0]
        REG32 ramp_curv11_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG112_T;

//pa_ramp_reg113
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv11_p6 : 8; // [7:0]
        REG32 ramp_curv11_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG113_T;

//pa_ramp_reg114
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv11_p8 : 8; // [7:0]
        REG32 ramp_curv11_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG114_T;

//pa_ramp_reg115
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv11_pa : 8; // [7:0]
        REG32 ramp_curv11_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG115_T;

//pa_ramp_reg116
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv11_pc : 8; // [7:0]
        REG32 ramp_curv11_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG116_T;

//pa_ramp_reg117
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv11_pe : 8; // [7:0]
        REG32 ramp_curv11_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG117_T;

//pa_ramp_reg120
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv12_p0 : 8; // [7:0]
        REG32 ramp_curv12_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG120_T;

//pa_ramp_reg121
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv12_p2 : 8; // [7:0]
        REG32 ramp_curv12_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG121_T;

//pa_ramp_reg122
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv12_p4 : 8; // [7:0]
        REG32 ramp_curv12_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG122_T;

//pa_ramp_reg123
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv12_p6 : 8; // [7:0]
        REG32 ramp_curv12_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG123_T;

//pa_ramp_reg124
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv12_p8 : 8; // [7:0]
        REG32 ramp_curv12_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG124_T;

//pa_ramp_reg125
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv12_pa : 8; // [7:0]
        REG32 ramp_curv12_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG125_T;

//pa_ramp_reg126
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv12_pc : 8; // [7:0]
        REG32 ramp_curv12_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG126_T;

//pa_ramp_reg127
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv12_pe : 8; // [7:0]
        REG32 ramp_curv12_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG127_T;

//pa_ramp_reg130
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv13_p0 : 8; // [7:0]
        REG32 ramp_curv13_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG130_T;

//pa_ramp_reg131
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv13_p2 : 8; // [7:0]
        REG32 ramp_curv13_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG131_T;

//pa_ramp_reg132
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv13_p4 : 8; // [7:0]
        REG32 ramp_curv13_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG132_T;

//pa_ramp_reg133
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv13_p6 : 8; // [7:0]
        REG32 ramp_curv13_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG133_T;

//pa_ramp_reg134
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv13_p8 : 8; // [7:0]
        REG32 ramp_curv13_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG134_T;

//pa_ramp_reg135
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv13_pa : 8; // [7:0]
        REG32 ramp_curv13_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG135_T;

//pa_ramp_reg136
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv13_pc : 8; // [7:0]
        REG32 ramp_curv13_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG136_T;

//pa_ramp_reg137
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv13_pe : 8; // [7:0]
        REG32 ramp_curv13_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG137_T;

//pa_ramp_reg140
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv14_p0 : 8; // [7:0]
        REG32 ramp_curv14_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG140_T;

//pa_ramp_reg141
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv14_p2 : 8; // [7:0]
        REG32 ramp_curv14_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG141_T;

//pa_ramp_reg142
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv14_p4 : 8; // [7:0]
        REG32 ramp_curv14_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG142_T;

//pa_ramp_reg143
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv14_p6 : 8; // [7:0]
        REG32 ramp_curv14_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG143_T;

//pa_ramp_reg144
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv14_p8 : 8; // [7:0]
        REG32 ramp_curv14_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG144_T;

//pa_ramp_reg145
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv14_pa : 8; // [7:0]
        REG32 ramp_curv14_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG145_T;

//pa_ramp_reg146
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv14_pc : 8; // [7:0]
        REG32 ramp_curv14_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG146_T;

//pa_ramp_reg147
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv14_pe : 8; // [7:0]
        REG32 ramp_curv14_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG147_T;

//pa_ramp_reg150
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv15_p0 : 8; // [7:0]
        REG32 ramp_curv15_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG150_T;

//pa_ramp_reg151
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv15_p2 : 8; // [7:0]
        REG32 ramp_curv15_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG151_T;

//pa_ramp_reg152
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv15_p4 : 8; // [7:0]
        REG32 ramp_curv15_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG152_T;

//pa_ramp_reg153
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv15_p6 : 8; // [7:0]
        REG32 ramp_curv15_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG153_T;

//pa_ramp_reg154
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv15_p8 : 8; // [7:0]
        REG32 ramp_curv15_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG154_T;

//pa_ramp_reg155
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv15_pa : 8; // [7:0]
        REG32 ramp_curv15_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG155_T;

//pa_ramp_reg156
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv15_pc : 8; // [7:0]
        REG32 ramp_curv15_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG156_T;

//pa_ramp_reg157
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv15_pe : 8; // [7:0]
        REG32 ramp_curv15_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG157_T;

//pa_ramp_reg160
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv16_p0 : 8; // [7:0]
        REG32 ramp_curv16_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG160_T;

//pa_ramp_reg161
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv16_p2 : 8; // [7:0]
        REG32 ramp_curv16_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG161_T;

//pa_ramp_reg162
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv16_p4 : 8; // [7:0]
        REG32 ramp_curv16_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG162_T;

//pa_ramp_reg163
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv16_p6 : 8; // [7:0]
        REG32 ramp_curv16_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG163_T;

//pa_ramp_reg164
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv16_p8 : 8; // [7:0]
        REG32 ramp_curv16_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG164_T;

//pa_ramp_reg165
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv16_pa : 8; // [7:0]
        REG32 ramp_curv16_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG165_T;

//pa_ramp_reg166
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv16_pc : 8; // [7:0]
        REG32 ramp_curv16_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG166_T;

//pa_ramp_reg167
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv16_pe : 8; // [7:0]
        REG32 ramp_curv16_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG167_T;

//pa_ramp_reg170
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv17_p0 : 8; // [7:0]
        REG32 ramp_curv17_p1 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG170_T;

//pa_ramp_reg171
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv17_p2 : 8; // [7:0]
        REG32 ramp_curv17_p3 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG171_T;

//pa_ramp_reg172
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv17_p4 : 8; // [7:0]
        REG32 ramp_curv17_p5 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG172_T;

//pa_ramp_reg173
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv17_p6 : 8; // [7:0]
        REG32 ramp_curv17_p7 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG173_T;

//pa_ramp_reg174
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv17_p8 : 8; // [7:0]
        REG32 ramp_curv17_p9 : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG174_T;

//pa_ramp_reg175
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv17_pa : 8; // [7:0]
        REG32 ramp_curv17_pb : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG175_T;

//pa_ramp_reg176
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv17_pc : 8; // [7:0]
        REG32 ramp_curv17_pd : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG176_T;

//pa_ramp_reg177
typedef union {
    REG32 v;
    struct {
        REG32 ramp_curv17_pe : 8; // [7:0]
        REG32 ramp_curv17_pf : 8; // [15:8]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_PA_RAMP_REG177_T;

//pa_on_h_reg
typedef union {
    REG32 v;
    struct {
        REG32 ramp_dac_th : 10; // [9:0]
        REG32 pa_on_h_dr_ctrl : 1; // [10]
        REG32 pa_on_h_dr_reg : 1; // [11]
        REG32 __31_12 : 20;
    } b;
} REG_RF_REGISTERS_PA_ON_H_REG_T;

//sys_ctrl_reg_20
typedef union {
    REG32 v;
    struct {
        REG32 sys_ctrl2_0 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_SYS_CTRL_REG_20_T;

//sys_ctrl_reg_22
typedef union {
    REG32 v;
    struct {
        REG32 sys_ctrl2_2 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_SYS_CTRL_REG_22_T;

//sys_ctrl_reg_24
typedef union {
    REG32 v;
    struct {
        REG32 sys_ctrl2_4 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_SYS_CTRL_REG_24_T;

//sys_ctrl_reg_26
typedef union {
    REG32 v;
    struct {
        REG32 sys_ctrl2_6 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_SYS_CTRL_REG_26_T;

//sys_ctrl_reg_28
typedef union {
    REG32 v;
    struct {
        REG32 sys_ctrl2_8 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_SYS_CTRL_REG_28_T;

//sys_ctrl_reg_2a
typedef union {
    REG32 v;
    struct {
        REG32 sys_ctrl2_a : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_SYS_CTRL_REG_2A_T;

//sys_ctrl_reg_2c
typedef union {
    REG32 v;
    struct {
        REG32 sys_ctrl2_c : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_SYS_CTRL_REG_2C_T;

//sys_ctrl_reg_2e
typedef union {
    REG32 v;
    struct {
        REG32 sys_ctrl2_e : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_SYS_CTRL_REG_2E_T;

//sys_ctrl_reg_210
typedef union {
    REG32 v;
    struct {
        REG32 sys_ctrl2_10 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_SYS_CTRL_REG_210_T;

//sys_ctrl_reg_212
typedef union {
    REG32 v;
    struct {
        REG32 sys_ctrl2_12 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_SYS_CTRL_REG_212_T;

//sys_ctrl_reg_214
typedef union {
    REG32 v;
    struct {
        REG32 sys_ctrl2_14 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_SYS_CTRL_REG_214_T;

//sys_ctrl_reg_216
typedef union {
    REG32 v;
    struct {
        REG32 sys_ctrl2_16 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_SYS_CTRL_REG_216_T;

//sys_ctrl_reg_218
typedef union {
    REG32 v;
    struct {
        REG32 sys_ctrl2_18 : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_SYS_CTRL_REG_218_T;

//sys_ctrl_reg_21a
typedef union {
    REG32 v;
    struct {
        REG32 sys_ctrl2_1a : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_SYS_CTRL_REG_21A_T;

//sys_ctrl_reg_21c
typedef union {
    REG32 v;
    struct {
        REG32 sys_ctrl2_1c : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_SYS_CTRL_REG_21C_T;

//sys_ctrl_reg_21e
typedef union {
    REG32 v;
    struct {
        REG32 sys_ctrl2_1e : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_RF_REGISTERS_SYS_CTRL_REG_21E_T;

//dlpf_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 dlpf_rstn : 1; // [0]
        REG32 dlpf_iir3_rstn : 1; // [1]
        REG32 dlpf_vco_band_bit_sel : 1; // [2]
        REG32 __31_3 : 29;
    } b;
} REG_RF_REGISTERS_DLPF_CTRL_REG_T;


//int_clear0
#define RF_REGISTERS_INT_CTRL_BIT_0(n) (((n)&0xFFFF)<<0)

//int_clear1
#define RF_REGISTERS_INT_CTRL_BIT_1(n) (((n)&0xFFFF)<<0)

//int2tmcu0
#define RF_REGISTERS_IRQ0(n)       (((n)&0xFFFF)<<0)

//int2tmcu1
#define RF_REGISTERS_IRQ1(n)       (((n)&0xFFFF)<<0)

//afc_freq_rxpll1
#define RF_REGISTERS_FREQ_OFFSET_RXPLL1(n) (((n)&0xFFFF)<<0)

//afc_freq_trxpll2
#define RF_REGISTERS_FREQ_OFFSET_RXPLL2(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_FREQ_OFFSET_TXPLL2(n) (((n)&0xFF)<<0)

//afc_freq_txpll
#define RF_REGISTERS_FREQ_OFFSET_TXPLL1(n) (((n)&0xFFFF)<<0)

//afc_freq_bbpll1
#define RF_REGISTERS_FREQ_OFFSET_BBPLL11(n) (((n)&0xFFFF)<<0)

//afc_freq_bbpll12
#define RF_REGISTERS_FREQ_OFFSET_BBPLL12(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_FREQ_OFFSET_BBPLL22(n) (((n)&0xFF)<<0)

//afc_freq_bbpll2
#define RF_REGISTERS_FREQ_OFFSET_BBPLL21(n) (((n)&0xFFFF)<<0)

//afc_freq_offset_mode
#define RF_REGISTERS_RESERVED_0(n) (((n)&0xFFF)<<4)
#define RF_REGISTERS_FREQ_OFFSET_MODE_BBPLL2 (1<<3)
#define RF_REGISTERS_FREQ_OFFSET_MODE_BBPLL1 (1<<2)
#define RF_REGISTERS_FREQ_OFFSET_MODE_TXPLL (1<<1)
#define RF_REGISTERS_FREQ_OFFSET_MODE_RXPLL (1<<0)

//rxsdm_reg1
//#define RF_REGISTERS_RESERVED_0(n) (((n)&0xFFF)<<4)
#define RF_REGISTERS_DITHER_BYPASS_RXSDM (1<<3)
#define RF_REGISTERS_CLK_FORMER_EDGE_RXSDM (1<<2)
#define RF_REGISTERS_DLL_MODE_RXSDM(n) (((n)&3)<<0)

//rxsdm_reg2
#define RF_REGISTERS_FREQ_RXSDM0(n) (((n)&0xFFFF)<<0)

//rxsdm_reg3
#define RF_REGISTERS_FREQ_RXSDM1(n) (((n)&0xFFFF)<<0)

//rxsdm_reg4
#define RF_REGISTERS_RESERVED_0_X1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_TX_RX_RXSDM   (1<<7)
#define RF_REGISTERS_RESETN_RXSDM  (1<<6)
#define RF_REGISTERS_INT_DEC_SEL_RXSDM(n) (((n)&3)<<4)
#define RF_REGISTERS_FREQ_FORMER_SHIFT_CT_RXSDM(n) (((n)&7)<<1)
#define RF_REGISTERS_FREQ_FORMER_BYPASS_RXSDM (1<<0)

//rxsdm_reg5
//#define RF_REGISTERS_RESERVED_0_X1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RESER_SDM_RXSDM(n) (((n)&0xFF)<<0)

//freq_offset_ini_rx_reg1
#define RF_REGISTERS_FREQ_OFFSET_INI_RXPLL1(n) (((n)&0xFFFF)<<0)

//freq_offset_ini_rx_reg2
#define RF_REGISTERS_FREQ_OFFSET_INI_TXPLL2(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_FREQ_OFFSET_INI_RXPLL2(n) (((n)&0xFF)<<0)

//txsdm_reg1
#define RF_REGISTERS_RESERVED_0_X2(n) (((n)&0x7FF)<<5)
#define RF_REGISTERS_DLL_UPDATE_TXSDM (1<<4)
#define RF_REGISTERS_DITHER_BYPASS_TXSDM (1<<3)
#define RF_REGISTERS_CLK_FORMER_EDGE_TXSDM (1<<2)
#define RF_REGISTERS_DLL_MODE_TXSDM(n) (((n)&3)<<0)

//txsdm_reg4
//#define RF_REGISTERS_RESERVED_0_X1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_TX_RX_TXSDM   (1<<7)
#define RF_REGISTERS_RESETN_TXSDM  (1<<6)
#define RF_REGISTERS_INT_DEC_SEL_TXSDM(n) (((n)&3)<<4)
#define RF_REGISTERS_FREQ_FORMER_SHIFT_CT_TXSDM(n) (((n)&7)<<1)
#define RF_REGISTERS_FREQ_FORMER_BYPASS_TXSDM (1<<0)

//txsdm_reg5
//#define RF_REGISTERS_RESERVED_0_X1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RESER_SDM_TXSDM(n) (((n)&0xFF)<<0)

//freq_offset_ini_tx_reg1
#define RF_REGISTERS_FREQ_OFFSET_INI_TXPLL1(n) (((n)&0xFFFF)<<0)

//freq_offset_ini_bbpll1_reg1
#define RF_REGISTERS_FREQ_OFFSET_INI_BBPLL11(n) (((n)&0xFFFF)<<0)

//freq_offset_ini_bbpll1_reg2
#define RF_REGISTERS_FREQ_OFFSET_INI_BBPLL22(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_FREQ_OFFSET_INI_BBPLL12(n) (((n)&0xFF)<<0)

//freq_offset_ini_bbpll2_reg1
#define RF_REGISTERS_FREQ_OFFSET_INI_BBPLL21(n) (((n)&0xFFFF)<<0)

//bbpll1_reg1
#define RF_REGISTERS_RESERVED_0_X3(n) (((n)&7)<<13)
#define RF_REGISTERS_PLL_CPAUX_BIT_RX(n) (((n)&7)<<10)
#define RF_REGISTERS_PLL_FILTER_IBIT_RX(n) (((n)&7)<<7)
#define RF_REGISTERS_PLL_CP_BIT_RX(n) (((n)&7)<<4)
#define RF_REGISTERS_PLL_VREG_BIT_RX(n) (((n)&15)<<0)

//bbpll1_reg2
//#define RF_REGISTERS_RESERVED_0_X3(n) (((n)&7)<<13)
#define RF_REGISTERS_PLL_RESER_RX(n) (((n)&15)<<9)
#define RF_REGISTERS_PLL_REFMULTI2_EN_RX (1<<8)
#define RF_REGISTERS_PLL_HIGH_TEST_RX (1<<7)
#define RF_REGISTERS_PLL_LOW_TEST_RX (1<<6)
#define RF_REGISTERS_PLL_TEST_EN_RX (1<<5)
#define RF_REGISTERS_PLL_SDM_CLK_TEST_EN_RX (1<<4)
#define RF_REGISTERS_PLL_SDM_CLK_SEL_RST_RX (1<<3)
#define RF_REGISTERS_PLL_SDM_CLK_SEL_NOR_RX (1<<2)
#define RF_REGISTERS_PU_PLL_DR_RX  (1<<1)
#define RF_REGISTERS_PU_PLL_REG_RX (1<<0)

//bbpll1_reg3
#define RF_REGISTERS_PLL_RESER_DIG_1_RX(n) (((n)&0xFFFF)<<0)

//bbpll1_reg5
#define RF_REGISTERS_PLL_SDM_FREQ_RX1(n) (((n)&0xFFFF)<<0)

//bbpll1_reg6
#define RF_REGISTERS_PLL_SDM_FREQ_RX0(n) (((n)&0xFFFF)<<0)

//bbpll1_reg7
#define RF_REGISTERS_RESER_SDM_RX(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_INT_DEC_SEL_RX(n) (((n)&7)<<5)
#define RF_REGISTERS_DITHER_BYPASS_RX (1<<4)
#define RF_REGISTERS_SS_EN_RX      (1<<3)
#define RF_REGISTERS_SS_SQURE_TRI_SEL_RX (1<<2)
#define RF_REGISTERS_PLL_SDM_RESETN_DR_RX (1<<1)
#define RF_REGISTERS_PLL_SDM_RESETN_REG_RX (1<<0)

//bbpll1_reg8
#define RF_REGISTERS_PLL_SS_DEVI_CT_RX(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_PLL_SS_PERI_CT_RX(n) (((n)&0xFF)<<0)

//bbpll1_reg9
#define RF_REGISTERS_PLL_RESER_DIG_2_RX(n) (((n)&0xFFFF)<<0)

//bbpll1_rega
#define RF_REGISTERS_SDM_RESET_TIME_SEL_RX(n) (((n)&3)<<14)
#define RF_REGISTERS_SDMCLK_SEL_TIME_SEL_RX(n) (((n)&3)<<12)
#define RF_REGISTERS_RESERVED_0_X4(n) (((n)&7)<<9)
#define RF_REGISTERS_PLL_CLK_ADC_SEL_REG_RX(n) (((n)&3)<<7)
#define RF_REGISTERS_PLL_CLK_ADC_EN_REG_RX (1<<6)
#define RF_REGISTERS_PLL_CLK_ADC_DFE_EN_REG_RX (1<<5)
#define RF_REGISTERS_PLL_CLKOUT_EN_REG_RX(n) (((n)&15)<<1)
#define RF_REGISTERS_CLK_GEN_EN_REG_RX (1<<0)

//bbpll1_regb
#define RF_REGISTERS_PU_PLL_RX     (1<<15)
#define RF_REGISTERS_PLL_LOCK_RX   (1<<14)
#define RF_REGISTERS_PLL_SDM_RESETN_RX (1<<13)
#define RF_REGISTERS_PLL_SDM_CLK_SEL_RX (1<<12)
#define RF_REGISTERS_PLL_CLK_READY_RX (1<<11)
#define RF_REGISTERS_PLL_LOCK_STEADY_RX (1<<10)
#define RF_REGISTERS_RESERVED_0_X5(n) (((n)&0x3FF)<<0)

//bbpll1_regd
#define RF_REGISTERS_RESERVED_0_X6(n) (((n)&0x3FF)<<6)
#define RF_REGISTERS_RESETN_SPLL_RX (1<<5)
#define RF_REGISTERS_VCO_RESET_DIS_RX (1<<4)
#define RF_REGISTERS_PLL_CLKOUT_EN_COUNTER_SEL_RX(n) (((n)&3)<<2)
#define RF_REGISTERS_LOCK_COUNTER_SEL_RX(n) (((n)&3)<<0)

//bbpll1_regf
#define RF_REGISTERS_RESERVED_0_X7(n) (((n)&3)<<14)
#define RF_REGISTERS_PLL_CNT2_RX(n) (((n)&31)<<9)
#define RF_REGISTERS_PLL_CNT1_RX(n) (((n)&31)<<4)
#define RF_REGISTERS_PLL_MOD23_BB2_RX (1<<3)
#define RF_REGISTERS_PLL_MOD23_BB_RX (1<<2)
#define RF_REGISTERS_PLL_CLK_AD_SEL1_RX (1<<1)
#define RF_REGISTERS_PLL_CLK_AD_SEL0_RX (1<<0)

//bbpll2_reg1
//#define RF_REGISTERS_RESERVED_0_X3(n) (((n)&7)<<13)
#define RF_REGISTERS_PLL_CPAUX_BIT_TX(n) (((n)&7)<<10)
#define RF_REGISTERS_PLL_FILTER_IBIT_TX(n) (((n)&7)<<7)
#define RF_REGISTERS_PLL_CP_BIT_TX(n) (((n)&7)<<4)
#define RF_REGISTERS_PLL_VREG_BIT_TX(n) (((n)&15)<<0)

//bbpll2_reg2
//#define RF_REGISTERS_RESERVED_0_X3(n) (((n)&7)<<13)
#define RF_REGISTERS_PLL_RESER_TX(n) (((n)&15)<<9)
#define RF_REGISTERS_PLL_REFMULTI2_EN_TX (1<<8)
#define RF_REGISTERS_PLL_HIGH_TEST_TX (1<<7)
#define RF_REGISTERS_PLL_LOW_TEST_TX (1<<6)
#define RF_REGISTERS_PLL_TEST_EN_TX (1<<5)
#define RF_REGISTERS_PLL_SDM_CLK_TEST_EN_TX (1<<4)
#define RF_REGISTERS_PLL_SDM_CLK_SEL_RST_TX (1<<3)
#define RF_REGISTERS_PLL_SDM_CLK_SEL_NOR_TX (1<<2)
#define RF_REGISTERS_PU_PLL_DR_TX  (1<<1)
#define RF_REGISTERS_PU_PLL_REG_TX (1<<0)

//bbpll2_reg3
#define RF_REGISTERS_PLL_RESER_DIG_1_TX(n) (((n)&0xFFFF)<<0)

//bbpll2_reg5
#define RF_REGISTERS_PLL_SDM_FREQ_TX1(n) (((n)&0xFFFF)<<0)

//bbpll2_reg6
#define RF_REGISTERS_PLL_SDM_FREQ_TX0(n) (((n)&0xFFFF)<<0)

//bbpll2_reg7
#define RF_REGISTERS_RESER_SDM_TX(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_INT_DEC_SEL_TX(n) (((n)&7)<<5)
#define RF_REGISTERS_DITHER_BYPASS_TX (1<<4)
#define RF_REGISTERS_SS_EN_TX      (1<<3)
#define RF_REGISTERS_SS_SQURE_TRI_SEL_TX (1<<2)
#define RF_REGISTERS_PLL_SDM_RESETN_DR_TX (1<<1)
#define RF_REGISTERS_PLL_SDM_RESETN_REG_TX (1<<0)

//bbpll2_reg8
#define RF_REGISTERS_PLL_SS_DEVI_CT_TX(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_PLL_SS_PERI_CT_TX(n) (((n)&0xFF)<<0)

//bbpll2_reg9
#define RF_REGISTERS_PLL_RESER_DIG_2_TX(n) (((n)&0xFFFF)<<0)

//bbpll2_rega
#define RF_REGISTERS_SDM_RESET_TIME_SEL_TX(n) (((n)&3)<<14)
#define RF_REGISTERS_SDMCLK_SEL_TIME_SEL_TX(n) (((n)&3)<<12)
//#define RF_REGISTERS_RESERVED_0_X4(n) (((n)&7)<<9)
#define RF_REGISTERS_PLL_CLK_ADC_SEL_REG_TX(n) (((n)&3)<<7)
#define RF_REGISTERS_PLL_CLK_ADC_EN_REG_TX (1<<6)
#define RF_REGISTERS_PLL_CLK_ADC_DFE_EN_REG_TX (1<<5)
#define RF_REGISTERS_PLL_CLKOUT_EN_REG_TX(n) (((n)&15)<<1)
#define RF_REGISTERS_CLK_GEN_EN_REG_TX (1<<0)

//bbpll2_regb
#define RF_REGISTERS_PU_PLL_TX     (1<<15)
#define RF_REGISTERS_PLL_LOCK_TX   (1<<14)
#define RF_REGISTERS_PLL_SDM_RESETN_TX (1<<13)
#define RF_REGISTERS_PLL_SDM_CLK_SEL_TX (1<<12)
#define RF_REGISTERS_PLL_CLK_READY_TX (1<<11)
#define RF_REGISTERS_PLL_LOCK_STEADY_TX (1<<10)
//#define RF_REGISTERS_RESERVED_0_X5(n) (((n)&0x3FF)<<0)

//bbpll2_regd
//#define RF_REGISTERS_RESERVED_0_X6(n) (((n)&0x3FF)<<6)
#define RF_REGISTERS_RESETN_SPLL_TX (1<<5)
#define RF_REGISTERS_VCO_RESET_DIS_TX (1<<4)
#define RF_REGISTERS_PLL_CLKOUT_EN_COUNTER_SEL_TX(n) (((n)&3)<<2)
#define RF_REGISTERS_LOCK_COUNTER_SEL_TX(n) (((n)&3)<<0)

//bbpll2_regf
//#define RF_REGISTERS_RESERVED_0_X7(n) (((n)&3)<<14)
#define RF_REGISTERS_PLL_CNT2_TX(n) (((n)&31)<<9)
#define RF_REGISTERS_PLL_CNT1_TX(n) (((n)&31)<<4)
#define RF_REGISTERS_PLL_MOD23_BB2_TX (1<<3)
#define RF_REGISTERS_PLL_MOD23_BB_TX (1<<2)
#define RF_REGISTERS_PLL_CLK_AD_SEL1_TX (1<<1)
#define RF_REGISTERS_PLL_CLK_AD_SEL0_TX (1<<0)

//rxpll_cal_reg1
#define RF_REGISTERS_REG_90_BIT1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_REG_90_BIT0(n) (((n)&0xFF)<<0)

//rxpll_cal_reg2
#define RF_REGISTERS_REG_91_BIT15TO8(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_REG_91_BIT70  (1<<7)
#define RF_REGISTERS_REG_91_BIT6TO0(n) (((n)&0x7F)<<0)

//rxpll_cal_reg3
#define RF_REGISTERS_REG_92_BIT1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_REG_92_BIT0(n) (((n)&0xFF)<<0)

//rxpll_cal_reg4
//#define RF_REGISTERS_RESERVED_0_X7(n) (((n)&3)<<14)
#define RF_REGISTERS_RXPLL_CAL_ENABLE (1<<13)
#define RF_REGISTERS_RXPLL_CNT_ENABLE (1<<12)
#define RF_REGISTERS_RXPLL_CAL_READY (1<<11)
#define RF_REGISTERS_RXPLL_VCO_BITS(n) (((n)&0x7FF)<<0)

//txpll_cal_reg1
#define RF_REGISTERS_REG_98_BIT1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_REG_98_BIT0(n) (((n)&0xFF)<<0)

//txpll_cal_reg2
#define RF_REGISTERS_REG_99_BIT15TO8(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_REG_99_BIT70  (1<<7)
#define RF_REGISTERS_REG_99_BIT6TO0(n) (((n)&0x7F)<<0)

//txpll_cal_reg3
#define RF_REGISTERS_REG_9A_BIT1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_REG_9A_BIT0(n) (((n)&0xFF)<<0)

//txpll_cal_reg4
//#define RF_REGISTERS_RESERVED_0_X7(n) (((n)&3)<<14)
#define RF_REGISTERS_TXPLL_CAL_ENABLE (1<<13)
#define RF_REGISTERS_TXPLL_CNT_ENABLE (1<<12)
#define RF_REGISTERS_TXPLL_CAL_READY (1<<11)
#define RF_REGISTERS_TXPLL_VCO_BITS(n) (((n)&0x7FF)<<0)

//gpio_reg
#define RF_REGISTERS_GPIO_REG_OUT(n) (((n)&0xFFFF)<<0)

//strobe_reg
#define RF_REGISTERS_RESERVED_0_X8 (1<<15)
#define RF_REGISTERS_STROBE_SEL    (1<<14)
#define RF_REGISTERS_MULTI_SLOT_SEL(n) (((n)&3)<<12)
#define RF_REGISTERS_STROBE        (1<<11)
#define RF_REGISTERS_SLOT_NUMBER_DR (1<<10)
#define RF_REGISTERS_SLOT_NUMBER_REG(n) (((n)&3)<<8)
#define RF_REGISTERS_TIME_SEL_AFTER_RAMPD(n) (((n)&0xFF)<<0)

//pa_ctrl_reg1
#define RF_REGISTERS_RAMP_CURVE_SEL_3(n) (((n)&15)<<12)
#define RF_REGISTERS_RAMP_CURVE_SEL_2(n) (((n)&15)<<8)
#define RF_REGISTERS_RAMP_CURVE_SEL_1(n) (((n)&15)<<4)
#define RF_REGISTERS_RAMP_CURVE_SEL_0(n) (((n)&15)<<0)

//pa_ctrl_reg2
#define RF_REGISTERS_RAMP_MULT_FACTOR_1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_MULT_FACTOR_0(n) (((n)&0xFF)<<0)

//pa_ctrl_reg3
#define RF_REGISTERS_RAMP_MULT_FACTOR_3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_MULT_FACTOR_2(n) (((n)&0xFF)<<0)

//pa_ctrl_reg4
#define RF_REGISTERS_RESERVED_0_X9(n) (((n)&31)<<11)
#define RF_REGISTERS_RAMP_DAC_DIN_REG(n) (((n)&0x3FF)<<1)
#define RF_REGISTERS_RAMP_DAC_DIN_DR (1<<0)

//ana_ctrl_reg0
#define RF_REGISTERS_REG_100_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg1
#define RF_REGISTERS_REG_101_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg2
#define RF_REGISTERS_REG_102_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg3
#define RF_REGISTERS_REG_103_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg4
#define RF_REGISTERS_REG_104_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg5
#define RF_REGISTERS_REG_105_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg6
#define RF_REGISTERS_REG_106_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg7
#define RF_REGISTERS_REG_107_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg8
#define RF_REGISTERS_REG_108_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg9
#define RF_REGISTERS_REG_109_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_rega
#define RF_REGISTERS_REG_10A_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_regb
#define RF_REGISTERS_REG_10B_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_regc
#define RF_REGISTERS_REG_10C_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_regd
#define RF_REGISTERS_REG_10D_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_rege
#define RF_REGISTERS_REG_10E_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_regf
#define RF_REGISTERS_REG_10F_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg10
#define RF_REGISTERS_REG_110_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg11
#define RF_REGISTERS_REG_111_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg12
#define RF_REGISTERS_REG_112_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg13
#define RF_REGISTERS_REG_113_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg14
#define RF_REGISTERS_REG_114_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg15
#define RF_REGISTERS_REG_115_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg16
#define RF_REGISTERS_REG_116_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg17
#define RF_REGISTERS_REG_117_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg18
#define RF_REGISTERS_REG_118_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg19
#define RF_REGISTERS_REG_119_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg1a
#define RF_REGISTERS_REG_11A_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg1b
#define RF_REGISTERS_REG_11B_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg1c
#define RF_REGISTERS_REG_11C_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg1d
#define RF_REGISTERS_REG_11D_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg1e
#define RF_REGISTERS_REG_11E_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg1f
#define RF_REGISTERS_REG_11F_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg20
#define RF_REGISTERS_REG_120_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg21
#define RF_REGISTERS_REG_121_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg22
#define RF_REGISTERS_REG_122_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg23
#define RF_REGISTERS_REG_123_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg24
#define RF_REGISTERS_REG_124_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg25
#define RF_REGISTERS_REG_125_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg26
#define RF_REGISTERS_REG_126_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg27
#define RF_REGISTERS_REG_127_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg28
#define RF_REGISTERS_REG_128_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg29
#define RF_REGISTERS_REG_129_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg2a
#define RF_REGISTERS_REG_12A_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg2b
#define RF_REGISTERS_REG_12B_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg2c
#define RF_REGISTERS_REG_12C_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg2d
#define RF_REGISTERS_REG_12D_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg2e
#define RF_REGISTERS_REG_12E_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg2f
#define RF_REGISTERS_REG_12F_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg30
#define RF_REGISTERS_REG_130_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg31
#define RF_REGISTERS_REG_131_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg32
#define RF_REGISTERS_REG_132_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg33
#define RF_REGISTERS_REG_133_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg34
#define RF_REGISTERS_REG_134_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg35
#define RF_REGISTERS_REG_135_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg36
#define RF_REGISTERS_REG_136_BIT(n) (((n)&0xFFFF)<<0)

//ana_ctrl_reg37
#define RF_REGISTERS_REG_137_BIT(n) (((n)&0xFFFF)<<0)

//control_rf_reg0
#define RF_REGISTERS_RESERVED_0_X10(n) (((n)&0x1FFF)<<3)
#define RF_REGISTERS_RX_TX_RF      (1<<2)
#define RF_REGISTERS_DCCAL_Q_ENABLE_REG (1<<1)
#define RF_REGISTERS_DCCAL_I_ENABLE_REG (1<<0)

//control_rf_reg1
#define RF_REGISTERS_RESERVED_0_X11(n) (((n)&0x1FF)<<7)
#define RF_REGISTERS_GPIO_AUXCLK_EN_NODELAY (1<<6)
#define RF_REGISTERS_AUX_BUF_DELAY (1<<5)
#define RF_REGISTERS_GPIO_AUXCLK_EN_DR (1<<4)
#define RF_REGISTERS_GPIO_AUXCLK_EN_REG (1<<3)
#define RF_REGISTERS_THERMO_CAL_ENABLE (1<<2)
#define RF_REGISTERS_POLARITY_I    (1<<1)
#define RF_REGISTERS_POLARITY_Q    (1<<0)

//control_rf_reg2
#define RF_REGISTERS_CNT_MAX_DCCAL_I(n) (((n)&0xFFFF)<<0)

//control_rf_reg3
//#define RF_REGISTERS_RESERVED_0_X1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_CNT_MAX_DCCAL_Q(n) (((n)&0xFF)<<0)

//control_rf_reg6
#define RF_REGISTERS_RESERVED_0_X12(n) (((n)&0x7FFF)<<1)
#define RF_REGISTERS_CHIP_TX_MODE_REG (1<<0)

//clk_gen_reg0
//#define RF_REGISTERS_RESERVED_0_X8 (1<<15)
#define RF_REGISTERS_FREQ_CLK_DIV_4(n) (((n)&7)<<12)
#define RF_REGISTERS_FREQ_CLK_DIV_3(n) (((n)&7)<<9)
#define RF_REGISTERS_FREQ_CLK_DIV_2(n) (((n)&7)<<6)
#define RF_REGISTERS_FREQ_CLK_DIV_1(n) (((n)&7)<<3)
#define RF_REGISTERS_FREQ_CLK_DIV_0(n) (((n)&7)<<0)

//clk_gen_reg1
#define RF_REGISTERS_RESERVED_0_X13(n) (((n)&0x7F)<<9)
#define RF_REGISTERS_FREQ_CLK_DIV_7(n) (((n)&7)<<6)
#define RF_REGISTERS_FREQ_CLK_DIV_6(n) (((n)&7)<<3)
#define RF_REGISTERS_FREQ_CLK_DIV_5(n) (((n)&7)<<0)

//clk_gen_reg2
#define RF_REGISTERS_INV_CLK_DIV(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_ENABLE_CLK_DIV(n) (((n)&0xFF)<<0)

//chip_id_reg0
#define RF_REGISTERS_CHIP_ID0(n)   (((n)&0xFFFF)<<0)

//chip_id_reg1
//#define RF_REGISTERS_RESERVED_0_X1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_CHIP_ID1(n)   (((n)&0xFF)<<0)

//chip_id_reg2
#define RF_REGISTERS_REVISION_ID(n) (((n)&0xFFFF)<<0)

//mean_dcccal_i_reg0
#define RF_REGISTERS_MEAN_DCCAL_I0(n) (((n)&0xFFFF)<<0)

//mean_dcccal_i_reg1
//#define RF_REGISTERS_RESERVED_0_X12(n) (((n)&0x7FFF)<<1)
#define RF_REGISTERS_MEAN_DCCAL_I1 (1<<0)

//mean_dcccal_q_reg
//#define RF_REGISTERS_RESERVED_0_X13(n) (((n)&0x7F)<<9)
#define RF_REGISTERS_MEAN_DCCAL_Q(n) (((n)&0x1FF)<<0)

//revid_reg
//#define RF_REGISTERS_RESERVED_0_X1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_REVID_TX(n)   (((n)&15)<<4)
#define RF_REGISTERS_REVID_RX(n)   (((n)&15)<<0)

//ana_rd_reg0
#define RF_REGISTERS_REG_221_BIT(n) (((n)&0xFFFF)<<0)

//ana_rd_reg1
#define RF_REGISTERS_REG_222_BIT(n) (((n)&0xFFFF)<<0)

//ana_rd_reg2
#define RF_REGISTERS_REG_223_BIT(n) (((n)&0xFFFF)<<0)

//ana_rd_reg3
#define RF_REGISTERS_REG_224_BIT(n) (((n)&0xFFFF)<<0)

//pa_ramp_reg0
#define RF_REGISTERS_RAMP_CURV0_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV0_P0(n) (((n)&0xFF)<<0)

//pa_ramp_reg1
#define RF_REGISTERS_RAMP_CURV0_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV0_P2(n) (((n)&0xFF)<<0)

//pa_ramp_reg2
#define RF_REGISTERS_RAMP_CURV0_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV0_P4(n) (((n)&0xFF)<<0)

//pa_ramp_reg3
#define RF_REGISTERS_RAMP_CURV0_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV0_P6(n) (((n)&0xFF)<<0)

//pa_ramp_reg4
#define RF_REGISTERS_RAMP_CURV0_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV0_P8(n) (((n)&0xFF)<<0)

//pa_ramp_reg5
#define RF_REGISTERS_RAMP_CURV0_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV0_PA(n) (((n)&0xFF)<<0)

//pa_ramp_reg6
#define RF_REGISTERS_RAMP_CURV0_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV0_PC(n) (((n)&0xFF)<<0)

//pa_ramp_reg7
#define RF_REGISTERS_RAMP_CURV0_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV0_PE(n) (((n)&0xFF)<<0)

//pa_ramp_reg10
#define RF_REGISTERS_RAMP_CURV1_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV1_P0(n) (((n)&0xFF)<<0)

//pa_ramp_reg11
#define RF_REGISTERS_RAMP_CURV1_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV1_P2(n) (((n)&0xFF)<<0)

//pa_ramp_reg12
#define RF_REGISTERS_RAMP_CURV1_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV1_P4(n) (((n)&0xFF)<<0)

//pa_ramp_reg13
#define RF_REGISTERS_RAMP_CURV1_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV1_P6(n) (((n)&0xFF)<<0)

//pa_ramp_reg14
#define RF_REGISTERS_RAMP_CURV1_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV1_P8(n) (((n)&0xFF)<<0)

//pa_ramp_reg15
#define RF_REGISTERS_RAMP_CURV1_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV1_PA(n) (((n)&0xFF)<<0)

//pa_ramp_reg16
#define RF_REGISTERS_RAMP_CURV1_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV1_PC(n) (((n)&0xFF)<<0)

//pa_ramp_reg17
#define RF_REGISTERS_RAMP_CURV1_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV1_PE(n) (((n)&0xFF)<<0)

//pa_ramp_reg20
#define RF_REGISTERS_RAMP_CURV2_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV2_P0(n) (((n)&0xFF)<<0)

//pa_ramp_reg21
#define RF_REGISTERS_RAMP_CURV2_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV2_P2(n) (((n)&0xFF)<<0)

//pa_ramp_reg22
#define RF_REGISTERS_RAMP_CURV2_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV2_P4(n) (((n)&0xFF)<<0)

//pa_ramp_reg23
#define RF_REGISTERS_RAMP_CURV2_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV2_P6(n) (((n)&0xFF)<<0)

//pa_ramp_reg24
#define RF_REGISTERS_RAMP_CURV2_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV2_P8(n) (((n)&0xFF)<<0)

//pa_ramp_reg25
#define RF_REGISTERS_RAMP_CURV2_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV2_PA(n) (((n)&0xFF)<<0)

//pa_ramp_reg26
#define RF_REGISTERS_RAMP_CURV2_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV2_PC(n) (((n)&0xFF)<<0)

//pa_ramp_reg27
#define RF_REGISTERS_RAMP_CURV2_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV2_PE(n) (((n)&0xFF)<<0)

//pa_ramp_reg30
#define RF_REGISTERS_RAMP_CURV3_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV3_P0(n) (((n)&0xFF)<<0)

//pa_ramp_reg31
#define RF_REGISTERS_RAMP_CURV3_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV3_P2(n) (((n)&0xFF)<<0)

//pa_ramp_reg32
#define RF_REGISTERS_RAMP_CURV3_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV3_P4(n) (((n)&0xFF)<<0)

//pa_ramp_reg33
#define RF_REGISTERS_RAMP_CURV3_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV3_P6(n) (((n)&0xFF)<<0)

//pa_ramp_reg34
#define RF_REGISTERS_RAMP_CURV3_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV3_P8(n) (((n)&0xFF)<<0)

//pa_ramp_reg35
#define RF_REGISTERS_RAMP_CURV3_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV3_PA(n) (((n)&0xFF)<<0)

//pa_ramp_reg36
#define RF_REGISTERS_RAMP_CURV3_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV3_PC(n) (((n)&0xFF)<<0)

//pa_ramp_reg37
#define RF_REGISTERS_RAMP_CURV3_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV3_PE(n) (((n)&0xFF)<<0)

//pa_ramp_reg40
#define RF_REGISTERS_RAMP_CURV4_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV4_P0(n) (((n)&0xFF)<<0)

//pa_ramp_reg41
#define RF_REGISTERS_RAMP_CURV4_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV4_P2(n) (((n)&0xFF)<<0)

//pa_ramp_reg42
#define RF_REGISTERS_RAMP_CURV4_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV4_P4(n) (((n)&0xFF)<<0)

//pa_ramp_reg43
#define RF_REGISTERS_RAMP_CURV4_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV4_P6(n) (((n)&0xFF)<<0)

//pa_ramp_reg44
#define RF_REGISTERS_RAMP_CURV4_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV4_P8(n) (((n)&0xFF)<<0)

//pa_ramp_reg45
#define RF_REGISTERS_RAMP_CURV4_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV4_PA(n) (((n)&0xFF)<<0)

//pa_ramp_reg46
#define RF_REGISTERS_RAMP_CURV4_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV4_PC(n) (((n)&0xFF)<<0)

//pa_ramp_reg47
#define RF_REGISTERS_RAMP_CURV4_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV4_PE(n) (((n)&0xFF)<<0)

//pa_ramp_reg50
#define RF_REGISTERS_RAMP_CURV5_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV5_P0(n) (((n)&0xFF)<<0)

//pa_ramp_reg51
#define RF_REGISTERS_RAMP_CURV5_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV5_P2(n) (((n)&0xFF)<<0)

//pa_ramp_reg52
#define RF_REGISTERS_RAMP_CURV5_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV5_P4(n) (((n)&0xFF)<<0)

//pa_ramp_reg53
#define RF_REGISTERS_RAMP_CURV5_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV5_P6(n) (((n)&0xFF)<<0)

//pa_ramp_reg54
#define RF_REGISTERS_RAMP_CURV5_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV5_P8(n) (((n)&0xFF)<<0)

//pa_ramp_reg55
#define RF_REGISTERS_RAMP_CURV5_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV5_PA(n) (((n)&0xFF)<<0)

//pa_ramp_reg56
#define RF_REGISTERS_RAMP_CURV5_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV5_PC(n) (((n)&0xFF)<<0)

//pa_ramp_reg57
#define RF_REGISTERS_RAMP_CURV5_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV5_PE(n) (((n)&0xFF)<<0)

//pa_ramp_reg60
#define RF_REGISTERS_RAMP_CURV6_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV6_P0(n) (((n)&0xFF)<<0)

//pa_ramp_reg61
#define RF_REGISTERS_RAMP_CURV6_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV6_P2(n) (((n)&0xFF)<<0)

//pa_ramp_reg62
#define RF_REGISTERS_RAMP_CURV6_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV6_P4(n) (((n)&0xFF)<<0)

//pa_ramp_reg63
#define RF_REGISTERS_RAMP_CURV6_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV6_P6(n) (((n)&0xFF)<<0)

//pa_ramp_reg64
#define RF_REGISTERS_RAMP_CURV6_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV6_P8(n) (((n)&0xFF)<<0)

//pa_ramp_reg65
#define RF_REGISTERS_RAMP_CURV6_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV6_PA(n) (((n)&0xFF)<<0)

//pa_ramp_reg66
#define RF_REGISTERS_RAMP_CURV6_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV6_PC(n) (((n)&0xFF)<<0)

//pa_ramp_reg67
#define RF_REGISTERS_RAMP_CURV6_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV6_PE(n) (((n)&0xFF)<<0)

//pa_ramp_reg70
#define RF_REGISTERS_RAMP_CURV7_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV7_P0(n) (((n)&0xFF)<<0)

//pa_ramp_reg71
#define RF_REGISTERS_RAMP_CURV7_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV7_P2(n) (((n)&0xFF)<<0)

//pa_ramp_reg72
#define RF_REGISTERS_RAMP_CURV7_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV7_P4(n) (((n)&0xFF)<<0)

//pa_ramp_reg73
#define RF_REGISTERS_RAMP_CURV7_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV7_P6(n) (((n)&0xFF)<<0)

//pa_ramp_reg74
#define RF_REGISTERS_RAMP_CURV7_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV7_P8(n) (((n)&0xFF)<<0)

//pa_ramp_reg75
#define RF_REGISTERS_RAMP_CURV7_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV7_PA(n) (((n)&0xFF)<<0)

//pa_ramp_reg76
#define RF_REGISTERS_RAMP_CURV7_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV7_PC(n) (((n)&0xFF)<<0)

//pa_ramp_reg77
#define RF_REGISTERS_RAMP_CURV7_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV7_PE(n) (((n)&0xFF)<<0)

//pa_ramp_reg80
#define RF_REGISTERS_RAMP_CURV8_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV8_P0(n) (((n)&0xFF)<<0)

//pa_ramp_reg81
#define RF_REGISTERS_RAMP_CURV8_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV8_P2(n) (((n)&0xFF)<<0)

//pa_ramp_reg82
#define RF_REGISTERS_RAMP_CURV8_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV8_P4(n) (((n)&0xFF)<<0)

//pa_ramp_reg83
#define RF_REGISTERS_RAMP_CURV8_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV8_P6(n) (((n)&0xFF)<<0)

//pa_ramp_reg84
#define RF_REGISTERS_RAMP_CURV8_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV8_P8(n) (((n)&0xFF)<<0)

//pa_ramp_reg85
#define RF_REGISTERS_RAMP_CURV8_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV8_PA(n) (((n)&0xFF)<<0)

//pa_ramp_reg86
#define RF_REGISTERS_RAMP_CURV8_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV8_PC(n) (((n)&0xFF)<<0)

//pa_ramp_reg87
#define RF_REGISTERS_RAMP_CURV8_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV8_PE(n) (((n)&0xFF)<<0)

//pa_ramp_reg90
#define RF_REGISTERS_RAMP_CURV9_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV9_P0(n) (((n)&0xFF)<<0)

//pa_ramp_reg91
#define RF_REGISTERS_RAMP_CURV9_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV9_P2(n) (((n)&0xFF)<<0)

//pa_ramp_reg92
#define RF_REGISTERS_RAMP_CURV9_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV9_P4(n) (((n)&0xFF)<<0)

//pa_ramp_reg93
#define RF_REGISTERS_RAMP_CURV9_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV9_P6(n) (((n)&0xFF)<<0)

//pa_ramp_reg94
#define RF_REGISTERS_RAMP_CURV9_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV9_P8(n) (((n)&0xFF)<<0)

//pa_ramp_reg95
#define RF_REGISTERS_RAMP_CURV9_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV9_PA(n) (((n)&0xFF)<<0)

//pa_ramp_reg96
#define RF_REGISTERS_RAMP_CURV9_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV9_PC(n) (((n)&0xFF)<<0)

//pa_ramp_reg97
#define RF_REGISTERS_RAMP_CURV9_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV9_PE(n) (((n)&0xFF)<<0)

//pa_ramp_rega0
#define RF_REGISTERS_RAMP_CURVA_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVA_P0(n) (((n)&0xFF)<<0)

//pa_ramp_rega1
#define RF_REGISTERS_RAMP_CURVA_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVA_P2(n) (((n)&0xFF)<<0)

//pa_ramp_rega2
#define RF_REGISTERS_RAMP_CURVA_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVA_P4(n) (((n)&0xFF)<<0)

//pa_ramp_rega3
#define RF_REGISTERS_RAMP_CURVA_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVA_P6(n) (((n)&0xFF)<<0)

//pa_ramp_rega4
#define RF_REGISTERS_RAMP_CURVA_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVA_P8(n) (((n)&0xFF)<<0)

//pa_ramp_rega5
#define RF_REGISTERS_RAMP_CURVA_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVA_PA(n) (((n)&0xFF)<<0)

//pa_ramp_rega6
#define RF_REGISTERS_RAMP_CURVA_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVA_PC(n) (((n)&0xFF)<<0)

//pa_ramp_rega7
#define RF_REGISTERS_RAMP_CURVA_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVA_PE(n) (((n)&0xFF)<<0)

//pa_ramp_regb0
#define RF_REGISTERS_RAMP_CURVB_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVB_P0(n) (((n)&0xFF)<<0)

//pa_ramp_regb1
#define RF_REGISTERS_RAMP_CURVB_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVB_P2(n) (((n)&0xFF)<<0)

//pa_ramp_regb2
#define RF_REGISTERS_RAMP_CURVB_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVB_P4(n) (((n)&0xFF)<<0)

//pa_ramp_regb3
#define RF_REGISTERS_RAMP_CURVB_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVB_P6(n) (((n)&0xFF)<<0)

//pa_ramp_regb4
#define RF_REGISTERS_RAMP_CURVB_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVB_P8(n) (((n)&0xFF)<<0)

//pa_ramp_regb5
#define RF_REGISTERS_RAMP_CURVB_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVB_PA(n) (((n)&0xFF)<<0)

//pa_ramp_regb6
#define RF_REGISTERS_RAMP_CURVB_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVB_PC(n) (((n)&0xFF)<<0)

//pa_ramp_regb7
#define RF_REGISTERS_RAMP_CURVB_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVB_PE(n) (((n)&0xFF)<<0)

//pa_ramp_regc0
#define RF_REGISTERS_RAMP_CURVC_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVC_P0(n) (((n)&0xFF)<<0)

//pa_ramp_regc1
#define RF_REGISTERS_RAMP_CURVC_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVC_P2(n) (((n)&0xFF)<<0)

//pa_ramp_regc2
#define RF_REGISTERS_RAMP_CURVC_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVC_P4(n) (((n)&0xFF)<<0)

//pa_ramp_regc3
#define RF_REGISTERS_RAMP_CURVC_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVC_P6(n) (((n)&0xFF)<<0)

//pa_ramp_regc4
#define RF_REGISTERS_RAMP_CURVC_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVC_P8(n) (((n)&0xFF)<<0)

//pa_ramp_regc5
#define RF_REGISTERS_RAMP_CURVC_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVC_PA(n) (((n)&0xFF)<<0)

//pa_ramp_regc6
#define RF_REGISTERS_RAMP_CURVC_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVC_PC(n) (((n)&0xFF)<<0)

//pa_ramp_regc7
#define RF_REGISTERS_RAMP_CURVC_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVC_PE(n) (((n)&0xFF)<<0)

//pa_ramp_regd0
#define RF_REGISTERS_RAMP_CURVD_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVD_P0(n) (((n)&0xFF)<<0)

//pa_ramp_regd1
#define RF_REGISTERS_RAMP_CURVD_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVD_P2(n) (((n)&0xFF)<<0)

//pa_ramp_regd2
#define RF_REGISTERS_RAMP_CURVD_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVD_P4(n) (((n)&0xFF)<<0)

//pa_ramp_regd3
#define RF_REGISTERS_RAMP_CURVD_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVD_P6(n) (((n)&0xFF)<<0)

//pa_ramp_regd4
#define RF_REGISTERS_RAMP_CURVD_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVD_P8(n) (((n)&0xFF)<<0)

//pa_ramp_regd5
#define RF_REGISTERS_RAMP_CURVD_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVD_PA(n) (((n)&0xFF)<<0)

//pa_ramp_regd6
#define RF_REGISTERS_RAMP_CURVD_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVD_PC(n) (((n)&0xFF)<<0)

//pa_ramp_regd7
#define RF_REGISTERS_RAMP_CURVD_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVD_PE(n) (((n)&0xFF)<<0)

//pa_ramp_rege0
#define RF_REGISTERS_RAMP_CURVE_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVE_P0(n) (((n)&0xFF)<<0)

//pa_ramp_rege1
#define RF_REGISTERS_RAMP_CURVE_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVE_P2(n) (((n)&0xFF)<<0)

//pa_ramp_rege2
#define RF_REGISTERS_RAMP_CURVE_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVE_P4(n) (((n)&0xFF)<<0)

//pa_ramp_rege3
#define RF_REGISTERS_RAMP_CURVE_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVE_P6(n) (((n)&0xFF)<<0)

//pa_ramp_rege4
#define RF_REGISTERS_RAMP_CURVE_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVE_P8(n) (((n)&0xFF)<<0)

//pa_ramp_rege5
#define RF_REGISTERS_RAMP_CURVE_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVE_PA(n) (((n)&0xFF)<<0)

//pa_ramp_rege6
#define RF_REGISTERS_RAMP_CURVE_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVE_PC(n) (((n)&0xFF)<<0)

//pa_ramp_rege7
#define RF_REGISTERS_RAMP_CURVE_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVE_PE(n) (((n)&0xFF)<<0)

//pa_ramp_regf0
#define RF_REGISTERS_RAMP_CURVF_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVF_P0(n) (((n)&0xFF)<<0)

//pa_ramp_regf1
#define RF_REGISTERS_RAMP_CURVF_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVF_P2(n) (((n)&0xFF)<<0)

//pa_ramp_regf2
#define RF_REGISTERS_RAMP_CURVF_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVF_P4(n) (((n)&0xFF)<<0)

//pa_ramp_regf3
#define RF_REGISTERS_RAMP_CURVF_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVF_P6(n) (((n)&0xFF)<<0)

//pa_ramp_regf4
#define RF_REGISTERS_RAMP_CURVF_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVF_P8(n) (((n)&0xFF)<<0)

//pa_ramp_regf5
#define RF_REGISTERS_RAMP_CURVF_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVF_PA(n) (((n)&0xFF)<<0)

//pa_ramp_regf6
#define RF_REGISTERS_RAMP_CURVF_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVF_PC(n) (((n)&0xFF)<<0)

//pa_ramp_regf7
#define RF_REGISTERS_RAMP_CURVF_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURVF_PE(n) (((n)&0xFF)<<0)

//pa_ramp_reg100
#define RF_REGISTERS_RAMP_CURV10_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV10_P0(n) (((n)&0xFF)<<0)

//pa_ramp_reg101
#define RF_REGISTERS_RAMP_CURV10_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV10_P2(n) (((n)&0xFF)<<0)

//pa_ramp_reg102
#define RF_REGISTERS_RAMP_CURV10_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV10_P4(n) (((n)&0xFF)<<0)

//pa_ramp_reg103
#define RF_REGISTERS_RAMP_CURV10_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV10_P6(n) (((n)&0xFF)<<0)

//pa_ramp_reg104
#define RF_REGISTERS_RAMP_CURV10_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV10_P8(n) (((n)&0xFF)<<0)

//pa_ramp_reg105
#define RF_REGISTERS_RAMP_CURV10_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV10_PA(n) (((n)&0xFF)<<0)

//pa_ramp_reg106
#define RF_REGISTERS_RAMP_CURV10_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV10_PC(n) (((n)&0xFF)<<0)

//pa_ramp_reg107
#define RF_REGISTERS_RAMP_CURV10_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV10_PE(n) (((n)&0xFF)<<0)

//pa_ramp_reg110
#define RF_REGISTERS_RAMP_CURV11_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV11_P0(n) (((n)&0xFF)<<0)

//pa_ramp_reg111
#define RF_REGISTERS_RAMP_CURV11_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV11_P2(n) (((n)&0xFF)<<0)

//pa_ramp_reg112
#define RF_REGISTERS_RAMP_CURV11_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV11_P4(n) (((n)&0xFF)<<0)

//pa_ramp_reg113
#define RF_REGISTERS_RAMP_CURV11_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV11_P6(n) (((n)&0xFF)<<0)

//pa_ramp_reg114
#define RF_REGISTERS_RAMP_CURV11_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV11_P8(n) (((n)&0xFF)<<0)

//pa_ramp_reg115
#define RF_REGISTERS_RAMP_CURV11_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV11_PA(n) (((n)&0xFF)<<0)

//pa_ramp_reg116
#define RF_REGISTERS_RAMP_CURV11_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV11_PC(n) (((n)&0xFF)<<0)

//pa_ramp_reg117
#define RF_REGISTERS_RAMP_CURV11_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV11_PE(n) (((n)&0xFF)<<0)

//pa_ramp_reg120
#define RF_REGISTERS_RAMP_CURV12_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV12_P0(n) (((n)&0xFF)<<0)

//pa_ramp_reg121
#define RF_REGISTERS_RAMP_CURV12_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV12_P2(n) (((n)&0xFF)<<0)

//pa_ramp_reg122
#define RF_REGISTERS_RAMP_CURV12_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV12_P4(n) (((n)&0xFF)<<0)

//pa_ramp_reg123
#define RF_REGISTERS_RAMP_CURV12_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV12_P6(n) (((n)&0xFF)<<0)

//pa_ramp_reg124
#define RF_REGISTERS_RAMP_CURV12_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV12_P8(n) (((n)&0xFF)<<0)

//pa_ramp_reg125
#define RF_REGISTERS_RAMP_CURV12_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV12_PA(n) (((n)&0xFF)<<0)

//pa_ramp_reg126
#define RF_REGISTERS_RAMP_CURV12_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV12_PC(n) (((n)&0xFF)<<0)

//pa_ramp_reg127
#define RF_REGISTERS_RAMP_CURV12_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV12_PE(n) (((n)&0xFF)<<0)

//pa_ramp_reg130
#define RF_REGISTERS_RAMP_CURV13_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV13_P0(n) (((n)&0xFF)<<0)

//pa_ramp_reg131
#define RF_REGISTERS_RAMP_CURV13_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV13_P2(n) (((n)&0xFF)<<0)

//pa_ramp_reg132
#define RF_REGISTERS_RAMP_CURV13_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV13_P4(n) (((n)&0xFF)<<0)

//pa_ramp_reg133
#define RF_REGISTERS_RAMP_CURV13_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV13_P6(n) (((n)&0xFF)<<0)

//pa_ramp_reg134
#define RF_REGISTERS_RAMP_CURV13_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV13_P8(n) (((n)&0xFF)<<0)

//pa_ramp_reg135
#define RF_REGISTERS_RAMP_CURV13_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV13_PA(n) (((n)&0xFF)<<0)

//pa_ramp_reg136
#define RF_REGISTERS_RAMP_CURV13_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV13_PC(n) (((n)&0xFF)<<0)

//pa_ramp_reg137
#define RF_REGISTERS_RAMP_CURV13_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV13_PE(n) (((n)&0xFF)<<0)

//pa_ramp_reg140
#define RF_REGISTERS_RAMP_CURV14_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV14_P0(n) (((n)&0xFF)<<0)

//pa_ramp_reg141
#define RF_REGISTERS_RAMP_CURV14_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV14_P2(n) (((n)&0xFF)<<0)

//pa_ramp_reg142
#define RF_REGISTERS_RAMP_CURV14_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV14_P4(n) (((n)&0xFF)<<0)

//pa_ramp_reg143
#define RF_REGISTERS_RAMP_CURV14_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV14_P6(n) (((n)&0xFF)<<0)

//pa_ramp_reg144
#define RF_REGISTERS_RAMP_CURV14_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV14_P8(n) (((n)&0xFF)<<0)

//pa_ramp_reg145
#define RF_REGISTERS_RAMP_CURV14_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV14_PA(n) (((n)&0xFF)<<0)

//pa_ramp_reg146
#define RF_REGISTERS_RAMP_CURV14_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV14_PC(n) (((n)&0xFF)<<0)

//pa_ramp_reg147
#define RF_REGISTERS_RAMP_CURV14_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV14_PE(n) (((n)&0xFF)<<0)

//pa_ramp_reg150
#define RF_REGISTERS_RAMP_CURV15_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV15_P0(n) (((n)&0xFF)<<0)

//pa_ramp_reg151
#define RF_REGISTERS_RAMP_CURV15_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV15_P2(n) (((n)&0xFF)<<0)

//pa_ramp_reg152
#define RF_REGISTERS_RAMP_CURV15_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV15_P4(n) (((n)&0xFF)<<0)

//pa_ramp_reg153
#define RF_REGISTERS_RAMP_CURV15_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV15_P6(n) (((n)&0xFF)<<0)

//pa_ramp_reg154
#define RF_REGISTERS_RAMP_CURV15_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV15_P8(n) (((n)&0xFF)<<0)

//pa_ramp_reg155
#define RF_REGISTERS_RAMP_CURV15_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV15_PA(n) (((n)&0xFF)<<0)

//pa_ramp_reg156
#define RF_REGISTERS_RAMP_CURV15_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV15_PC(n) (((n)&0xFF)<<0)

//pa_ramp_reg157
#define RF_REGISTERS_RAMP_CURV15_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV15_PE(n) (((n)&0xFF)<<0)

//pa_ramp_reg160
#define RF_REGISTERS_RAMP_CURV16_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV16_P0(n) (((n)&0xFF)<<0)

//pa_ramp_reg161
#define RF_REGISTERS_RAMP_CURV16_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV16_P2(n) (((n)&0xFF)<<0)

//pa_ramp_reg162
#define RF_REGISTERS_RAMP_CURV16_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV16_P4(n) (((n)&0xFF)<<0)

//pa_ramp_reg163
#define RF_REGISTERS_RAMP_CURV16_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV16_P6(n) (((n)&0xFF)<<0)

//pa_ramp_reg164
#define RF_REGISTERS_RAMP_CURV16_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV16_P8(n) (((n)&0xFF)<<0)

//pa_ramp_reg165
#define RF_REGISTERS_RAMP_CURV16_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV16_PA(n) (((n)&0xFF)<<0)

//pa_ramp_reg166
#define RF_REGISTERS_RAMP_CURV16_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV16_PC(n) (((n)&0xFF)<<0)

//pa_ramp_reg167
#define RF_REGISTERS_RAMP_CURV16_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV16_PE(n) (((n)&0xFF)<<0)

//pa_ramp_reg170
#define RF_REGISTERS_RAMP_CURV17_P1(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV17_P0(n) (((n)&0xFF)<<0)

//pa_ramp_reg171
#define RF_REGISTERS_RAMP_CURV17_P3(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV17_P2(n) (((n)&0xFF)<<0)

//pa_ramp_reg172
#define RF_REGISTERS_RAMP_CURV17_P5(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV17_P4(n) (((n)&0xFF)<<0)

//pa_ramp_reg173
#define RF_REGISTERS_RAMP_CURV17_P7(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV17_P6(n) (((n)&0xFF)<<0)

//pa_ramp_reg174
#define RF_REGISTERS_RAMP_CURV17_P9(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV17_P8(n) (((n)&0xFF)<<0)

//pa_ramp_reg175
#define RF_REGISTERS_RAMP_CURV17_PB(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV17_PA(n) (((n)&0xFF)<<0)

//pa_ramp_reg176
#define RF_REGISTERS_RAMP_CURV17_PD(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV17_PC(n) (((n)&0xFF)<<0)

//pa_ramp_reg177
#define RF_REGISTERS_RAMP_CURV17_PF(n) (((n)&0xFF)<<8)
#define RF_REGISTERS_RAMP_CURV17_PE(n) (((n)&0xFF)<<0)

//pa_on_h_reg
#define RF_REGISTERS_RESERVED_0_X14(n) (((n)&15)<<12)
#define RF_REGISTERS_PA_ON_H_DR_REG (1<<11)
#define RF_REGISTERS_PA_ON_H_DR_CTRL (1<<10)
#define RF_REGISTERS_RAMP_DAC_TH(n) (((n)&0x3FF)<<0)

//sys_ctrl_reg_20
#define RF_REGISTERS_SYS_CTRL2_0(n) (((n)&0xFFFF)<<0)

//sys_ctrl_reg_22
#define RF_REGISTERS_SYS_CTRL2_2(n) (((n)&0xFFFF)<<0)

//sys_ctrl_reg_24
#define RF_REGISTERS_SYS_CTRL2_4(n) (((n)&0xFFFF)<<0)

//sys_ctrl_reg_26
#define RF_REGISTERS_SYS_CTRL2_6(n) (((n)&0xFFFF)<<0)

//sys_ctrl_reg_28
#define RF_REGISTERS_SYS_CTRL2_8(n) (((n)&0xFFFF)<<0)

//sys_ctrl_reg_2a
#define RF_REGISTERS_SYS_CTRL2_A(n) (((n)&0xFFFF)<<0)

//sys_ctrl_reg_2c
#define RF_REGISTERS_SYS_CTRL2_C(n) (((n)&0xFFFF)<<0)

//sys_ctrl_reg_2e
#define RF_REGISTERS_SYS_CTRL2_E(n) (((n)&0xFFFF)<<0)

//sys_ctrl_reg_210
#define RF_REGISTERS_SYS_CTRL2_10(n) (((n)&0xFFFF)<<0)

//sys_ctrl_reg_212
#define RF_REGISTERS_SYS_CTRL2_12(n) (((n)&0xFFFF)<<0)

//sys_ctrl_reg_214
#define RF_REGISTERS_SYS_CTRL2_14(n) (((n)&0xFFFF)<<0)

//sys_ctrl_reg_216
#define RF_REGISTERS_SYS_CTRL2_16(n) (((n)&0xFFFF)<<0)

//sys_ctrl_reg_218
#define RF_REGISTERS_SYS_CTRL2_18(n) (((n)&0xFFFF)<<0)

//sys_ctrl_reg_21a
#define RF_REGISTERS_SYS_CTRL2_1A(n) (((n)&0xFFFF)<<0)

//sys_ctrl_reg_21c
#define RF_REGISTERS_SYS_CTRL2_1C(n) (((n)&0xFFFF)<<0)

//sys_ctrl_reg_21e
#define RF_REGISTERS_SYS_CTRL2_1E(n) (((n)&0xFFFF)<<0)

//dlpf_ctrl_reg
//#define RF_REGISTERS_RESERVED_0_X10(n) (((n)&0x1FFF)<<3)
#define RF_REGISTERS_DLPF_VCO_BAND_BIT_SEL (1<<2)
#define RF_REGISTERS_DLPF_IIR3_RSTN (1<<1)
#define RF_REGISTERS_DLPF_RSTN     (1<<0)





#endif

