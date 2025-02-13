/******************************************************************************
 ** File Name:      fm_kt0812g.h                                              *
 ** Author:         Sunny.Fei                                                 *
 ** DATE:           2010.12.06                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2010.12.06   Sunny.Fei             Create.                               *
 ******************************************************************************/
// *****************************************************************************/
#ifndef _FM_KT0812G_H
#define _FM_KT0812G_H
#ifndef _FM_KT0812G_H_2010_12_06_10_44_0_533_
#define _FM_KT0812G_H_2010_12_06_10_44_0_533_

#define FMDRV_DEBUG
#ifdef FMDRV_DEBUG
#define FMDRV_PRINT	SCI_TRACE_LOW
#else
#define FMDRV_PRINT	
#endif
#define FMDRV_ASSERT       SCI_ASSERT 
/**---------------------------------------------------------------------------*
 **                             Dependencies                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" 
{
#endif
/**---------------------------------------------------------------------------*
 **                           Macros & Enums                                  *
 **---------------------------------------------------------------------------*/
#define KT0812G_RET_OK              0x00
#define KT0812G_RET_FAIL            0xff

#define KT0812G_FREQ_LOW_LIMIT      875
#define KT0812G_FREQ_HIGH_LIMIT     1080

// Register Setting*/
// Seek Configuration(Reg 0x2)*/
#define KT0812G_SEEK_ENABLE         (0x1<<15 )// 0:disable, 1:enable*/
#define KT0812G_SEEK_DIR            (0x1<<14 )// 0:seek down; 1:seek up*/
#define KT0812G_SEEK_THRESHOLD      (0x1f<<7 )// ????*/
#define KT0812G_USA_BAND            (0x0<<4  )// 87~108MHz*/
#define KT0812G_JAPAN_WIDEBAND      (0x1<<4  )// 76~108MHz*/
#define KT0812G_JAPAN_BAND          (0x2<<4  )// 76~90MHz*/
#define KT0812G_CHAN_US_SPACE       (0x0<<2  )// 200KHz US,Europe*/
#define KT0812G_CHAN_JAPAN_SPACE    (0x1<<2  )// 100KHz Europe, Japan*/
#define KT0812G_CHAN_VALID_SPACE    (0x2<<2  )// 50KHz*/

// TUNE Register(Reg 0x03)*/
#define KT0812G_TUNE_ENABLE         (0x1<<15) // 0:disable, 1:enable*/
#define KT0812G_AUTOTUNE            (0x1<<12) /* 0:Will NOT tune after a successful seek until a separated Tune 
                                                command is received from the control interface
                                              1:Automatically starts Tune process after Seek*/
#define KT0812G_TUNE_CHAN           (0x3ff) // Tune Channel Value*/

// VOLUME Control Register(Reg 0x04)*/
#define KT0812G_SMUTE_B             (0x1<<15) // 0:Softmute enable, 1:Softmute disable*/
#define KT0812G_MUTE_B              (0x1<<14) // 0:Hard mute enable, 1:disable*/
#define KT0812G_SMUTER_SLOWEST      (0x0<<12) // Softmute Attack Recover Rate 0:Slowest*/

#define KT0812G_SMUTER_FASTEST      (0x1<<12) // 1:fastest(RSSI mode only)*/
#define KT0812G_SMUTER_FAST         (0x2<<12) // 2:fast*/
#define KT0812G_SMUTER_SLOW         (0x3<<12) // 3:slow*/
#define KT0812G_BASSBOOST_MODE      (0x3<<8 ) // 00:disable, 01:low, 10:med, 11:high*/
#define KT0812G_SMTH                (0x3<<4 ) // SMTH[1:0]00:Lowest, 11:Highest, SMTH[2] in reg0x21*/
#define KT0812G_VOLUME              (0xf)  // vol mask
#define KT0812G_VOLUME_LEVEL0       (0x0 ) // MUTE*/
#define KT0812G_VOLUME_LEVEL1       (0x2 ) // -39dBFS*/
#define KT0812G_VOLUME_LEVEL2       (0x4 ) // -33dBFS*/
#define KT0812G_VOLUME_LEVEL3       (0x6 ) // -27dBFS*/
#define KT0812G_VOLUME_LEVEL4       (0x8 ) // -21dBFS*/
#define KT0812G_VOLUME_LEVEL5       (0xa ) // -15dBFS*/
#define KT0812G_VOLUME_LEVEL6       (0xc ) // -9dBFS*/
#define KT0812G_VOLUME_LEVEL7       (0xd ) 
#define KT0812G_VOLUME_LEVEL8       (0xe ) // -3dBFS*/
#define KT0812G_VOLUME_LEVEL9       (0xf ) // FS*/

