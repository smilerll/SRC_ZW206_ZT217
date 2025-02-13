//##!(important notice: do not remove this line)
/******************************************************************************
 ** Copyright (c) 
 ** File Name:		sensor_OV7670.c 										  *
 ** Author: 													  *
 ** DATE:															  *
 ** Description:   This file contains driver for sensor OV7670. 
 ** 														 
 ******************************************************************************

 ******************************************************************************
 ** 					   Edit History 									  *
 ** ------------------------------------------------------------------------- *
 ** DATE		   NAME 			DESCRIPTION 							  *
 ** 	  
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 ** 						Dependencies									  *
 **---------------------------------------------------------------------------*/
#include "sensor_cfg.h"
#include "spload_main.h"
#include "sensor_drv.h"
#include "i2c_api.h"
#include "os_api.h"
#include "chip.h"
#include "dal_dcamera.h"
#include "gpio_prod_api.h"

/**---------------------------------------------------------------------------*
 ** 						Compiler Flag									  *
 **---------------------------------------------------------------------------*/
#ifdef	 __cplusplus
	extern	 "C" 
	{
#endif
/**---------------------------------------------------------------------------*
 ** 					Extern Function Declaration 						  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 ** 						Const variables 								  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 ** 						   Macro Define
 **---------------------------------------------------------------------------*/
#define OV7670_I2C_ADDR_W		0x42//0x60
#define OV7670_I2C_ADDR_R		0x43//0x61

#define SENSOR_GAIN_SCALE		16

#define OV7670_COM11 0x3b
#define PLL_ADDR    0x11


#define AE_ENABLE 0x13

 
/**---------------------------------------------------------------------------*
 ** 					Local Function Prototypes							  *
 **---------------------------------------------------------------------------*/
LOCAL uint32 set_ov7670_ae_awb_enable(uint32 ae_enable, uint32 awb_enable);
LOCAL uint32 set_ov7670_ae_enable(uint32 enable);
LOCAL uint32 set_hmirror_enable(uint32 enable);
LOCAL uint32 set_vmirror_enable(uint32 enable);
LOCAL uint32 set_preview_mode(uint32 preview_mode);
LOCAL uint32 OV7670_Identify(uint32 param);
LOCAL uint32 OV7670_BeforeSnapshot(uint32 param);
LOCAL uint32 OV7670_After_Snapshot(uint32 param);

LOCAL uint32 set_brightness(uint32 level);
LOCAL uint32 set_contrast(uint32 level);
LOCAL uint32 set_sharpness(uint32 level);
LOCAL uint32 set_saturation(uint32 level);
LOCAL uint32 set_image_effect(uint32 effect_type);

LOCAL uint32 read_ev_value(uint32 value);
LOCAL uint32 write_ev_value(uint32 exposure_value);
LOCAL uint32 read_gain_value(uint32 value);
LOCAL uint32 write_gain_value(uint32 gain_value);
LOCAL uint32 read_gain_scale(uint32 value);
LOCAL uint32 set_frame_rate(uint32 param);
LOCAL uint32 OV7670_set_work_mode(uint32 mode);

LOCAL uint32 set_ov7670_ev(uint32 level);
LOCAL uint32 set_ov7670_awb(uint32 mode);
LOCAL uint32 set_ov7670_anti_flicker(uint32 mode);
LOCAL uint32 set_ov7670_video_mode(uint32 mode);
LOCAL uint32 _ov7670_Power_On(uint32 power_on);
LOCAL uint32 _ov7670_GetResolutionTrimTab(uint32 param);
LOCAL uint32 _ov7670_GetExifInfo(uint32 param);
LOCAL uint32 _ov7670_InitExifInfo(void);

/**---------------------------------------------------------------------------*
 ** 						Local Variables 								 *
 **---------------------------------------------------------------------------*/
LOCAL uint32 s_preview_mode;

/*lint -save -e533 */
__align(4) LOCAL const SENSOR_REG_T ov7670_YUV_640X480[]=
{
    {0x12, 0x80},
    {SENSOR_WRITE_DELAY, 0x20},//delay 100ms
    {0x11, 0x80},
    {0x3a, 0x04},
    {0x3d, 0xc3},
    {0x12, 0x00},
    {0x15, 0x00},
    {0x17, 0x13},
    {0x18, 0x01},
    {0x32, 0xbf},
    {0x19, 0x02},
    {0x1a, 0x7a},
    {0x03, 0x0a},
    {0x0c, 0x00},
    {0x3e, 0x00},
    {0x70, 0x3a},
    {0x71, 0x35},
    {0x72, 0x11},
    {0x73, 0xf0},
    {0xa2, 0x02},
    {0x7a, 0x20},
    {0x7b, 0x03},
    {0x7c, 0x0a},
    {0x7d, 0x1a},
    {0x7e, 0x3f},
    {0x7f, 0x4e},
    {0x80, 0x5b},
    {0x81, 0x68},
    {0x82, 0x75},
    {0x83, 0x7f},
    {0x84, 0x89},
    {0x85, 0x9a},
    {0x86, 0xa6},
    {0x87, 0xbd},
    {0x88, 0xd3},
    {0x89, 0xe8},
    {0x13, 0xe0},
    {0x00, 0x00},
    {0x10, 0x00},
    {0x0d, 0x60},
    {0x14, 0x08}, 
    {0xa5, 0x05},
    {0xab, 0x02},
    {0x24, 0x60},
    {0x25, 0x50},
    {0x26, 0x92},
    {0x9f, 0x78},
    {0xa0, 0x68},
    {0xa1, 0x03},
    {0xa6, 0xd8},
    {0xa7, 0xd8},
    {0xa8, 0xf0},
    {0xa9, 0x90},
    {0xaa, 0x14},
    {0x13, 0xe5},
    {0x0e, 0x61},
    {0x0f, 0x4b},
    {0x16, 0x02},
    {0x1e, 0x07},
    {0x21, 0x02},
    {0x22, 0x91},
    {0x29, 0x07},
    {0x33, 0x0b},
    {0x35, 0x0b},
    {0x37, 0x1d},
    {0x38, 0x71},
    {0x39, 0x2a},
    {0x3c, 0x78},
    {0x4d, 0xc0},
    {0x4e, 0x20},
    {0x69, 0x00},
    {0x6b, 0x0a},
    {0x74, 0x10},
    {0x8d, 0x4f},
    {0x8e, 0x00},
    {0x8f, 0x00},
    {0x90, 0x00},
    {0x91, 0x00},
    {0x96, 0x00},
    {0x9a, 0x80},
    {0xb0, 0x84},
    {0xb1, 0x0c},
    {0xb2, 0x0e},
    {0xb3, 0x82},
    {0xb8, 0x0a},
    {0x43, 0x00},
    {0x44, 0xf0},
    {0x45, 0x41},
    {0x46, 0x66},
    {0x47, 0x2a},
    {0x48, 0x3e},
    {0x59, 0x8d},
    {0x5a, 0x8e},
    {0x5b, 0x53},
    {0x5c, 0x83},
    {0x5d, 0x4f},
    {0x5e, 0x0e},
    {0x6c, 0x0a},
    {0x6d, 0x55},
    {0x6e, 0x11},
    {0x6f, 0x9e},
    {0x6a, 0x40},
    {0x61, 0x63},        
    {0x01, 0x56},
    {0x02, 0x44},
    {0x13, 0xe7},
    {0x4f, 0x88},
    {0x50, 0x8b},
    {0x51, 0x04},
    {0x52, 0x11},
    {0x53, 0x8c},
    {0x54, 0x9d},
    {0x55, 0x00},
    {0x56, 0x40},
    {0x57, 0x80},
    {0x58, 0x9a},
    {0x41, 0x08},
    {0x3f, 0x00},
    {0x75, 0x02},
    {0x76, 0x60},
    {0x4c, 0x00},
    {0x77, 0x01},
    {0x3d, 0xc3},
    {0x4b, 0x09},
    {0xc9, 0x30},
    {0x41, 0x38},
    {0x56, 0x40},
    {0x34, 0x11},
    {0x3b, 0x0a},
    {0xa4, 0x88},
    {0x96, 0x00},
    {0x97, 0x30},
    {0x98, 0x20},
    {0x99, 0x30},
    {0x9a, 0x84},
    {0x9b, 0x29},
    {0x9c, 0x03},
    {0x9d, 0x98},
    {0x9e, 0x7f},
    {0x78, 0x04},
    {0x79, 0x01},
    {0xc8, 0xf0},
    {0x79, 0x0f},
    {0xc8, 0x00},
    {0x79, 0x10},
    {0xc8, 0x7e},
    {0x79, 0x0a},
    {0xc8, 0x80},
    {0x79, 0x0b},
    {0xc8, 0x01},
    {0x79, 0x0c},
    {0xc8, 0x0f},
    {0x79, 0x0d},
    {0xc8, 0x20},
    {0x79, 0x09},
    {0xc8, 0x80},
    {0x79, 0x02},
    {0xc8, 0xc0},
    {0x79, 0x03},
    {0xc8, 0x40},
    {0x79, 0x05},
    {0xc8, 0x30},
    {0x79, 0x26},
    {0x62, 0x00},
    {0x63, 0x00},
    {0x64, 0x06},
    {0x65, 0x00},
    {0x66, 0x05},
    {0x94, 0x05},
    {0x95, 0x09},

    {0x2d, 0x00},
    {0x2e, 0x00},
    {0x14, 0x18},
    {0xa5, 0x05},
    //;LENS
    {0x62, 0x00},
    {0x63, 0x00},
    {0x65, 0x00},
    {0x64, 0x15},
    {0x94, 0x15},
    {0x95, 0x18},
    {0x66, 0x05},
    //; CMX
    {0x4f, 0x6e},
    {0x50, 0x69},
    {0x51, 0x05},
    {0x52, 0x1b},
    {0x53, 0x63},
    {0x54, 0x7e},
    {0x58, 0x9e},

    {0x92, 0x0},//0xfc},
    {0x93, 0x0},    

    //brightness
    {0x55, 0x00}
};

__align(4) LOCAL const SENSOR_REG_T ov7670_YUV_MOTION_320X240[]=
{
    {0x12, 0x80},
    {SENSOR_WRITE_DELAY, 0x20},//delay 100ms
    {0x11, 0x80},
    {0x3a, 0x04},
    {0x3d, 0xc3},
    {0x12, 0x00},
    {0x15, 0x00},
    {0x17, 0x13},
    {0x18, 0x01},
    {0x32, 0xbf},
    {0x19, 0x02},
    {0x1a, 0x7a},
    {0x03, 0x0a},
    {0x0c, 0x00},
    {0x3e, 0x00},
    {0x70, 0x3a},
    {0x71, 0x35},
    {0x72, 0x11},
    {0x73, 0xf0},
    {0xa2, 0x02},
    {0x7a, 0x20},
    {0x7b, 0x03},
    {0x7c, 0x0a},
    {0x7d, 0x1a},
    {0x7e, 0x3f},
    {0x7f, 0x4e},
    {0x80, 0x5b},
    {0x81, 0x68},
    {0x82, 0x75},
    {0x83, 0x7f},
    {0x84, 0x89},
    {0x85, 0x9a},
    {0x86, 0xa6},
    {0x87, 0xbd},
    {0x88, 0xd3},
    {0x89, 0xe8},
    {0x13, 0xe0},
    {0x00, 0x00},
    {0x10, 0x00},
    {0x0d, 0x60},
    {0x14, 0x08}, 
    {0xa5, 0x02},
    {0xab, 0x02},
    {0x24, 0x60},
    {0x25, 0x50},
    {0x26, 0xc2},
    {0x9f, 0x78},
    {0xa0, 0x68},
    {0xa1, 0x03},
    {0xa6, 0xd8},
    {0xa7, 0xd8},
    {0xa8, 0xf0},
    {0xa9, 0x90},
    {0xaa, 0x14},
    {0x13, 0xe5},
    {0x0e, 0x61},
    {0x0f, 0x4b},
    {0x16, 0x02},
    {0x1e, 0x07},
    {0x21, 0x02},
    {0x22, 0x91},
    {0x29, 0x07},
    {0x33, 0x0b},
    {0x35, 0x0b},
    {0x37, 0x1d},
    {0x38, 0x71},
    {0x39, 0x2a},
    {0x3c, 0x78},
    {0x4d, 0xc0},
    {0x4e, 0x20},
    {0x69, 0x00},
    {0x6b, 0x0a},
    {0x74, 0x10},
    {0x8d, 0x4f},
    {0x8e, 0x00},
    {0x8f, 0x00},
    {0x90, 0x00},
    {0x91, 0x00},
    {0x96, 0x00},
    {0x9a, 0x80},
    {0xb0, 0x84},
    {0xb1, 0x0c},
    {0xb2, 0x0e},
    {0xb3, 0x7c},
    {0xb8, 0x0a},
    {0x43, 0x04},
    {0x44, 0xf0},
    {0x45, 0x41},
    {0x46, 0x5b},
    {0x47, 0x2d},
    {0x48, 0x46},
    {0x59, 0xab},
    {0x5a, 0xbd},
    {0x5b, 0xb5},
    {0x5c, 0x6c},
    {0x5d, 0x5b},
    {0x5e, 0x0d},
    {0x6c, 0x0a},
    {0x6d, 0x55},
    {0x6e, 0x11},
    {0x6f, 0x9f},
    {0x6a, 0x40},
    {0x01, 0x56},
    {0x02, 0x44},
    {0x13, 0xe7},
    {0x4f, 0x88},
    {0x50, 0x8b},
    {0x51, 0x04},
    {0x52, 0x11},
    {0x53, 0x8c},
    {0x54, 0x9d},
    {0x55, 0x00},
    {0x56, 0x40},
    {0x57, 0x80},
    {0x58, 0x9a},
    {0x41, 0x08},
    {0x3f, 0x00},
    {0x75, 0x02},
    {0x76, 0x60},
    {0x4c, 0x00},
    {0x77, 0x01},
    {0x3d, 0xc3},
    {0x4b, 0x09},
    {0xc9, 0x30},
    {0x41, 0x38},
    {0x56, 0x40},
    {0x34, 0x11},
    {0x3b, 0x0a},
    {0xa4, 0x88},
    {0x96, 0x00},
    {0x97, 0x30},
    {0x98, 0x20},
    {0x99, 0x30},
    {0x9a, 0x84},
    {0x9b, 0x29},
    {0x9c, 0x03},
    {0x9d, 0x98},
    {0x9e, 0x7f},
    {0x78, 0x04},
    {0x79, 0x01},
    {0xc8, 0xf0},
    {0x79, 0x0f},
    {0xc8, 0x00},
    {0x79, 0x10},
    {0xc8, 0x7e},
    {0x79, 0x0a},
    {0xc8, 0x80},
    {0x79, 0x0b},
    {0xc8, 0x01},
    {0x79, 0x0c},
    {0xc8, 0x0f},
    {0x79, 0x0d},
    {0xc8, 0x20},
    {0x79, 0x09},
    {0xc8, 0x80},
    {0x79, 0x02},
    {0xc8, 0xc0},
    {0x79, 0x03},
    {0xc8, 0x40},
    {0x79, 0x05},
    {0xc8, 0x30},
    {0x79, 0x26},
    {0x62, 0x00},
    {0x63, 0x00},
    {0x64, 0x06},
    {0x65, 0x00},
    {0x66, 0x05},
    {0x94, 0x05},
    {0x95, 0x09},

    {0x2d, 0x00},
    {0x2e, 0x00},
    {0x14, 0x18},
    {0xa5, 0x02},
    //;LENS
    {0x62, 0x00},
    {0x63, 0x00},
    {0x65, 0x00},
    {0x64, 0x15},
    {0x94, 0x15},
    {0x95, 0x18},
    {0x66, 0x05},
    //; CMX
    {0x4f, 0x6e},
    {0x50, 0x69},
    {0x51, 0x05},
    {0x52, 0x1b},
    {0x53, 0x63},
    {0x54, 0x7e},
    {0x58, 0x9e},

    {0x55, 0x00}
};



LOCAL const SENSOR_REG_TAB_INFO_T s_OV7670_resolution_Tab_YUV[]=
{	
	// COMMON INIT
	{ADDR_AND_LEN_OF_ARRAY(ov7670_YUV_640X480), 640, 480, 24, SENSOR_IMAGE_FORMAT_YUV422},
	
	// YUV422 PREVIEW 1	
	{PNULL, 0, 640, 480,24, SENSOR_IMAGE_FORMAT_YUV422},
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0},
	
	// YUV422 PREVIEW 2 
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0}
};

