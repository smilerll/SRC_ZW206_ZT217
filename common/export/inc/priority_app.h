#ifndef __PRIORITY_APP_H__
#define __PRIORITY_APP_H__

#include "os_api.h"
#include "priority_system.h"

DECLARE_PRIORITY(PRI_MTV_FAKE_TASK)
#define PRI_MTV_FAKE_TASK        SCI_TASK_PRI(PRI_MTV_FAKE_TASK)

DECLARE_PRIORITY(PRI_MTV_NORMAL_TASK)
#define PRI_MTV_NORMAL_TASK      SCI_TASK_PRI(PRI_MTV_NORMAL_TASK)


DECLARE_PRIORITY(PRI_MTV_REALTIME_TASK)
#define PRI_MTV_REALTIME_TASK    SCI_TASK_PRI(PRI_MTV_REALTIME_TASK)

DECLARE_PRIORITY(PRI_MMITTS_TASK)
#define PRI_MMITTS_TASK         SCI_TASK_PRI(PRI_MMITTS_TASK)


DECLARE_PRIORITY(PRI_EBOOK_TASK)
#define PRI_EBOOK_TASK           SCI_TASK_PRI(PRI_EBOOK_TASK)

DECLARE_PRIORITY(PRI_EBOOK_SEGTAB_TASK)
#define PRI_EBOOK_SEGTAB_TASK    SCI_TASK_PRI(PRI_EBOOK_SEGTAB_TASK)

DECLARE_PRIORITY(PRI_MTHREAD_KERNEL)
#define PRI_MTHREAD_KERNEL         SCI_TASK_PRI(PRI_MTHREAD_KERNEL)

DECLARE_PRIORITY(PRI_MTHREAD_CRITICAL)
#define PRI_MTHREAD_CRITICAL       SCI_TASK_PRI(PRI_MTHREAD_CRITICAL)

DECLARE_PRIORITY(PRI_MTHREAD_HIGHEST)
#define PRI_MTHREAD_HIGHEST        SCI_TASK_PRI(PRI_MTHREAD_HIGHEST)

DECLARE_PRIORITY(PRI_MTHREAD_ABOVE_NORMAL)
#define PRI_MTHREAD_ABOVE_NORMAL   SCI_TASK_PRI(PRI_MTHREAD_ABOVE_NORMAL)

DECLARE_PRIORITY(PRI_MTHREAD_NORMAL)
#define PRI_MTHREAD_NORMAL         SCI_TASK_PRI(PRI_MTHREAD_NORMAL)

DECLARE_PRIORITY(PRI_MTHREAD_BELOW_NORMAL)
#define PRI_MTHREAD_BELOW_NORMAL   SCI_TASK_PRI(PRI_MTHREAD_BELOW_NORMAL)

DECLARE_PRIORITY(PRI_MTHREAD_LOWEST)
#define PRI_MTHREAD_LOWEST         SCI_TASK_PRI(PRI_MTHREAD_LOWEST)

DECLARE_PRIORITY(PRI_MTHREAD_IDLE)
#define PRI_MTHREAD_IDLE           SCI_TASK_PRI(PRI_MTHREAD_IDLE)

DECLARE_PRIORITY(PRI_HQR_TASK)
#define PRI_HQR_TASK            SCI_TASK_PRI(PRI_HQR_TASK)


DECLARE_PRIORITY(PRI_AJAVAKVE_TASK)
#define PRI_AJAVAKVE_TASK      SCI_TASK_PRI(PRI_AJAVAKVE_TASK)


DECLARE_PRIORITY(PRI_AJAVACOMM_TASK)
#define PRI_AJAVACOMM_TASK    SCI_TASK_PRI(PRI_AJAVACOMM_TASK)

DECLARE_PRIORITY(PRI_JVM_TASK)
#define PRI_JVM_TASK        SCI_TASK_PRI(PRI_JVM_TASK)