// DSP Configuration Register (Reg 0x05)*/
#define KT0812G_DSP_MONO            (0x1<<15) // 0:Stereo, 1:Force mono*/
#define KT0812G_DSP_DE              (0x1<<11) // De-emphasis 0:75us, USA; 1:50us, Europe, Australia, Japan*/

#define KT0812G_DSP_BLNDADJ_HIGH    (0x00<<8) // NOTE: Write 00 explicity even if 00 is the default value, Stereo/Mono Blend Start Level*/
#define KT0812G_DSP_BLNDADJ_HIGHEST (0x01<<8) // NOTE: Write 00 explicity even if 00 is the default value*/
#define KT0812G_DSP_BLNDADJ_LOWEST  (0x02<<8) // NOTE: Write 00 explicity even if 00 is the default value*/
#define KT0812G_DSP_BLNDADJ_LOW     (0x03<<8) // NOTE: Write 00 explicity even if 00 is the default value*/

#define KT0812G_DSP_DBLND_DISABLE   (0x1<<5) // Blend disable*/
#define KT0812G_DSP_DBLND_ENABLE    (0x0<<5) // Blend enable*/

// RF Configuration Register (Reg 0x09)*/
#define KT0812G_RF_ANTTYP_EP        (0x1<<8 )// earphone/short antenna*/
#define KT0812G_RF_ANTTYP_LONG      (0x0<<8 )// long antenna*/
                                    
// LO Synthesizer Configuraton (Reg 0x0A)*/
#define KT0812G_LO_AFCRANGE_0       (0x0<<12) // AFC correction range 12KHz*/
#define KT0812G_LO_AFCRANGE_1       (0x1<<12) // AFC correction range 15KHz*/
#define KT0812G_LO_AFCRANGE_2       (0x2<<12) // AFC correction range 14KHz*/
#define KT0812G_LO_AFCRANGE_3       (0x3<<12) // AFC correction range 17KHz*/
#define KT0812G_LO_AFCRANGE_4       (0x4<<12) // AFC correction range 16KHz*/
#define KT0812G_LO_AFCRANGE_5       (0x5<<12) // AFC correction range 19KHz*/
#define KT0812G_LO_AFCRANGE_6       (0x6<<12) // AFC correction range 22KHz*/
#define KT0812G_LO_AFCRANGE_7       (0x7<<12) // AFC correction range 25KHz*/

#define KT0812G_LO_AFCD_DISABLE     (0x1<<8)  // AFC disable control bit*/
#define KT0812G_LO_AFCD_ENABLE      (0x0<<8)  // AFC enable control bit*/
#define KT0812G_LO_HLSI_HIGH        (0x1<<6)  // High side injection*/
#define KT0812G_LO_HLSI_LOW         (0x0<<6)  // Low side injection*/

// System Configuration Register(Reg 0x0F)*/
#define KT0812G_INTLVL_HIGH         (0x1<<15) // Interrupt level control(GPIO2):HIGH Level*/
#define KT0812G_INTLVL_LOW          (0x0<<15) // LOW Level interrupt*/

#define KT0812G_SOFT_RESET          (0x1<<14) // 1:soft reset; 0: normal operation*/
#define KT0812G_SEEKTUNE_COMP_EN    (0x1<<13) // 1:Enable Interrupt*/
#define KT0812G_SEEKTUNE_COM_DIS    (0x0<<13) // 0:Disable Interrupt*/
#define KT0812G_STANDBY_EN          (0x1<<12) // 1:Enable*/
#define KT0812G_STANDBY_DIS         (0x0<<12) // 0:Disable*/

#define KT0812G_SEEK_MOSELECTION    (0x1<<10) // 1:stop at the band edge; 0:cycling seek*/

#define KT0812G_SEEK_SEL            (0x1<<6)  // 1:Advanced method; 0:Traditional method*/


// Status Register A(Reg 0x12)*/
#define KT0812G_XTAL_OK             (0x1<<15) // 1: crystal is OK
#define KT0812G_SEEKTUNE_COMP       (0x1<<14) // 0: not complete; 1:complete. The status can be cleared manually*/
#define KT0812G_SF_BL_FAIL          (0x1<<13) // 0:Seek successful; 1:Seek failure*/
#define KT0812G_PLL_LOCK            (0x1<<11) // 1: PLL Ready

// Status Register A(Reg 0x13)*/
#define KT0812G_READ_CHAN           (0x3ff) //  The tuned current chan
// SNR Register*/
#define KT0812G_SNRTH               (0xff<<8) // SNR threshold for traditional seek mode*/