LOCAL const SENSOR_TRIM_T s_OV7670_Resolution_Trim_Tab[]=
{	
	// COMMON INIT
	{0, 0, 640, 480, 150, 0},
	
	// YUV422 PREVIEW 1	
	{0, 0, 640, 480, 150, 0},
	{0, 0, 0, 0, 0, 0},
	
	{0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0},
	
	// YUV422 PREVIEW 2 
	{0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0}
};

LOCAL EXIF_SPEC_PIC_TAKING_COND_T s_ov7670_exif={0x00};

LOCAL const SENSOR_IOCTL_FUNC_TAB_T s_OV7670_ioctl_func_tab = 
{
    // Internal 
    PNULL,
    _ov7670_Power_On,
    PNULL,
    OV7670_Identify,

    PNULL,			// write register
    PNULL,			// read  register	
    PNULL,
    _ov7670_GetResolutionTrimTab,

    // External
    set_ov7670_ae_enable,
    set_hmirror_enable,
    set_vmirror_enable,

    set_brightness,
    set_contrast,
    set_sharpness,
    set_saturation,

    set_preview_mode,	
    set_image_effect,

    OV7670_BeforeSnapshot, 
    OV7670_After_Snapshot,

    PNULL,

    read_ev_value,
    write_ev_value,
    read_gain_value,
    write_gain_value,
    read_gain_scale,
    set_frame_rate,	
    PNULL,
    PNULL,
    set_ov7670_awb,
    PNULL,
    PNULL,
    set_ov7670_ev,
    PNULL,
    PNULL,
    PNULL,
    _ov7670_GetExifInfo,
    PNULL,
    set_ov7670_anti_flicker,
    set_ov7670_video_mode,
    PNULL,
    PNULL    
};

