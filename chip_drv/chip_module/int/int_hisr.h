/******************************************************************************
 ** File Name:    int_hisr.h                                             *
 ** Author:       Steve.Zhan                                                  *
 ** DATE:         06/29/2010                                                  *
 ** Copyright:    2010 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE                   NAME               DESCRIPTION                     *
 ** 06/29/2010         Steve.Zhan       Create.                               *
 ******************************************************************************/
#ifndef __INT_HISR_H__
#define __INT_HISR_H__

#include "os_api.h"
#include "isr_drvapi.h"

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

/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

typedef enum
{
    CHIPDRV_HISR_START_ID = 0x50,
    CHIPDRV_HISR_0,
    CHIPDRV_HISR_1,
    CHIPDRV_HISR_2,
    CHIPDRV_HISR_3,


    CHIPDRV_HISR_LAST_ID

} CHIPDRV_HISR_ID_E;


typedef struct Hisr_T
{
    hisr_func_t            *hisr_func;
    void                   *data;
    CHIPDRV_HISR_PRI_E     pri;

    struct Hisr_T *volatile  next_hisr;
    volatile uint32          hisr_count;
} Hisr_T;

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/

/*****************************************************************************/
// Description :    This function is used to create HISR
// Global resource dependence :
// Author : steve.zhan
// Note :
/*****************************************************************************/
PUBLIC void CHIPDRVHISR_Create (const uint8 *HisrName, CHIPDRV_HISR_ID_E ThreadId, void (*entry) (uint32, void *),
                                CHIPDRV_HISR_PRI_E Priority, int queueNum);


/*****************************************************************************/
// Description :    This function is used in task to get semaphore.
// Global resource dependence :
// Author : steve.zhan
// Note :
/*****************************************************************************/
PUBLIC  uint32  CHIPDRVHISR_WaitActive (CHIPDRV_HISR_ID_E ThreadId);


/*****************************************************************************/
// Description :    This function is used to activate the HISR to process some
//                  Delay isr things.
// Global resource dependence :
// Author : steve.zhan
// Note :
/*****************************************************************************/
PUBLIC void CHIPDRVHISR_Activate (CHIPDRV_HISR_ID_E ThreadId);



/*****************************************************************************/
// Description :    This function Init Hisr data struct.
// Global resource dependence :
// Author : steve.zhan
// Note :
/*****************************************************************************/
PUBLIC void Hisr_init (void);


/*****************************************************************************/
// Description :    Invoked by ISR.
// Global resource dependence :
// Author : steve.zhan
// Note :
/*****************************************************************************/
PUBLIC void Hisr_put (int Intpri);



/*****************************************************************************/
// Description :    Invokedy by interrupt module, register Hisr callback
//                  function and the priority to HISR.
// Global resource dependence :
// Author : steve.zhan
// Note :
/*****************************************************************************/
PUBLIC uint32 Hisr_RegHandler (
    uint32 priIndex,
    hisr_func_t *pHisrFunc,
    CHIPDRV_HISR_PRI_E hisrPri,
    void             *pHisrData
);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End