DECLARE_PRIORITY(PRI_JAVA_SIGDISPATCH)
#define PRI_JAVA_SIGDISPATCH     SCI_TASK_PRI(PRI_JAVA_SIGDISPATCH)



DECLARE_PRIORITY(PRI_MMIJAVA_TASK_HIGH)
#define PRI_MMIJAVA_TASK_HIGH      SCI_TASK_PRI(PRI_MMIJAVA_TASK_HIGH)



DECLARE_PRIORITY(PRI_MMIJAVA_TASK_LOW)
#define PRI_MMIJAVA_TASK_LOW      SCI_TASK_PRI(PRI_MMIJAVA_TASK_LOW)


DECLARE_PRIORITY(PRI_APP_FILE_TASK)
#define PRI_APP_FILE_TASK    SCI_TASK_PRI(PRI_APP_FILE_TASK)


DECLARE_PRIORITY(PRI_APP_IKB_TASK)
#define PRI_APP_IKB_TASK    SCI_TASK_PRI(PRI_APP_IKB_TASK)


DECLARE_PRIORITY(PRI_MAIL_MAIN_TASK)
#define PRI_MAIL_MAIN_TASK    SCI_TASK_PRI(PRI_MAIL_MAIN_TASK)


DECLARE_PRIORITY(PRI_MAIL_PARSE_TASK_PRIO)
#define PRI_MAIL_PARSE_TASK_PRIO    SCI_TASK_PRI(PRI_MAIL_PARSE_TASK_PRIO)


DECLARE_PRIORITY(PRI_MV_TASK)
#define PRI_MV_TASK          SCI_TASK_PRI(PRI_MV_TASK)


DECLARE_PRIORITY(PRI_MV_SOCKET_TASK)
#define PRI_MV_SOCKET_TASK    SCI_TASK_PRI(PRI_MV_SOCKET_TASK)


DECLARE_PRIORITY(PRI_ASYN_TASK_PRIORITY)
#define PRI_ASYN_TASK_PRIORITY        SCI_TASK_PRI(PRI_ASYN_TASK_PRIORITY)

DECLARE_PRIORITY(PRI_USER_THREAD_TASK_PRIORITY)
#define PRI_USER_THREAD_TASK_PRIORITY  SCI_TASK_PRI(PRI_USER_THREAD_TASK_PRIORITY)


DECLARE_PRIORITY(PRI_FTCP_THROUGHUP_TASK)
#define PRI_FTCP_THROUGHUP_TASK    SCI_TASK_PRI(PRI_FTCP_THROUGHUP_TASK)


DECLARE_PRIORITY(PRI_HTTP_TASK)
#define PRI_HTTP_TASK              SCI_TASK_PRI(PRI_HTTP_TASK)


DECLARE_PRIORITY(PRI_INNO_LPM_TASK)
#define PRI_INNO_LPM_TASK          SCI_TASK_PRI(PRI_INNO_LPM_TASK)


DECLARE_PRIORITY(PRI_INNO_TASK)
#define PRI_INNO_TASK              SCI_TASK_PRI(PRI_INNO_TASK)


DECLARE_PRIORITY(PRI_JAVA_SIGDISPATCH_TASK)
#define PRI_JAVA_SIGDISPATCH_TASK  SCI_TASK_PRI(PRI_JAVA_SIGDISPATCH_TASK)


DECLARE_PRIORITY(PRI_JBED_TASK)
#define PRI_JBED_TASK              SCI_TASK_PRI(PRI_JBED_TASK)

DECLARE_PRIORITY(PRI_EMAIL_APP_TASK)
#define  PRI_EMAIL_APP_TASK        SCI_TASK_PRI(PRI_EMAIL_APP_TASK)

DECLARE_PRIORITY(PRI_WRE_KERNEL_TASK) 
#define PRI_WRE_KERNEL_TASK          SCI_TASK_PRI(PRI_WRE_KERNEL_TASK)