/**---------------------------------------------------------------------------*
 ** 						Global Variables								  *
 **---------------------------------------------------------------------------*/
 PUBLIC SENSOR_INFO_T g_OV7670_yuv_info =
{
	OV7670_I2C_ADDR_W,				// salve i2c write address
	OV7670_I2C_ADDR_R, 				// salve i2c read address
	
	0,								// bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
									// bit2: 0: i2c register addr  is 8 bit, 1: i2c register addr  is 16 bit
									// other bit: reseved
	SENSOR_HW_SIGNAL_PCLK_N|\
	SENSOR_HW_SIGNAL_VSYNC_P|\
	SENSOR_HW_SIGNAL_HSYNC_P,		// bit0: 0:negative; 1:positive -> polarily of pixel clock
									// bit2: 0:negative; 1:positive -> polarily of horizontal synchronization signal
									// bit4: 0:negative; 1:positive -> polarily of vertical synchronization signal
									// other bit: reseved											
											
	// environment mode
	SENSOR_ENVIROMENT_NORMAL|\
	SENSOR_ENVIROMENT_NIGHT,		
	
	// image effect
	SENSOR_IMAGE_EFFECT_NORMAL|\
	SENSOR_IMAGE_EFFECT_BLACKWHITE|\
	SENSOR_IMAGE_EFFECT_RED|\
	SENSOR_IMAGE_EFFECT_GREEN|\
	SENSOR_IMAGE_EFFECT_BLUE|\
	SENSOR_IMAGE_EFFECT_YELLOW|\
	SENSOR_IMAGE_EFFECT_NEGATIVE|\
	SENSOR_IMAGE_EFFECT_CANVAS,
	
	// while balance mode
	SENSOR_WB_MODE_AUTO |\
	SENSOR_WB_MODE_INCANDESCENCE |\
	SENSOR_WB_MODE_U30 |\
	SENSOR_WB_MODE_CWF |\
	SENSOR_WB_MODE_FLUORESCENT |\
	SENSOR_WB_MODE_SUN |\
	SENSOR_WB_MODE_CLOUD,		
		
	0x77777,						// brightness/contrast/sharpness/saturation/EV
	
	SENSOR_LOW_PULSE_RESET,			// reset pulse level
	20,								// reset pulse width(ms)
	
	SENSOR_HIGH_LEVEL_PWDN,			// 1: high level valid; 0: low level valid
	
	1,								// count of identify code
	0x0A, 0x76,						// supply two code to identify sensor.
	0x0B, 0x73,						// for Example: index = 0-> Device id, index = 1 -> version id	
									
	SENSOR_AVDD_2800MV,				// voltage of avdd	

	640,							// max width of source image
	480,							// max height of source image
	"OV7670",						// name of sensor												

	SENSOR_IMAGE_FORMAT_YUV422,		// define in SENSOR_IMAGE_FORMAT_E enum,
									// if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
	SENSOR_IMAGE_PATTERN_YUV422_YUYV,	// pattern of input image form sensor;			

	s_OV7670_resolution_Tab_YUV,	/*lint !e605*/ // point to resolution table information structure  
	&s_OV7670_ioctl_func_tab,		/*lint !e605*/ // point to ioctl function table    
			
	PNULL,							// information and table about Rawrgb sensor
	PNULL,							// extend information about sensor	
	SENSOR_AVDD_2800MV,                     // iovdd
	SENSOR_AVDD_1800MV,                      // dvdd
	4,                     // skip frame num before preview 
	3,                      // skip frame num before capture
	0,                      // deci frame num during preview	
	2,                      // deci frame num during video preview

	0,	// threshold enable		
	0,  // threshold mode 	
	0,	// threshold start postion	
	0,	// threshold end postion 
	0
};
/**---------------------------------------------------------------------------*
 ** 							Function  Definitions
 **---------------------------------------------------------------------------*/