// SEEKTH Register*/
#define KT0812G_SEEKTH_HIGH         (0xff<<8) // High threshold for advanced seek mode*/
#define KT0812G_SEEKTH_LOW          (0xff<<8) // Low threshold for advanced seek mode*/

// Softmute Register(Reg 0x21)*/
#define KT0812G_SMMD_RSSI_SNR       (0x1<<6)  // 0:RSSI;1:SNR*/
#define KT0812G_SMTH_2              (0x0<<3)  // Softmute threshold MSB, with SMTH[1:0] in Reg0x04*/

// Clock Register (Reg 0x23)*/
#define KT0812G_RCLK_EN             (0x1<<12 )// 1:reference clock; 0:crystal*/
#define KT0812G_REF_CLK_SEL0        (0x0<<8  )// Reference clock selection 0:32.768KHz*/
#define KT0812G_REF_CLK_SEL1        (0x1<<8  )// 1:6.5MHz*/
#define KT0812G_REF_CLK_SEL2        (0x2<<8  )// 2:7.6MHz*/
#define KT0812G_REF_CLK_SEL3        (0x3<<8  )// 3:12MHz*/
#define KT0812G_REF_CLK_SEL4        (0x4<<8  )// 4:13MHz*/
#define KT0812G_REF_CLK_SEL5        (0x5<<8  )// 5:15.2MHz*/
#define KT0812G_REF_CLK_SEL6        (0x6<<8  )// 6:19.2MHz*/
#define KT0812G_REF_CLK_SEL7        (0x7<<8  )// 7:24MHz*/
#define KT0812G_REF_CLK_SEL8        (0x8<<8  )// 8:26MHz*/
#define KT0812G_LRSWAP              (0x1<<4  )// 1:swap; 0:don't swap. Swap Left and Right audio channels*/
#define KT0812G_ANT_TUNE_EN         (0x1<<2  )/* Antenna tuning enable
                            0 = disable. Adjustable cap before LAN will not connect to RFIN pin.
                            1 = enable. Adjustable cap will connect to RFIN pin.*/
                            

// Register address,because it isn't continue address*/
#define KT0812G_REG_DEVICE          0x0 //address
#define KT0812G_REG_CHIPID          0x1 //address
#define KT0812G_REG_SEEK            0x2
#define KT0812G_REG_TUNE            0x3
#define KT0812G_REG_VOLUME          0x4
#define KT0812G_REG_DSPCFGA         0x5
#define KT0812G_REG_RFCFG           0x9
#define KT0812G_REG_LOCFGA          0xa
#define KT0812G_REG_SYSCFG          0xf
#define KT0812G_REG_STATUSA         0x12
#define KT0812G_REG_STATUSB         0x13
#define KT0812G_REG_STATUSC         0x14
#define KT0812G_REG_STATUSD         0x15
#define KT0812G_REG_ANTENNA         0x1d
#define KT0812G_REG_SNR             0x1f
#define KT0812G_REG_SEEKTH          0x20
#define KT0812G_REG_SOFTMUTE        0x21
#define KT0812G_REG_CLOCK           0x23


// Register default value*/
#define KT0812G_REG_VAL_DEVICE      0xb002 
#define KT0812G_REG_VAL_CHIPID      0x0440
#define KT0812G_REG_VAL_SEEK        0x2303
#define KT0812G_REG_VAL_TUNE        0x19cc
#define KT0812G_REG_VAL_VOLUME      0x8027
#define KT0812G_REG_VAL_DSPCFGA     0x5000
#define KT0812G_REG_VAL_RF          0x0500
#define KT0812G_REG_VAL_LO          0x0100
#define KT0812G_REG_VAL_SYS         0x8a00
#define KT0812G_REG_VAL_STATUSA     0x0c00
#define KT0812G_REG_VAL_STATUSB     0x0
#define KT0812G_REG_VAL_STATUSC     0x0
#define KT0812G_REG_VAL_STATUSD     0x0
#define KT0812G_REG_VAL_ANTENNA     0x0087 //Reserved
#define KT0812G_REG_VAL_SNR         0x0
#define KT0812G_REG_VAL_SEEKTH      0x1419
#define KT0812G_REG_VAL_SOFTMUTE    0x1012
#define KT0812G_REG_VAL_CLOCK       0x0001
/**---------------------------------------------------------------------------*
 **                           Data Structures                                 *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                             Variables                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
PUBLIC extern FM_OPERATION_T g_KT0812G_ops;

#ifdef __cplusplus
}
#endif

/******************************************************************************/
#endif //#ifndef _FM_KT0812G_H_2010_12_06_10_44_0_533_
#endif //#ifndef _FM_KT0812G_H
