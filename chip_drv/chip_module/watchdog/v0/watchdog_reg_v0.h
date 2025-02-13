/******************************************************************************
 ** File Name:    watchdog_reg_v0.h                                            *
 ** Author:       Steve.Zhan                                                 *
 ** DATE:         06/05/2010                                                  *
 ** Copyright:    2010 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/05/2010    Steve.Zhan      Create.                                     *
 ** 08/02/2010    Jie.Dai         Add #ifndef WDG_LOCK                        *
 ******************************************************************************/
#ifndef _WATCHDOG_REG_V0_H_
#define _WATCHDOG_REG_V0_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */


/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/

/*----------Watchdog Timer Counter Register----------*/
//#define WDG_BASE                      0x89003000
#define WDG_LOAD                        (WDG_BASE + 0x0000)
#define WDG_VALUE                       (WDG_BASE + 0x0004)
#define WDG_CTL                         (WDG_BASE + 0x0008)
#define WDG_INT_CLR                     (WDG_BASE + 0x000C)
#define WDG_INT_RAW_STS                 (WDG_BASE + 0x0010)
#define WDG_INT_MASKED_STS              (WDG_BASE + 0x0014)

#ifndef WDG_LOCK
#define WDG_LOCK                        (WDG_BASE + 0x0018)
#endif

#define WDG_CTL_INT_EN                  (uint32)(1 << 0)
#define WDG_CTL_CNT_EN                  (uint32)(1 << 1)

#define WDG_UNLOCK_KEY                  0x1ACCE551
#define WDG_LOCK_KEY                    0x12345678

#define WDG_INT_CLR_BIT                 (uint32)(1 << 0)

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End