LOCAL uint32 OV7670_Identify(uint32 param)
{
#define OV7670_PID_VALUE	0x76	
#define OV7670_PID_ADDR	0x0A
#define OV7670_VER_VALUE	0x73	
#define OV7670_VER_ADDR		0x0B	

	uint32 i;
	uint32 nLoop;
	uint8 ret;
	uint32 err_cnt = 0;
	uint8 reg[2] 	= {0x0A, 0x0B};
	uint8 value[2] 	= {0x76, 0x73};

	for(i = 0; i<2; )
	{
		nLoop = 1000;
		ret = s_lcm_func_table->sensor_readReg(reg[i]);
		if( ret != value[i])
		{
			err_cnt++;
			if(err_cnt>3)
			{
				return SCI_ERROR;
			}
			else
			{
				//Masked by frank.yang,s_lcm_func_table->sci_sleep() will cause a  Assert when called in boot precedure
				//s_lcm_func_table->sci_sleep(10);
				while(nLoop--){};
				continue;
			}
		}
        err_cnt = 0;
		i++;
	}

	s_lcm_func_table->trace("SENSOR: OV7670_Identify: it is OV7670");

	_ov7670_InitExifInfo();
	
	return (uint32)SCI_SUCCESS;
}

/******************************************************************************/
// Description: get ov7670 rssolution trim tab
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _ov7670_InitExifInfo(void)
{
    EXIF_SPEC_PIC_TAKING_COND_T* exif_ptr=&s_ov7670_exif;

    s_lcm_func_table->trace("SENSOR: _ov7670_InitExifInfo");

    exif_ptr->valid.FNumber=1;
    exif_ptr->FNumber.numerator=14;
    exif_ptr->FNumber.denominator=5;
    
    exif_ptr->valid.ExposureProgram=1;
    exif_ptr->ExposureProgram=0x04;

    //exif_ptr->SpectralSensitivity[MAX_ASCII_STR_SIZE];	
    //exif_ptr->ISOSpeedRatings;
    //exif_ptr->OECF;
    
    //exif_ptr->ShutterSpeedValue;
    
    exif_ptr->valid.ApertureValue=1;
    exif_ptr->ApertureValue.numerator=14;
    exif_ptr->ApertureValue.denominator=5;
    
    //exif_ptr->BrightnessValue;
    //exif_ptr->ExposureBiasValue;

    exif_ptr->valid.MaxApertureValue=1;
    exif_ptr->MaxApertureValue.numerator=14;
    exif_ptr->MaxApertureValue.denominator=5;
    
    //exif_ptr->SubjectDistance;
    //exif_ptr->MeteringMode;
    //exif_ptr->LightSource;
    //exif_ptr->Flash;

    exif_ptr->valid.FocalLength=1;
    exif_ptr->FocalLength.numerator=289;
    exif_ptr->FocalLength.denominator=100;
    
    //exif_ptr->SubjectArea;
    //exif_ptr->FlashEnergy;
    //exif_ptr->SpatialFrequencyResponse;
    //exif_ptr->FocalPlaneXResolution;
    //exif_ptr->FocalPlaneYResolution;
    //exif_ptr->FocalPlaneResolutionUnit;
    //exif_ptr->SubjectLocation[2];
    //exif_ptr->ExposureIndex;
    //exif_ptr->SensingMethod;

    exif_ptr->valid.FileSource=1;
    exif_ptr->FileSource=0x03;

    //exif_ptr->SceneType;
    //exif_ptr->CFAPattern;
    //exif_ptr->CustomRendered;

    exif_ptr->valid.ExposureMode=1;
    exif_ptr->ExposureMode=0x00;

    exif_ptr->valid.WhiteBalance=1;
    exif_ptr->WhiteBalance=0x00;
    
    //exif_ptr->DigitalZoomRatio;
    //exif_ptr->FocalLengthIn35mmFilm;
    //exif_ptr->SceneCaptureType;	
    //exif_ptr->GainControl;
    //exif_ptr->Contrast;
    //exif_ptr->Saturation;
    //exif_ptr->Sharpness;
    //exif_ptr->DeviceSettingDescription;
    //exif_ptr->SubjectDistanceRange;

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: get ov7670 rssolution trim tab
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _ov7670_GetResolutionTrimTab(uint32 param)
{
    return (uint32)s_OV7670_Resolution_Trim_Tab;
}

/******************************************************************************/
// Description: set ov7670 power down according to sensor id
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _ov7670_SetPowerDown(BOOLEAN is_on)
{
    if(SENSOR_MAIN==s_lcm_func_table->sensor_getCurId())
    {
        s_lcm_func_table->gpio_setsensorpwdn(is_on);
    }
    else if(SENSOR_SUB==s_lcm_func_table->sensor_getCurId())
    {
        s_lcm_func_table->gpio_setfrontsensorpwdn(is_on);
    }
}

/******************************************************************************/
// Description: sensor ov7670 power on/down sequence
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _ov7670_Power_On(uint32 power_on)
{
    SENSOR_AVDD_VAL_E		dvdd_val=g_OV7670_yuv_info.dvdd_val;
    SENSOR_AVDD_VAL_E		avdd_val=g_OV7670_yuv_info.avdd_val;
    SENSOR_AVDD_VAL_E		iovdd_val=g_OV7670_yuv_info.iovdd_val;  
    BOOLEAN 				power_down=g_OV7670_yuv_info.power_down_level;	    
    BOOLEAN 				reset_level=g_OV7670_yuv_info.reset_pulse_level;
    uint32 				reset_width=g_OV7670_yuv_info.reset_pulse_width;	    
    
    if(SCI_TRUE==power_on)
    {
        s_lcm_func_table->sensor_setvoltage(dvdd_val, avdd_val, iovdd_val);
        
        _ov7670_SetPowerDown((BOOLEAN)!power_down);

        // Open Mclk in default frequency
        s_lcm_func_table->sensor_setmclk(SENSOR_DEFALUT_MCLK);   
        
        s_lcm_func_table->sci_sleep(20);
        s_lcm_func_table->sensor_setresetlevel(reset_level);
        s_lcm_func_table->sci_sleep(reset_width);
        s_lcm_func_table->sensor_setresetlevel((BOOLEAN)!reset_level);
        s_lcm_func_table->sci_sleep(100);
    }
    else
    {
        _ov7670_SetPowerDown(power_down);

        s_lcm_func_table->sensor_setmclk(SENSOR_DISABLE_MCLK);           

        s_lcm_func_table->sensor_setvoltage(SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED);        
    }
    
    s_lcm_func_table->trace("SENSOR: _ov7670_Power_On(1:on, 0:off): %d", power_on);    
    
    return SCI_SUCCESS;
}

LOCAL uint32 set_ov7670_ae_enable(uint32 enable)
{
    unsigned char ae_value;

        ae_value=s_lcm_func_table->sensor_readReg(AE_ENABLE);

        if(0x00==enable)
        {
            ae_value&=0xfa;
            s_lcm_func_table->sensor_writereg(AE_ENABLE,ae_value);
        }
        else if(0x01==enable)
        {
            ae_value|=0x05;
            s_lcm_func_table->sensor_writereg(AE_ENABLE,ae_value);
        }

	s_lcm_func_table->trace("SENSOR: set_ae_enable: enable = %d", enable);

	return 0;
}


LOCAL uint32 set_ov7670_ae_awb_enable(uint32 ae_enable, uint32 awb_enable)
{
        unsigned char ae_value;

        ae_value=s_lcm_func_table->sensor_readReg(AE_ENABLE);

        if(0x00==ae_enable)
        {
            ae_value&=0xfa;
        }
        else if(0x01==ae_enable)
        {
            ae_value|=0x05;
        }

        if(0x00==awb_enable)
        {
            ae_value&=0xfd;
        }
        else if(0x01==awb_enable)
        {
            ae_value|=0x02;
        }        

        s_lcm_func_table->sensor_writereg(AE_ENABLE,ae_value);

        s_lcm_func_table->trace("SENSOR: set_ae_awb_enable: ae=%d awb=%d", ae_enable, awb_enable);

	return 0;
}



LOCAL uint32 set_hmirror_enable(uint32 enable)
{
 
	s_lcm_func_table->trace("set_hmirror_enable: enable = %d", enable);
	
	return 0;
}


LOCAL uint32 set_vmirror_enable(uint32 enable)
{

	s_lcm_func_table->trace("set_vmirror_enable: enable = %d", enable);
	
	return 0;
}


__align(4) LOCAL const SENSOR_REG_T ov7670_ev_tab[][4]=
{
    {{0x24, 0x30}, {0x25, 0x20}, {0x26, 0xa2}, {0xff, 0xff}},
    {{0x24, 0x40}, {0x25, 0x30}, {0x26, 0xb2}, {0xff, 0xff}},            
    {{0x24, 0x50}, {0x25, 0x40}, {0x26, 0xc2}, {0xff, 0xff}},            
    {{0x24, 0x60}, {0x25, 0x50}, {0x26, 0xc2}, {0xff, 0xff}},            
    {{0x24, 0x70}, {0x25, 0x60}, {0x26, 0xc2}, {0xff, 0xff}},            
    {{0x24, 0x80}, {0x25, 0x70}, {0x26, 0xd4}, {0xff, 0xff}},            
    {{0x24, 0x90}, {0x25, 0x80}, {0x26, 0xd5}, {0xff, 0xff}}      
};

LOCAL uint32 set_ov7670_ev(uint32 level)
{
    uint16 i;    
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)ov7670_ev_tab[level];

    s_lcm_func_table->Assert(PNULL != sensor_reg_ptr);   /*assert verified*/
    s_lcm_func_table->Assert(level < 7);

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        s_lcm_func_table->sensor_writereg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    
    s_lcm_func_table->trace("SENSOR: set_ev: level = %d", level);

    return 0;
}
/******************************************************************************/
// Description: anti 50/60 hz banding flicker
// Global resource dependence: 
// Author:
// Note:
//		level  must smaller than 8
/******************************************************************************/
LOCAL uint32 set_ov7670_anti_flicker(uint32 mode)
{//24m->65.6 us 12m->131us
    uint8 data=0x00;

    data=s_lcm_func_table->sensor_readReg(OV7670_COM11);
    switch(mode)
    {
        case DCAMERA_FLICKER_50HZ:
                data|=0x08;
                s_lcm_func_table->sensor_writereg(OV7670_COM11,data);                    
            break;
            
        case DCAMERA_FLICKER_60HZ:
                data&=0xf7;
                s_lcm_func_table->sensor_writereg(OV7670_COM11,data);                      
            break;
            
        default:
            break;
    }
    
    s_lcm_func_table->sci_sleep(100); 
    s_lcm_func_table->trace("SENSOR: set_banding_mode: mode = %d", mode);

    return 0;
}
/******************************************************************************/
// Description: set video mode
// Global resource dependence: 
// Author:
// Note:
//		 
/******************************************************************************/
__align(4) LOCAL const SENSOR_REG_T ov7670_video_mode_nand_tab[][7]=
{
    // normal mode
    {
        {0x2d, 0x00},{0x2e, 0x00},{0x14, 0x18},{0x92, 0x00}, {0x93, 0x00}, {0xa5, 0x02},{0xff, 0xff}
    },    
    //vodeo mode
    {
         {0x2d, 0x00},{0x2e, 0x00},{0x14, 0x18}, {0x92, 0x00}, {0x93, 0x00}, {0xa5, 0x02}, {0xff, 0xff}           
    }
};
/******************************************************************************/
// Description: set video mode
// Global resource dependence: 
// Author:
// Note:
//		 
/******************************************************************************/
__align(4) LOCAL const SENSOR_REG_T ov7670_video_mode_nor_tab[][7]=
{
    // normal mode
    {
        {0x2d, 0x00},{0x2e, 0x00},{0x14, 0x18},{0x92, 0x0}, {0x93, 0x00}, {0xa5, 0x02},{0xff, 0xff}
    }, 

    #ifdef CHIP_DRV_SC6600L
    //vodeo mode
    {// 30fps (for l page 10fps, for brust 15fps)
        {0x2d, 0x00},{0x2e, 0x00},{0x14, 0x28}, {0x92, 0x00}, {0x93, 0x00}, {0xa5, 0x02}, {0xff, 0xff}      
    }
    #else
     //vodeo mode
    {
        {0x2d, 0x00},{0x2e, 0x00},{0x14, 0x38}, {0x92, 0x00}, {0x93, 0x00}, {0xff, 0xff}, {0xff, 0xff}       
    }
    #endif 
};    

