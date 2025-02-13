/******************************************************************************
                                   海信WCDMA  自主业务模块
        由于3G  网络的逐渐成熟，大量的业务需求成为第三代
        移动终端的亮点，逐渐成为不可或缺的一部分。本
        模块设计基于OMA  联盟提出的基于syncml 协议的数据和终
        端管理，实现在多平台、多操作系统中的PIM业务。目标
        是要达到任意平台的可移植性良好的功能稳定的模块。
        本模块的实现分为三个步骤:
        1.实现Data sync功能
        2.实现Device Management功能
        3.实现多平台的平滑移植
        模块开发参考OMA成员共同开发的SRT_c。
        目前模块计划开发支持OMA syncML 协议1.1


        版权声明:本软件版权属于海信通信，保留一切法律权利
        开发人员:刘刚
        开发日期:2005年4月11日。
******************************************************************************/


/******************************************************************************
文件名称:PIMwsm.h
文件描述:wsm头文件
文件说明:syncml workspace manager，用来做功能工作区，主要完成
                       模块功能的操作，内存分配和管理，生成发送、
                       接收数据等等。
开发人员:刘刚
首次开发日期:2005-4-11
******************************************************************************/


/******************************************************************************
                                                            修改记录
  日期                   作者                修改原因
  -----------------------------------------------------------------------------
  2004-4-11              刘刚                第一次开发编码。
  -----------------------------------------------------------------------------
  *****************************************************************************/
/*加入此FEATURE防止头文件重复包含*/
  #ifndef   HEADER_FILE_WSM
  #define  HEADER_FILE_WSM
  
  /*  包含头文件*/

  /*  数据结构声明*/
  
 // use default value of 4 (not much for a multi-connection server)
 
#define HS_MAX_WSM_BUFFERS 2
#define HS_MAX_WSM_BUFFER_SIZE (100*1024) //add by wangxiaolin
#define HS_ONE_WORK_SPACE_SIZE  (30*1024)

  typedef struct WsmOptions_s {
  long maxAvailMem;   // maximum amount of memory available for all wsm buffers
} WsmOptions_t;
  
/** WSM internal buffer structure */
typedef struct WsmBuf_s {
  char*    bufName;     // external name of buffer
  short     memH;        // memory handle
  unsigned char*    pFirstFree;  // pointer to first free element in buffer
  unsigned char*     pFirstData;  // pointer to first data element in buffer
  long   size;        // size of buffer
  long   usedBytes;   // used bytes in buffer
  unsigned char      flags;
} WsmBuf_t;

/* sbuffer list */
typedef struct smWinList_s {
  char               *memName;      // name of buffer
  char               *winH;         // reference to memory block
  short         memH;         // handle of memory block
  unsigned char              locked;       // is handle locked?
  long           memSize;      // size of memory block    
  struct smWinList_s *next;         // next list item
} smWinList_t;

typedef smWinList_t *WsmSmGlobals_t;
  
  /** WSM globals for use with global Anchor */
typedef struct WsmGlobals_s {
  short           wsmRet;          // last WSM return code
  unsigned char          initWasCalled;   // was hs_wsmInit() called?
  WsmBuf_t        wsmBuf[HS_MAX_WSM_BUFFERS];   
  short         wsmIndex;        // Index of actual buffer
  WsmSmGlobals_t  wsmSm;           // WSM_SM global; device dependent!
} *WsmGlobalsPtr_t, WsmGlobals_t;
  
  /*  全局变量声明*/

  /*  功能函数声明*/
  //private functions
  short hs_getNextFreeEntry(void);
  unsigned char hs_isMemAvailable(long memToAlloc);
  short hs_resetBufferGlobals(short memH);
 
  short hs_nameToHandle(char* name);
  unsigned char hs_isValidMemH(short memH);
  unsigned char hs_isLockedMemH(short memH);
  unsigned char hs_locateH(short memH, smWinList_t **p);
  short hs_deleteBufferHandle(short memH);

  unsigned char hs_newListEle(const char *name, smWinList_t **newEle, short *newHandle);
  unsigned char hs_locateEle(const char *eleName, smWinList_t **p);
  void hs_removeEle(const char *eleName);
  
 
  //public functions
  void hs_createDataStructs(void);

  InstanceInfoPtr_t hs_findInfo(short id);

  short hs_addInfo(InstanceInfoPtr_t pInfo);

  short hs_removeInfo(short id);

  short hs_smCreate (char* memName, long memSize, short *memH); 
  short hs_smOpen (char* memName, short *memH);
  short hs_smClose (short memH);
  short hs_smDestroy (char* memName);
  short hs_smSetSize (short memH, long newSize);
  short hs_smLock (short memH, unsigned char **pMem);
  short hs_smUnlock (short memH) ;

  /**
 * FUNCTION: hs_wsmInit
 *
 * Initializes all Workspace Manager related resources.<BR>
 * Should only be called once!
 *
 * PRE-Condition:   This is the first function call to WSM
 *
 * POST-Condition:  All WSM resources are initialized
 *
 * IN:      wsmOpts
 *          WSM options, valid options are:
 *          <UL>
 *          <LI> tbd
 *          </UL>
 * 
 * OUT:     wsmH
 *          Handle to new buffer
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_INVALID_OPTIONS, if wsmOpts is not valid
 *          HS_SML_ERR_NOT_ENOUGH_SPACE, if not enough available memory
 *          HS_SML_ERR_WRONG_USAGE, if hs_wsmInit was already called
 */