DECLARE_PRIORITY(PRI_WRE_CRITICAL_TASK)
#define PRI_WRE_CRITICAL_TASK        SCI_TASK_PRI(PRI_WRE_CRITICAL_TASK)

DECLARE_PRIORITY(PRI_WRE_HIGHEST_TASK)
#define PRI_WRE_HIGHEST_TASK         SCI_TASK_PRI(PRI_WRE_HIGHEST_TASK)

DECLARE_PRIORITY(PRI_WRE_ABOVE_NORMAL_TASK)
#define PRI_WRE_ABOVE_NORMAL_TASK    SCI_TASK_PRI(PRI_WRE_ABOVE_NORMAL_TASK)

DECLARE_PRIORITY(PRI_WRE_NORMAL_TASK)
#define PRI_WRE_NORMAL_TASK          SCI_TASK_PRI(PRI_WRE_NORMAL_TASK)

DECLARE_PRIORITY(PRI_WRE_BELOW_NORMAL_TASK)
#define PRI_WRE_BELOW_NORMAL_TASK    SCI_TASK_PRI(PRI_WRE_BELOW_NORMAL_TASK)

DECLARE_PRIORITY(PRI_WRE_LOWEST_TASK)
#define PRI_WRE_LOWEST_TASK          SCI_TASK_PRI(PRI_WRE_LOWEST_TASK)

DECLARE_PRIORITY(PRI_WRE_IDLE_TASK)
#define PRI_WRE_IDLE_TASK            SCI_TASK_PRI(PRI_WRE_IDLE_TASK)

DECLARE_PRIORITY(PRI_MODEM_THROUGHUP_TASK)
#define PRI_MODEM_THROUGHUP_TASK     SCI_TASK_PRI(PRI_MODEM_THROUGHUP_TASK)

DECLARE_PRIORITY(PRI_MTEKVIDEOENC_TASK)
#define PRI_MTEKVIDEOENC_TASK        SCI_TASK_PRI(PRI_MTEKVIDEOENC_TASK)
                                   
DECLARE_PRIORITY(PRI_GPS_OS_TASK)                                      
#define  PRI_GPS_OS_TASK              SCI_TASK_PRI(PRI_GPS_OS_TASK)

DECLARE_PRIORITY(PRI_OCR_TASK)
#define  PRI_OCR_TASK                 SCI_TASK_PRI(PRI_OCR_TASK)

DECLARE_PRIORITY(PRI_CACL_TASK)                                      
#define  PRI_CACL_TASK                SCI_TASK_PRI(PRI_CACL_TASK)

DECLARE_PRIORITY(PRI_MWINSOCK_TASK)
#define  PRI_MWINSOCK_TASK            SCI_TASK_PRI(PRI_MWINSOCK_TASK)

DECLARE_PRIORITY(PRI_EXT_VT_MW_TASK)
#define  PRI_EXT_VT_MW_TASK          SCI_TASK_PRI(PRI_EXT_VT_MW_TASK)

DECLARE_PRIORITY(PRI_APP_PB_TASK)
#define  PRI_APP_PB_TASK          SCI_TASK_PRI(PRI_APP_PB_TASK)

DECLARE_PRIORITY(PRI_CAMERA_ROLL_TASK)
#define  PRI_CAMERA_ROLL_TASK          SCI_TASK_PRI(PRI_CAMERA_ROLL_TASK)

DECLARE_PRIORITY(PRI_GALLERY_FILEARRAY_TASK)
#define  PRI_GALLERY_FILEARRAY_TASK          SCI_TASK_PRI(PRI_GALLERY_FILEARRAY_TASK)

DECLARE_PRIORITY(PRI_BAIDU_DRV_TASK)
#define  PRI_BAIDU_DRV_TASK          SCI_TASK_PRI(PRI_BAIDU_DRV_TASK)

#endif /*__PRIORITY_APP_H__*/