LOCAL uint32 set_ov7670_video_mode(uint32 mode)
{
    uint8 data=0x00;
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr = PNULL;

    s_lcm_func_table->Assert(mode <= 1);

    if(s_lcm_func_table->chip_detectmemtype())
        sensor_reg_ptr = (SENSOR_REG_T*)ov7670_video_mode_nand_tab[mode];
    else
        sensor_reg_ptr = (SENSOR_REG_T*)ov7670_video_mode_nor_tab[mode];

    s_lcm_func_table->Assert(PNULL != sensor_reg_ptr);   /*assert verified*/
    
    data=s_lcm_func_table->sensor_readReg(OV7670_COM11);
    data&=0x0f; 
    s_lcm_func_table->sensor_writereg(OV7670_COM11,data);          
    
    switch(mode)
    {
        case DCAMERA_NORMAL_MODE:
            s_lcm_func_table->sensor_writereg(PLL_ADDR,0x80);                  
            break;

        case DCAMERA_VIDEO_MODE:     
            s_lcm_func_table->sensor_writereg(PLL_ADDR,0x80);      
            break;

        default :
            break;
    }    

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
    	s_lcm_func_table->sensor_writereg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    //s_lcm_func_table->sci_sleep(500);

    s_lcm_func_table->trace("SENSOR: set_video_mode: mode = %d", mode);
    return 0;
}