short hs_wsmInit (const WsmOptions_t *wsmOpts);


/**
 * FUNCTION: hs_wsmCreate
 *
 * Creates and opens a new buffer with name bufName and size bufSize.<BR> 
 * If a buffer with name bufName already exists, the existing buffer 
 * is resized to bufSize.
 *
 * PRE-Condition:   bufSize > 0
 *
 * POST-Condition:  handle refers to buffer bufName; BufferSize = size
 *
 * IN:      bufName
 *          Name of buffer to be created
 * IN:      bufSize
 *          Size of buffer to be created
 * 
 * OUT:     wsmH
 *          Handle to new buffer
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_INVALID_SIZE, if bufSize <= 0
 *          HS_SML_ERR_NOT_ENOUGH_SPACE, if available memory < bufSize
 *          HS_SML_ERR_WSM_BUF_TABLE_FULL, if buffer table is full
 *          HS_SML_ERR_WRONG_USAGE, if hs_wsmInit wasn't called before
 *
 * @see  hs_wsmDestroy
 */
short hs_wsmCreate (char* bufName, long bufSize, short *wsmH);

/**
 * FUNCTION: hs_wsmClose
 *
 * Close an open buffer.
 *
 * PRE-Condition:   handle is valid; handle is unlocked
 *
 * POST-Condition:  handle is not known to WSM any more
 *
 * IN:      wsmH
 *          Handle to the open buffer
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_INVALID_HANDLE, if handle was invalid
 *          HS_SML_ERR_WRONG_USAGE, if handle was still locked
 *
 * @see  wsmOpen
 */
short hs_wsmClose (short wsmH);

/**
 * FUNCTION: hs_wsmDestroy
 *
 * Destroy existing buffer with name bufName.
 *
 * PRE-Condition:   WSM knows bufName; handle is unlocked
 *
 * POST-Condition:  buffer is not known to WSM any more; all resources 
 *                  connected to this buffer are freed
 *
 * IN:      bufName
 *          Name of buffer to be opened
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_WRONG_PARAM, if bufName is unknown to WSM
 *          HS_SML_ERR_WRONG_USAGE, if handle was still locked
 *
 * @see  hs_wsmCreate
 */
short hs_wsmDestroy (char* bufName);

/**
 * FUNCTION: hs_wsmTerminate
 *
 * Terminate WSM; free all buffers and resources.
 *
 * PRE-Condition: all handles must be unlocked
 *
 * POST-Condition: all resources are freed
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_WRONG_USAGE, if a handle was still locked
 *
 */
short hs_wsmTerminate (void);

/**
 * FUNCTION: hs_wsmLockH
 *
 * Locks handle wsmH and get a pointer to the contents of wsmH. <BR>
 * RequestedPos describes the position in the buffer to which the returned 
 * pointer should point. Valid values are: 
 * <UL>
 *   <LI> SML_FIRST_DATA_ITEM
 *   <LI> SML_FIRST_FREE_ITEM
 * </UL>
 *
 * PRE-Condition:   handle is unlocked; handle is valid
 *
 * POST-Condition:  handle is locked; points to first data item, 
 *                  or first free item.
 *
 * IN:      wsmH
 *          Handle to the open buffer
 * IN:      requestedPos
 *          Requested position of the returned pointer
 *          <UL>
 *            <LI> SML_FIRST_DATA_ITEM : points to first data entry
 *            <LI> SML_FIRST_FREE_ITEM : points to first free entry
 *          </UL>
 * 
 * OUT:     pMem
 *          Pointer to requested memory          
 *          
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_INVALID_HANDLE, if handle was invalid
 *          HS_SML_ERR_WRONG_USAGE, if handle was still locked
 *          HS_SML_ERR_UNSPECIFIC, if requested position is unknown, or lock failed
 *
 * @see  hs_wsmUnlockH
 */
