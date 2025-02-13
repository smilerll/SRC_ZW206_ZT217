/**************************************************************************/ 
/*                                                                        */ 
/*            Copyright (c) 1996-2001 by Express Logic Inc.               */ 
/*                                                                        */ 
/*  This software is copyrighted by and is the sole property of Express   */ 
/*  Logic, Inc.  All rights, title, ownership, or other interests         */ 
/*  in the software remain the property of Express Logic, Inc.  This      */ 
/*  software may only be used in accordance with the corresponding        */ 
/*  license agreement.  Any unauthorized use, duplication, transmission,  */ 
/*  distribution, or disclosure of this software is expressly forbidden.  */ 
/*                                                                        */
/*  This Copyright notice may not be removed or modified without prior    */ 
/*  written consent of Express Logic, Inc.                                */ 
/*                                                                        */ 
/*  Express Logic, Inc. reserves the right to modify this software        */ 
/*  without notice.                                                       */ 
/*                                                                        */ 
/*  Express Logic, Inc.                                                   */
/*  11440 West Bernardo Court               info@expresslogic.com         */
/*  Suite 366                               http://www.expresslogic.com   */
/*  San Diego, CA  92127                                                  */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */ 
/** ThreadX Component                                                     */
/**                                                                       */
/**   Port Specific                                                       */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/ 
/*                                                                        */ 
/*  PORT SPECIFIC C INFORMATION                            RELEASE        */ 
/*                                                                        */ 
/*    tx_port.h                                            ARM7/ARM       */ 
/*                                                           4.0          */ 
/*                                                                        */
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    William E. Lamie, Express Logic, Inc.                               */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This file contains data type definitions that make the ThreadX      */ 
/*    real-time kernel function identically on a variety of different     */ 
/*    processor architectures.  For example, the size or number of bits   */ 
/*    in an "int" data type vary between microprocessor architetures and  */ 
/*    even C compilers for the same microprocessor.  ThreadX does not     */ 
/*    directly use native C data types.  Instead, ThreadX creates its     */ 
/*    own special types that can be mapped to actual data types by this   */ 
/*    file to gaurantee consistency in the interface and functionality.   */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  06-15-1997     William E. Lamie         Initial ARM7/ARM Support      */ 
/*                                            Version 3.0                 */ 
/*  07-04-1997     William E. Lamie         Incorporate new generic code  */ 
/*                                            release, resulting in       */ 
/*                                            version 3.0a.               */  
/*  11-11-1997     William E. Lamie         Incorporated new generic code */ 
/*                                            and modified comment(s),    */ 
/*                                            resulting in version 3.0b.  */ 
/*  12-25-1997     William E. Lamie         Incorporated new generic code */ 
/*                                            and modified comment(s),    */ 
/*                                            resulting in version 3.0c.  */ 
/*  03-01-1998     William E. Lamie         Incorporated new generic code */ 
/*                                            and modified comment(s),    */ 
/*                                            resulting in version 3.0d.  */ 
/*  01-01-1999     William E. Lamie         Changed version string, added */ 
/*                                            in-line assembly support,   */  
/*                                            and modified comments,      */ 
/*                                            resulting in version 3.0e.  */ 
/*  03-01-1999     William E. Lamie         Changed version string and    */ 
/*                                            changed in-line assembly,   */  
/*                                            resulting in version 3.0f.  */ 
/*  03-01-2000     William E. Lamie         Incorporated new generic code */ 
/*                                            and modified comment(s),    */ 
/*                                            resulting in version 3.0g.  */ 
/*  01-28-2001     William E. Lamie         Incorporated new generic code */ 
/*                                            and modified comment(s),    */ 
/*                                            resulting in version 4.0.   */ 
/*                                                                        */ 
/**************************************************************************/ 

#ifndef TX_PORT
#define TX_PORT


/* Define various constants for the port.  */ 

#define TX_MINIMUM_STACK         200         /* Minimum stack size       */
#define TX_INT_DISABLE           0x80        /* Disable IRQ interrupts   */
#define TX_INT_ENABLE            0x00        /* Enable IRQ interrupts    */


/* Define ThreadX specific types for the port.  */ 

#define VOID                        void
typedef void *                      VOID_PTR;
typedef char                        CHAR;
typedef char *                      CHAR_PTR;
typedef unsigned char               UCHAR;
typedef unsigned char *             UCHAR_PTR;
typedef int                         INT;
typedef int *                       INT_PTR;
typedef unsigned int                UINT;
typedef unsigned int *              UINT_PTR;
typedef long                        LONG;
typedef long *                      LONG_PTR;
typedef unsigned long               ULONG;
typedef unsigned long *             ULONG_PTR;


/* Define register constants for the port.  These definitions are 
   prioritized in the order they are defined.  In other words, REG_1
   is assigned to the most used variable, while REG_4 is assigned to
   the least used variable.  */

#define REG_1                       register
#define REG_2                       register
#define REG_3                       register
#define REG_4                       register
#define REG_5                       register


/* Define the port extension field of the thread control block.  Nothing 
   additional is needed for this port so it is defined as white space.  */

#define TX_THREAD_PORT_EXTENSION    


/* Define ThreadX interrupt lockout and restore macros for protection on 
   access of critical kernel information.  The restore interrupt macro must 
   restore the interrupt posture of the running thread prior to the value 
   present prior to the disable macro.  In most cases, the save area macro
   is used to define a local function save area for the disable and restore
   macros.  */

#ifndef TX_DISABLE_INLINE
/* Richard modified. 06/02/2002*/
/*
#define TX_INTERRUPT_SAVE_AREA      register unsigned int interrupt_save, temp;

#define TX_DISABLE                  __asm \
                                    { \
                                        MRS  interrupt_save, CPSR; \
                                        ORR  temp, interrupt_save, 0x80; \
                                        MSR  CPSR_c, temp \
                                    }

#define TX_RESTORE                  __asm \
                                    { \
                                        MSR CPSR_c, interrupt_save \
                                    }
*/
#define TX_INTERRUPT_SAVE_AREA      

#define TX_INTERRUPT_SAVE_AREA      

#define TX_DISABLE                  _tx_thread_interrupt_control(TX_INT_DISABLE);
#define TX_RESTORE                  _tx_thread_interrupt_control(TX_INT_ENABLE);

#else

unsigned int  _tx_thread_interrupt_control(unsigned int);
/* Richard modified. 06/02/2002*/
/*
#define TX_INTERRUPT_SAVE_AREA      unsigned int interrupt_save;

#define TX_DISABLE                  interrupt_save =  _tx_thread_interrupt_control(TX_INT_DISABLE);
#define TX_RESTORE                  _tx_thread_interrupt_control(interrupt_save);
  */
#define TX_INTERRUPT_SAVE_AREA      

#define TX_DISABLE                  _tx_thread_interrupt_control(TX_INT_DISABLE);
#define TX_RESTORE                  _tx_thread_interrupt_control(TX_INT_ENABLE);

#endif

/* Define the version ID of ThreadX.  This may be utilized by the application.  */

#ifdef  TX_THREAD_INIT
CHAR                            _tx_version_id[] = 
                                    "Copyright (c) 1996-2001 Express Logic Inc. * ThreadX ARM7/ARM Version G4.0.4.0 *";
#else
extern  CHAR                    _tx_version_id[];
#endif


#define  TX_THREAD_ZONE_NUM     8
#endif