/******************************************************************************/
// Description: set wb mode 
// Global resource dependence: 
// Author:
// Note:
//		
/******************************************************************************/
__align(4) LOCAL const SENSOR_REG_T ov7670_awb_tab[][7]=

{
    //AUTO
    {
        {0x13, 0xe7},
        {0x2d, 0x00},
        {0x2e, 0x00},
        {0xff, 0xff},
        {0xff, 0xff},
        {0xff, 0xff},
        {0xff, 0xff}            
    },    
    //INCANDESCENCE:
    {
        {0x13, 0xe5},
        {0x01, 0x78},
        {0x02, 0x58},
        {0x6a, 0x40},
        {0x2d, 0x00},
        {0x2e, 0x00},
        {0xff, 0xff}         
    },
    //U30
    {
        {0x13, 0xe7},
        {0x2d, 0x00},
        {0x2e, 0x00},
        {0xff, 0xff},
        {0xff, 0xff},
        {0xff, 0xff},
        {0xff, 0xff}            
    },  
    //CWF
    {
        {0x13, 0xe7},
        {0x2d, 0x00},
        {0x2e, 0x00},
        {0xff, 0xff},
        {0xff, 0xff},
        {0xff, 0xff},
        {0xff, 0xff}            
    },    
    //FLUORESCENT:
    {
        {0x13, 0xe5},
        {0x01, 0x96},
        {0x02, 0x40},
        {0x6a, 0x4a},
        {0x2d, 0x00},
        {0x2e, 0x00},
        {0xff, 0xff}           
    },
    //SUN:
    {
        {0x13, 0xe5},
        {0x01, 0x5a},
        {0x02, 0x5c},
        {0x6a, 0x42},
        {0x2d, 0x00},
        {0x2e, 0x00},
        {0xff, 0xff}            
    },
    //CLOUD:
    {
        {0x13, 0xe5},
        {0x01, 0x58},
        {0x02, 0x60},
        {0x6a, 0x40},
        {0x2d, 0x00},
        {0x2e, 0x00},
        {0xff, 0xff}            
    },
};

LOCAL uint32 set_ov7670_awb(uint32 mode)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)ov7670_awb_tab[mode];

	s_lcm_func_table->Assert(mode < 7);
	s_lcm_func_table->Assert(PNULL != sensor_reg_ptr);   /*assert verified*/
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		s_lcm_func_table->sensor_writereg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}

	s_lcm_func_table->sensor_setSensorExifInfo(SENSOR_EXIF_CTRL_LIGHTSOURCE, (uint32)mode);

	s_lcm_func_table->sci_sleep(100); 
	s_lcm_func_table->trace("SENSOR: set_awb_mode: mode = %d", mode);
	
	return 0;
}

/******************************************************************************/
// Description: set brightness 
// Global resource dependence: 
// Author:
// Note:
//		level  must smaller than 8
/******************************************************************************/
__align(4) LOCAL const SENSOR_REG_T ov7670_brightness_tab[][2]=
{
	{{0x55, 0xb0},{0xff,0xff}},
	{{0x55, 0xa0},{0xff,0xff}},
	{{0x55, 0x90},{0xff,0xff}},
	{{0x55, 0x00},{0xff,0xff}},
	{{0x55, 0x10},{0xff,0xff}},
	{{0x55, 0x20},{0xff,0xff}},
	{{0x55, 0x30},{0xff,0xff}},
};

LOCAL uint32 set_brightness(uint32 level)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)ov7670_brightness_tab[level];

	s_lcm_func_table->Assert(level < 7);
	s_lcm_func_table->Assert(PNULL != sensor_reg_ptr);   /*assert verified*/
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		s_lcm_func_table->sensor_writereg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}
	s_lcm_func_table->sci_sleep(100); 
	s_lcm_func_table->trace("SENSOR: set_brightness: level = %d", level);
	
	return 0;
}