short hs_wsmLockH (short wsmH, SmlBufPtrPos_t requestedPos,
		unsigned char **pMem);
/**
 * FUNCTION: hs_wsmGetFreeSize
 *
 * Returns the remaining unused bytes in the buffer.
 *
 * PRE-Condition:   handle is valid
 *
 * POST-Condition:  hs_wsmGetFreeSize = BufferSize - hs_wsmGetUsedSize
 *
 * IN:      wsmH
 *          Handle to the open buffer
 *
 * OUT:     freeSize
 *          Number of bytes which are unused in this buffer
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_INVALID_HANDLE, if handle was invalid
 *
 * @see  hs_wsmGetUsedSize
 * @see  hs_wsmProcessedBytes
 */
short hs_wsmGetFreeSize(short wsmH, long *freeSize) ;

/**
 * FUNCTION: hs_wsmSetUsedSize
 *
 * Tell Workspace how many data were written into buffer.
 *
 * PRE-Condition:   handle is valid; usedSize <= hs_wsmGetFreeSize; handle is 
 *                  locked
 *
 * POST-Condition:  hs_wsmGetUsedSize += usedSize; hs_wsmGetFreeSize -= usedSize;
 *                  instancePtr += usedSize;
 *
 * IN:      wsmH
 *          Handle to the open buffer
 * IN:      usedSize
 *          Number of bytes which were written into buffer
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_INVALID_HANDLE, if handle was invalid
 *          HS_SML_ERR_INVALID_SIZE, if usedSize <= hs_wsmGetFreeSize
 *
 * @see  hs_wsmGetUsedSize
 */
short hs_wsmSetUsedSize (short wsmH, long usedSize);

/**
 * FUNCTION: hs_wsmGetUsedSize
 *
 * Returns the number of bytes used in the buffer.
 *
 * PRE-Condition:   handle is valid
 *
 * POST-Condition:  usedSize = BufferSize - hs_wsmGetFreeSize
 *
 * IN:      wsmH
 *          Handle to the open buffer
 *
 * OUT:     usedSize
 *          Number of bytes which are already used in this buffer
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_INVALID_HANDLE, if handle was invalid
 *
 * @see  hs_wsmGetFreeSize
 * @see  hs_wsmSetUsedSize
 */
short hs_wsmGetUsedSize(short wsmH, long *usedSize);

/**
 * FUNCTION: hs_wsmProcessedBytes
 *
 * Tell Workspace Manager the number of bytes already processed.
 *
 * PRE-Condition:   handle is locked; handle is valid;
 *                  noBytes <= hs_wsmGetUsedSize
 *
 * POST-Condition:  noBytes starting at wsmGetPtr() position are deleted; 
 *                  remaining bytes are copied to 
 *                  wsmGetPtr(SML_FIRST_FREE_ITEM) position;
 *                  hs_wsmGetUsedSize -= noBytes; hs_wsmGetFreeSize += noBytes
 *
 * IN:      wsmH
 *          Handle to the open buffer
 * IN:      noBytes
 *          Number of bytes already processed from buffer.
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_INVALID_HANDLE, if handle was invalid
 *          HS_SML_ERR_WRONG_USAGE, if handle was not locked
 *          HS_SML_ERR_INVALID_SIZE, if noBytes > hs_wsmGetUsedSize
 *
 * @see  hs_wsmGetFreeSize
 */
short hs_wsmProcessedBytes (short wsmH, long noBytes);

/**
 * FUNCTION: hs_wsmUnlockH
 *
 * Unlock handle wsmH. <BR>
 * After this call all pointers to this memory handle are invalid 
 * and should no longer be used.
 *
 * PRE-Condition:   handle is locked; handle is valid
 *
 * POST-Condition:  handle is unlocked
 *
 * OUT:     wsmH
 *          Handle to unlock
 *
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *          HS_SML_ERR_INVALID_HANDLE, if handle was invalid
 *          HS_SML_ERR_WRONG_USAGE, if handle was not locked
 *          HS_SML_ERR_UNSPECIFIC, unlock failed
 *
 * @see  hs_wsmLockH
 */
short hs_wsmUnlockH (short wsmH);

/**
 * FUNCTION: hs_pim_wsmReset
 *
 * Reset the Workspace
 *
 * PRE-Condition:   -
 *
 * POST-Condition:  all data is lost. The FirstFree Position equals 
 * the First Data position
 *
 * IN:      wsmH
 *          Handle to the open buffer
 * 
 * RETURN:  HS_SML_ERR_OK, if O.K.
 *
 */
short hs_pim_wsmReset (short wsmH);

#endif