__align(4) LOCAL const SENSOR_REG_T ov7670_contrast_tab[][2]=
{
	{{0x56, 0x20},{0xff,0xff}},
	{{0x56, 0x30},{0xff,0xff}},
	{{0x56, 0x40},{0xff,0xff}},	
	{{0x56, 0x50},{0xff,0xff}},
	{{0x56, 0x60},{0xff,0xff}},
	{{0x56, 0x70},{0xff,0xff}},
	{{0x56, 0x80},{0xff,0xff}},
};

LOCAL uint32 set_contrast(uint32 level)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr;

    sensor_reg_ptr = (SENSOR_REG_T*)ov7670_contrast_tab[level];

    s_lcm_func_table->Assert(level < 7 );
    s_lcm_func_table->Assert(PNULL != sensor_reg_ptr);   /*assert verified*/

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        s_lcm_func_table->sensor_writereg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    
    s_lcm_func_table->sensor_setSensorExifInfo(SENSOR_EXIF_CTRL_CONTRAST, (uint32)level);
    
    s_lcm_func_table->sci_sleep(20);
    s_lcm_func_table->trace("SENSOR: set_contrast: level = %d", level);

    return 0;
}


LOCAL uint32 set_sharpness(uint32 level)
{
	
	return 0;
}


LOCAL uint32 set_saturation(uint32 level)
{

	
	return 0;
}

/******************************************************************************/
// Description: set brightness 
// Global resource dependence: 
// Author:
// Note:
//		level  must smaller than 8
/******************************************************************************/

LOCAL uint32 set_preview_mode(uint32 preview_mode)
{
	s_lcm_func_table->trace("SENSOR: set_preview_mode: preview_mode = %d", preview_mode);
	
	s_preview_mode = preview_mode;
	
	switch (preview_mode)
	{
		case DCAMERA_ENVIRONMENT_NORMAL: 
		{
			OV7670_set_work_mode(0);
			break;
		}
		case DCAMERA_ENVIRONMENT_NIGHT:
		{
			OV7670_set_work_mode(1);
			break;
		}
		case DCAMERA_ENVIRONMENT_SUNNY:
		{
			OV7670_set_work_mode(0);
			break;
		}
		default:
		{
			break;
		}
			
	}
	
	s_lcm_func_table->sci_sleep(250);
	
	return 0;
}


/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//		
/******************************************************************************/
__align(4) LOCAL const SENSOR_REG_T ov7670_image_effect_tab[][4]=
{
    // effect normal
    {
        {0x3a, 0x04},{0x67, 0x80},{0x68, 0x80},{0xff, 0xff}
    },
    //effect BLACKWHITE
    {
        {0x3a, 0x14},{0x67, 0x80},{0x68, 0x80},{0xff, 0xff}
    },
    // effect RED
    {
        {0x3a, 0x14},{0x67, 0xc0},{0x68, 0x80},{0xff, 0xff}
    },
    // effect GREEN
    {
        {0x3a, 0x14},{0x67, 0x40},{0x68, 0x40},{0xff, 0xff}
    },
    // effect  BLUE
    {
        {0x3a, 0x14},{0x67, 0x80},{0x68, 0xc0},{0xff, 0xff}
    },
    // effect  YELLOW
    {
        {0x3a, 0x14},{0x67, 0x90},{0x68, 0x20},{0xff, 0xff}
    },  
    // effect NEGATIVE
    {	     
        {0x3a, 0x24},{0x67, 0x80},{0x68, 0x80},{0xff, 0xff}
    },    
    //effect ANTIQUE
    {
        {0x3a, 0x14},{0x67, 0xa0},{0x68, 0x40},{0xff, 0xff}
    },

};

	
LOCAL uint32 set_image_effect(uint32 effect_type)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)ov7670_image_effect_tab[effect_type];

	
	s_lcm_func_table->Assert(PNULL != sensor_reg_ptr);   /*assert verified*/
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
	{
		s_lcm_func_table->sensor_writereg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}
	s_lcm_func_table->trace("SENSOR: set_image_effect: effect_type = %d", effect_type);
    
	return 0;
}

LOCAL uint32 OV7670_After_Snapshot(uint32 param)
{

    s_lcm_func_table->trace("SENSOR: OV7670_BeforeSnapshot: OV7670_After_Snapshot ");  

    set_ov7670_ae_awb_enable(0x01, 0x01);

    s_lcm_func_table->sensor_writereg(PLL_ADDR, 0x80); 

    if(DCAMERA_ENVIRONMENT_NIGHT == s_preview_mode)
    {
          s_lcm_func_table->sensor_writereg(OV7670_COM11,(s_lcm_func_table->sensor_readReg(OV7670_COM11)&0x0f)|0xa0);  
          s_lcm_func_table->sensor_writereg(OV7670_COM11,(s_lcm_func_table->sensor_readReg(OV7670_COM11)&0x0f)|0xa0);  
    } 
    else
    {
        //OV7670_WriteReg(OV7670_COM11,(OV7670_ReadReg(OV7670_COM11)&0x0f)|0xa0);  
    }
    
    s_lcm_func_table->sensor_writereg(0x92, 0x00);  
    s_lcm_func_table->sensor_writereg(0x93, 0x00);    

    s_lcm_func_table->sci_sleep(200); // wait 2 frame the sensor working normal if no delay the lum is incorrect
 
    return 0;
    
}

LOCAL uint32 OV7670_BeforeSnapshot(uint32 param)
{

#define GAIN_ADDR   0x00
#define EXPOSAL_ADDR0   0x04
#define EXPOSAL_ADDR2   0x07
#define EXPOSAL_ADDR1    0x10
#define DUMMY_LINE0     0x2d
#define DUMMY_LINE1     0x2e

#define OV7670_LINE_LENGTH (784*2)
#define OV7670_C2P_RATIO 2 //preview 12M PCLK, capture 6M PCLK
#define OV7670_NIGHT_MAX_EXPLINE 1530 //Including dummline

#define  OV7670_NORMAL_MAX_EXPLINE 508 

    uint8 temp=0x00;
    uint8 gain=0x00;
    uint16 exposal=0x00;
    uint16 dummy_line=0x00;
    uint16 max_line = OV7670_NORMAL_MAX_EXPLINE;
    
    s_lcm_func_table->trace("SENSOR: OV7670_BeforeSnapshot: OV7670_BeforeSnapshot ");  

    if(DCAMERA_ENVIRONMENT_NORMAL == s_preview_mode)
    {
    	    max_line = OV7670_NORMAL_MAX_EXPLINE;
    }
    else if(DCAMERA_ENVIRONMENT_NIGHT == s_preview_mode)
    {
    	   max_line = OV7670_NIGHT_MAX_EXPLINE;
    }
    else
    {
         s_lcm_func_table->trace("unknown Camera mode!");
    }
	
    s_lcm_func_table->trace("OV7670_BeforeSnapshot max_line= %d", max_line );
    set_ov7670_ae_awb_enable(0x00, 0x00);

    gain=s_lcm_func_table->sensor_readReg(GAIN_ADDR);

    temp=s_lcm_func_table->sensor_readReg(EXPOSAL_ADDR0);
    exposal|=(temp&0X03);
    temp=s_lcm_func_table->sensor_readReg(EXPOSAL_ADDR1);
    exposal|=(temp<<0x02);
    temp=s_lcm_func_table->sensor_readReg(EXPOSAL_ADDR2); 
    exposal|=((temp&0x3f)<<0x0a);

    temp=s_lcm_func_table->sensor_readReg(DUMMY_LINE0);
    dummy_line|=temp;
    temp=s_lcm_func_table->sensor_readReg(DUMMY_LINE1); 
    dummy_line|=(temp<<0x08); 

    if((SENSOR_MODE_PREVIEW_ONE<param)
        &&(SENSOR_MODE_MAX>param))
    {
        s_lcm_func_table->sensor_setMode(param);
    }

    s_lcm_func_table->sensor_writereg(PLL_ADDR, 0x81);

    s_lcm_func_table->sensor_writereg(OV7670_COM11,s_lcm_func_table->sensor_readReg(OV7670_COM11)&0x0f);  //0x3b->0x0a
    s_lcm_func_table->sensor_writereg(OV7670_COM11,s_lcm_func_table->sensor_readReg(OV7670_COM11)&0x0f);  //0x3b->0x0a
   
    set_ov7670_ae_awb_enable(0x00, 0x00);

    exposal=exposal + dummy_line;

    exposal = exposal/OV7670_C2P_RATIO;
    
    if(0x80==(gain&0x80) &&( (exposal *2) < max_line ))
    {
        gain&=0x7f;
        exposal*=0x02;
    }

    if(0x40==(gain&0x40)&&( (exposal *2) < max_line ))
    {
        gain&=0xbf;
        exposal*=0x02;
    }
        
     if(0x20==(gain&0x20)&&( (exposal *2) < max_line ))
    {
        gain&=0xdf;
        exposal*=0x02;
    }

    if(0x10==(gain&0x10)&&( (exposal *2) < max_line ))
    {
        gain&=0xef;
        exposal*=0x02;
    } 

    s_lcm_func_table->trace("OV7670_BeforeSnapshot exposal= %d", exposal );
    if(exposal>498)
    {
        dummy_line=(exposal-498);
    }
 
    s_lcm_func_table->sensor_writereg(0x2d, 0);      
    s_lcm_func_table->sensor_writereg(0x2e, 0); 
  	
    s_lcm_func_table->sensor_writereg(GAIN_ADDR, gain);

    temp=s_lcm_func_table->sensor_readReg(EXPOSAL_ADDR0);
    temp&=0xfc;
    temp|=(exposal&0X03);
    s_lcm_func_table->sensor_writereg(EXPOSAL_ADDR0, temp);

     temp=((exposal>>0x02)&0xff);
    s_lcm_func_table->sensor_writereg(EXPOSAL_ADDR1, temp);

    temp=s_lcm_func_table->sensor_readReg(EXPOSAL_ADDR2);
    temp&=0xc0;
    temp|=(exposal>>0x0a)&0x3f;
    s_lcm_func_table->sensor_writereg(EXPOSAL_ADDR2, temp);  
    
    temp=(uint8)(dummy_line&0xff);

    s_lcm_func_table->sensor_writereg(0x92, temp);  

    
    temp=(uint8)(dummy_line>>0x08)&0xff;
    s_lcm_func_table->sensor_writereg(0x93, temp);  

    s_lcm_func_table->sensor_setSensorExifInfo(SENSOR_EXIF_CTRL_EXPOSURETIME, (uint32)exposal);

    if(DCAMERA_ENVIRONMENT_NORMAL == s_preview_mode)
    {
    	     s_lcm_func_table->sci_sleep(200); // wait 2 frame the sensor working normal if no delay the lum is incorrect
    }
    else if(DCAMERA_ENVIRONMENT_NIGHT == s_preview_mode)
    {
    	    s_lcm_func_table->sci_sleep(800); // wait 2 frame the sensor working normal if no delay the lum is incorrect
    }
    else
    {
          s_lcm_func_table->sci_sleep(800); // wait 2 frame the sensor working normal if no delay the lum is incorrect

    }
 
    return 0;
    
}

LOCAL uint32 read_ev_value(uint32 value)
{
	return 0;
}

LOCAL uint32 write_ev_value(uint32 exposure_value)
{
	
	return 0;	
}

LOCAL uint32 read_gain_value(uint32 value)
{

	
	return 0;
}

LOCAL uint32 write_gain_value(uint32 gain_value)
{

	
	return 0;
}

LOCAL uint32 read_gain_scale(uint32 value)
{
	return SENSOR_GAIN_SCALE;
	
}


LOCAL uint32 set_frame_rate(uint32 param)
    
{
	
	
	return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//		mode 0:normal;	 1:night 
/******************************************************************************/
__align(4) LOCAL const SENSOR_REG_T ov7670_mode_tab[][5]=
{
    // 30 fps
    {{0x2d, 0x00},{0x2e, 0x00},{0x14, 0x18},{0xa5, 0x02}, {0xff,0xff}}, // normal
    {{0x2d, 0x00},{0x2e, 0x00},{0x14, 0x28},{0xa5, 0x06}, {0xff,0xff}} // night  
};

LOCAL uint32 OV7670_set_work_mode(uint32 mode)
{
    uint8 data=0x00;
    uint16 i;

    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)ov7670_mode_tab[mode];

    s_lcm_func_table->Assert(mode <= 1);
    s_lcm_func_table->Assert(PNULL != sensor_reg_ptr);   /*assert verified*/

    data=s_lcm_func_table->sensor_readReg(OV7670_COM11);    
    data&=0x0f;
    
    switch(mode)
    {
        case DCAMERA_ENVIRONMENT_NORMAL:
            s_lcm_func_table->sensor_writereg(OV7670_COM11,data);           
            break;

        case DCAMERA_ENVIRONMENT_NIGHT:
            data|=0xa0;
            s_lcm_func_table->sensor_writereg(OV7670_COM11,data);                    
            break;

        default :
            break;
    }    

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        s_lcm_func_table->sensor_writereg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    s_lcm_func_table->sensor_setSensorExifInfo(SENSOR_EXIF_CTRL_SCENECAPTURETYPE, (uint32)mode);

    s_lcm_func_table->trace("SENSOR: set_work_mode: mode = %d", mode);
    return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
/******************************************************************************/
LOCAL uint32 _ov7670_GetExifInfo(uint32 param)
{
    return (uint32)&s_ov7670_exif;
}

