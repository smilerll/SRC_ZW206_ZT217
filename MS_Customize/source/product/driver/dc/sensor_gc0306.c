/******************************************************************************
 ** Copyright (c) 
 ** File Name:		sensor_GC0306.c 										  *
 ** Author: 													  *
 ** DATE:															  *
 ** Description:   This file contains driver for sensor GC0306. 
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
#include "ms_customize_trc.h"
#include "sensor_cfg.h"
#include "sensor_drv.h"
#include "os_api.h"
#include "chip.h"
#include "dal_dcamera.h"

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
extern uint32 OS_TickDelay(uint32 ticks);
/*
extern void Sensor_SetMCLK(uint32 mclk);
*/
//extern ERR_I2C_E I2C_WriteCmdArrNoStop(uint8 addr, uint8 *pCmd, uint32 len, BOOLEAN ack_en);

/**---------------------------------------------------------------------------*
 ** 						Const variables 								  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 ** 						   Macro Define
 **---------------------------------------------------------------------------*/
#define GC0306_I2C_ADDR_W		0x20//0x60
#define GC0306_I2C_ADDR_R		0x21//0x61

#define SENSOR_GAIN_SCALE		16

 
/**---------------------------------------------------------------------------*
 ** 					Local Function Prototypes							  *
 **---------------------------------------------------------------------------*/
LOCAL uint32 set_gc0306_ae_enable(uint32 enable);
LOCAL uint32 set_hmirror_enable(uint32 enable);
LOCAL uint32 set_vmirror_enable(uint32 enable);
LOCAL uint32 set_preview_mode(uint32 preview_mode);
LOCAL uint32 GC0306_Identify(uint32 param);
/*
LOCAL uint32 GC0306_BeforeSnapshot(uint32 param);
LOCAL uint32 GC0306_After_Snapshot(uint32 param);
*/
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
LOCAL uint32 GC0306_set_work_mode(uint32 mode);

LOCAL uint32 set_gc0306_ev(uint32 level);
LOCAL uint32 set_gc0306_awb(uint32 mode);
LOCAL uint32 set_gc0306_anti_flicker(uint32 mode);
LOCAL uint32 set_gc0306_video_mode(uint32 mode);

LOCAL BOOLEAN gc_enter_effect = SCI_FALSE;

/**---------------------------------------------------------------------------*
 ** 						Local Variables 								 *
 **---------------------------------------------------------------------------*/

__align(4) const SENSOR_REG_T gc0306_YUV_640X480[]=
{
    { 0x13, 0x00},//
    { 0xf1, 0x00},//
   // { 0x00, 0x11}, //    //hex!
    { 0x01, 0xf8}, //  f8
    { 0x02, 0xff}, // 66
    { 0x03, 0x1}, //  3
    { 0x04, 0x0e}, //  ae
    { 0x05, 0x00}, //
    { 0x06, 0x00}, //
    { 0x07, 0x00}, //
    { 0x08, 0x04}, //
    { 0x09, 0x01}, //01 
    { 0x0a, 0xf0}, //e8
    { 0x0b, 0x02}, //02
    { 0x0c, 0x90}, //88
    { 0x0d, 0x22}, //00
    { 0x0e, 0x20}, //a0
    /*changed by wangdali*/
    //{ 0x0f, 0xc0}, //00
    {0x0f,0x90},		//{0x0f,0xb0},			//Upside-down & mirror
    /*******************/
    { 0x10, 0x24}, //25
    { 0x11, 0x10}, //c
    { 0x12, 0x10}, //4
    { 0x13, 0x00}, //
    { 0x14, 0x00}, //
    { 0x15, 0x08}, //
    { 0x16, 0x04}, //
    { 0x17, 0x0}, //
    { 0x18, 0x19}, //
    { 0x19, 0x0}, //
    { 0x1a, 0x00}, //
    { 0x1b, 0x0}, //
    { 0x1c, 0x02}, //
    { 0x1d, 0x02}, //
    { 0x1e, 0x00}, //
    { 0x40, 0x7c}, //
    { 0x41, 0x2d}, // //0x3F
    { 0x42, 0x30}, //
    { 0x43, 0x60}, //
    { 0x44, 0xe2}, //
    /*changed by wangdali*/
    //{ 0x45, 0x26},
    { 0x45, 0x27},
    /*******************/
    { 0x46, 0x20}, //
 // { 0x47, 0x00}, //
    { 0x48, 0x00}, //
     { 0x4e, 0x22}, // //0x23 ,0x06
    { 0x4f, 0x1a}, //
    
    { 0x59, 0xef}, // 0xf0 //offset grG
    { 0x5a, 0xef}, //  //offset R
    { 0x5b, 0xef}, //  //offset B
    { 0x5c, 0xef}, //  //offset bgG
    { 0x61, 0x40}, //  //manual gain grG
    { 0x63, 0x40}, //  //manual gain R
    { 0x65, 0x60}, //  //manual gain B
    { 0x67, 0x40}, //  //manual gain bgG
    { 0x68, 0x28}, // 
    
    { 0x69, 0x46}, // 
    { 0x6a, 0xf8}, //
    { 0x6b, 0xf4}, //
    { 0x6c, 0xfc}, //
    { 0x6d, 0x53}, //
    { 0x6e, 0xe7},//0xf0}, //
    
    { 0x70, 0x00}, //
    { 0x71, 0x10}, //
    { 0x72, 0x00}, //
    { 0x73, 0x14}, //
    { 0x74, 0x00}, //
    { 0x75, 0x10}, //
    { 0x76, 0x28}, //
    { 0x77, 0x3c}, //
    { 0x78, 0x50}, //
    { 0x80, 0x10}, //
    { 0x81, 0x10}, //
    { 0x82, 0x34}, //
    { 0x83, 0x14}, //
    { 0x84, 0x40}, //
    { 0x85, 0x04}, //
   // { 0x86, 0x00}, //
    { 0x87, 0x93}, //
    { 0x88, 0x0a}, //
    { 0x89, 0x04}, //
    //{ 0x8a, 0x18}, // //NA
    { 0x8b, 0x0a}, //
    { 0x8c, 0x12}, //
    //{ 0x8d, 0x80}, // //black_level
    //{ 0x8e, 0x00}, //
    //{ 0x8f, 0x00}, //
    { 0x90, 0x1b}, //
    { 0x91, 0x35}, //
    { 0x92, 0x48}, //
    { 0x93, 0x63}, //
    { 0x94, 0x77}, //
    { 0x95, 0x88}, //
    { 0x96, 0x96}, //
    { 0x97, 0xa3}, //
    { 0x98, 0xba}, //
    { 0x99, 0xce}, //
    { 0x9a, 0xe0}, //
    { 0x9b, 0xf0}, //
    { 0x9c, 0xfe}, //
    /*
     { 0x90, 0x18}, //
    { 0x91, 0x2b}, //
    { 0x92, 0x3d}, //
    { 0x93, 0x5b}, //
    { 0x94, 0x77}, //
    { 0x95, 0x8f}, //
    { 0x96, 0xa3}, //
    { 0x97, 0xb5}, //
    { 0x98, 0xcf}, //
    { 0x99, 0xe1}, //
    { 0x9a, 0xef}, //
    { 0x9b, 0xf8}, //
    { 0x9c, 0xfe}, */
	
    { 0x9d, 0x80},//
    { 0x9e, 0x40},//
    { 0xa0, 0x50}, //  //0xa0,0x40,	  satuation ,!!!!!!!!
    { 0xa1, 0x40}, //
    { 0xa2, 0x30}, // Cb saturation
    { 0xa3, 0x30}, //
    { 0xa4, 0xc0}, //
    { 0xa5, 0x02}, //
    { 0xa6, 0x60}, //
    { 0xa7, 0x04}, //
    { 0xa8, 0xf4}, //
    { 0xa9, 0x0c}, //
    { 0xaa, 0x01}, //
    { 0xab, 0x00}, //
    { 0xac, 0xf8}, //
    { 0xad, 0x10}, //
    { 0xae, 0x80}, //
    { 0xaf, 0x80}, //
    { 0xb0, 0x10},//
    { 0xb1, 0xff},//
    { 0xbf, 0x18},//
    { 0xc0, 0x20}, //
    { 0xc1, 0xf0}, //
    { 0xc2, 0x07}, //
    { 0xc3, 0x80}, //
    { 0xc4, 0x01}, //
    { 0xc5, 0x21}, //
    { 0xc6, 0x80}, //
  //  { 0xc7, 0x00}, //
  //  { 0xc8, 0x00}, //
  //  { 0xc9, 0x00}, //
    { 0xca, 0x40}, //
    { 0xcb, 0x40}, //
    { 0xcc, 0x40}, //
    { 0xcd, 0x40}, //
    { 0xce, 0x40}, //40
    { 0xcf, 0x40}, //
    { 0xd0, 0x00}, //
//    { 0xd1, 0x50}, //
    { 0xd2, 0xf4}, //
  //  { 0xd3, 0x00}, //
    { 0xd4, 0x30}, //
    { 0xd5, 0xf0}, //
    { 0xd6, 0x76}, // 3b
    { 0xd7, 0x02}, //
    { 0xd8, 0x03}, //07
    { 0xd9, 0x30}, //
    { 0xda, 0x30}, //
  //  { 0xdb, 0x00}, //
  //  { 0xdc, 0x00}, //
    { 0xdd, 0x40}, //
    { 0xdf, 0x90},//
    { 0xe0, 0x05}, //
    { 0xe1, 0x05}, //
    { 0xe2, 0x34}, //
    { 0xe3, 0x24}, //
    { 0xe4, 0x14}, //
    { 0xe5, 0x11}, //
    { 0xe6, 0x24}, //
    { 0xe7, 0x18}, //

};


LOCAL SENSOR_REG_TAB_INFO_T s_GC0306_resolution_Tab_YUV[]=
{
	// COMMON INIT
	{ADDR_AND_LEN_OF_ARRAY(gc0306_YUV_640X480), 640, 480, 24, SENSOR_IMAGE_FORMAT_YUV422},
	
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

LOCAL SENSOR_IOCTL_FUNC_TAB_T s_GC0306_ioctl_func_tab = 
{
    // Internal 
    PNULL,
    PNULL,
    PNULL,
    GC0306_Identify,

    PNULL,			// write register
    PNULL,			// read  register	
    PNULL,
    PNULL,

    // External
    set_gc0306_ae_enable,
    set_hmirror_enable,
    set_vmirror_enable,

    set_brightness,
    set_contrast,
    set_sharpness,
    set_saturation,

    set_preview_mode,	
    set_image_effect,

	PNULL,	//	GC0306_BeforeSnapshot,
    PNULL,		//GC0306_After_Snapshot,

    PNULL,

    read_ev_value,
    write_ev_value,
    read_gain_value,
    write_gain_value,
    read_gain_scale,
    set_frame_rate,	
    PNULL,
    PNULL,
    set_gc0306_awb,
    PNULL,
    PNULL,
    set_gc0306_ev,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    set_gc0306_anti_flicker,
    set_gc0306_video_mode,
};

/**---------------------------------------------------------------------------*
 ** 						Global Variables								  *
 **---------------------------------------------------------------------------*/
 PUBLIC SENSOR_INFO_T g_GC0306_yuv_info =
{
	GC0306_I2C_ADDR_W,				// salve i2c write address
	GC0306_I2C_ADDR_R, 				// salve i2c read address
	
	0,								// bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
									// bit2: 0: i2c register addr  is 8 bit, 1: i2c register addr  is 16 bit
									// other bit: reseved
	SENSOR_HW_SIGNAL_PCLK_N|\
	SENSOR_HW_SIGNAL_VSYNC_N|\
	SENSOR_HW_SIGNAL_HSYNC_P,		// bit0: 0:negative; 1:positive -> polarily of pixel clock
									// bit2: 0:negative; 1:positive -> polarily of horizontal synchronization signal
									// bit4: 0:negative; 1:positive -> polarily of vertical synchronization signal
									// other bit: reseved											
											
	// preview mode
	SENSOR_ENVIROMENT_NORMAL|\
	SENSOR_ENVIROMENT_NIGHT|\
	SENSOR_ENVIROMENT_SUNNY,		
	
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
	0,
		
	7,								// bit[0:7]: count of step in brightness, contrast, sharpness, saturation
									// bit[8:31] reseved
	
	SENSOR_LOW_PULSE_RESET,			// reset pulse level
	20,								// reset pulse width(ms)
	
	SENSOR_HIGH_LEVEL_PWDN,			// 1: high level valid; 0: low level valid
	
	0,								// count of identify code
	0x00, 0xff,						// supply two code to identify sensor.
	0x00, 0xff,						// for Example: index = 0-> Device id, index = 1 -> version id	
									
	SENSOR_AVDD_2800MV,				// voltage of avdd	

	640,							// max width of source image
	480,							// max height of source image
	"GC0306",						// name of sensor												

	SENSOR_IMAGE_FORMAT_YUV422,		// define in SENSOR_IMAGE_FORMAT_E enum,
									// if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
	SENSOR_IMAGE_PATTERN_YUV422_YUYV,	// pattern of input image form sensor;			

	s_GC0306_resolution_Tab_YUV,	// point to resolution table information structure
	&s_GC0306_ioctl_func_tab,		// point to ioctl function table
			
	PNULL,							// information and table about Rawrgb sensor
	PNULL,							// extend information about sensor	
	
};
/**---------------------------------------------------------------------------*
 ** 							Function  Definitions
 **---------------------------------------------------------------------------*/
LOCAL void GC0306_WriteReg( uint8  subaddr, uint8 data )
{
	
	#ifndef	_USE_DSP_I2C_
		//uint8 cmd[2];
		//cmd[0]	=	subaddr;
		//cmd[1]	=	data;		
		//I2C_WriteCmdArr(GC0306_I2C_ADDR_W, cmd, 2, SCI_TRUE);
		Sensor_WriteReg(subaddr, data);
	#else
		DSENSOR_IICWrite((uint16)subaddr, (uint16)data);
	#endif

	//SCI_TRACE_LOW:"SENSOR: GC0306_WriteReg reg/value(%x,%x) !!"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0306_418_112_2_18_0_30_39_742,(uint8*)"dd", subaddr, data);

}

LOCAL uint8 GC0306_ReadReg( uint8  subaddr)
{
	uint8 value = 0;
	
	#ifndef	_USE_DSP_I2C_
	//I2C_WriteCmdArrNoStop(GC0306_I2C_ADDR_W, &subaddr, 1,SCI_TRUE);
	//I2C_ReadCmd(GC0306_I2C_ADDR_R, &value, SCI_TRUE);
	value =Sensor_ReadReg( subaddr);
	#else
		value = (uint16)DSENSOR_IICRead((uint16)subaddr);
	#endif

    //SCI_TRACE_LOW:"SENSOR: GC0306_ReadReg reg/value(%x,%x) !!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0306_434_112_2_18_0_30_39_743,(uint8*)"dd", subaddr, value);
    
	return value;
}


LOCAL uint32 GC0306_Identify(uint32 param)
{
#define GC0306_PID_VALUE	0x97	
#define GC0306_PID_ADDR		0x00
#define GC0306_VER_VALUE	0x97	
#define GC0306_VER_ADDR		0x00	

	uint32 i;
	uint32 nLoop;
	uint8 ret;
	uint32 err_cnt = 0;
	uint8 reg[2] 	= {0x00, 0x00};
	uint8 value[2] 	= {0x97, 0x97};

	//SCI_TRACE_LOW:"GC0306_Identify"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0306_454_112_2_18_0_30_39_744,(uint8*)"");
	for(i = 0; i<2; )
	{
		nLoop = 1000;
		ret = GC0306_ReadReg(reg[i]);
		if( ret != value[i])
		{
			err_cnt++;
			if(err_cnt>3)
			{
				//SCI_TRACE_LOW:"It is not GC0306"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0306_464_112_2_18_0_30_39_745,(uint8*)"");
				return SCI_ERROR;
			}
			else
			{
				//Masked by frank.yang,SCI_Sleep() will cause a  Assert when called in boot precedure
				//SCI_Sleep(10);
				while(nLoop--);
				continue;
			}
		}
        	err_cnt = 0;
		i++;
	}

	//SCI_TRACE_LOW:"GC0306_Identify: it is GC0306"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0306_479_112_2_18_0_30_39_746,(uint8*)"");
	
	return (uint32)SCI_SUCCESS;
}

LOCAL uint32 set_gc0306_ae_enable(uint32 enable)
{
	//SCI_TRACE_LOW:"set_gc0306_ae_enable: enable = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0306_486_112_2_18_0_30_39_747,(uint8*)"d", enable);
	return 0;
}


LOCAL uint32 set_hmirror_enable(uint32 enable)
{
 
	//SCI_TRACE_LOW:"set_hmirror_enable: enable = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0306_494_112_2_18_0_30_39_748,(uint8*)"d", enable);
	
	return 0;
}


LOCAL uint32 set_vmirror_enable(uint32 enable)
{

	//SCI_TRACE_LOW:"set_vmirror_enable: enable = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0306_503_112_2_18_0_30_39_749,(uint8*)"d", enable);
	
	return 0;
}
/******************************************************************************/
// Description: set brightness 
// Global resource dependence: 
// Author:
// Note:
//		level  must smaller than 8
/******************************************************************************/
__align(4) const SENSOR_REG_T gc0306_brightness_tab[][2]=
{

   	{
		{0xd1, 0x20},		
		{0xff,0xff}
	},

	{
		{0xd1, 0x30},		
		{0xff,0xff}
	},

	{
		{0xd1, 0x40},		
		{0xff,0xff}
	},

	{
		{0xd1, 0x50},		
		{0xff,0xff}
	},

	{
		{0xd1, 0x60},		
		{0xff,0xff}
	},

	{
		{0xd1, 0x68},		
		{0xff,0xff},
	},

	{
		{0xd1, 0x78},		
		{0xff,0xff}
	},
};

LOCAL uint32 set_brightness(uint32 level)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)gc0306_brightness_tab[level];

	SCI_ASSERT(level < 7);
	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		GC0306_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}
	OS_TickDelay(100); 
	//SCI_TRACE_LOW:"set_brightness: level = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0306_564_112_2_18_0_30_39_750,(uint8*)"d", level);
	
	return 0;
}

__align(4) const SENSOR_REG_T gc0306_ev_tab[][14]=
{
   {
		    { 0x90, 0x0f}, //
		    { 0x91, 0x1d}, //
		    { 0x92, 0x2d}, //
		    { 0x93, 0x46}, //
		    { 0x94, 0x5a}, //
		    { 0x95, 0x6b}, //
		    { 0x96, 0x7b}, //
		    { 0x97, 0x8a}, //
		    { 0x98, 0xa5}, //
		    { 0x99, 0xbe}, //
		    { 0x9a, 0xd5}, //
		    { 0x9b, 0xeb}, //
		    { 0x9c, 0xfe}, //	
                  {0xff  ,  0xff}
	},

	{
		    { 0x90, 0x13}, //
		    { 0x91, 0x25}, //
		    { 0x92, 0x37}, //
		    { 0x93, 0x50}, //
		    { 0x94, 0x65}, //
		    { 0x95, 0x76}, //
		    { 0x96, 0x86}, //
		    { 0x97, 0x94}, //
		    { 0x98, 0xae}, //
		    { 0x99, 0xc4}, //
		    { 0x9a, 0xd9}, //
		    { 0x9b, 0xed}, //
		    { 0x9c, 0xfe}, //	
                  {0xff  ,  0xff}
	},

	{
		    { 0x90, 0x17}, //
		    { 0x91, 0x2d}, //
		    { 0x92, 0x40}, //
		    { 0x93, 0x5a}, //
		    { 0x94, 0x6e}, //
		    { 0x95, 0x80}, //
		    { 0x96, 0x8f}, //
		    { 0x97, 0x9c}, //
		    { 0x98, 0xb4}, //
		    { 0x99, 0xca}, //
		    { 0x9a, 0xdd}, //
		    { 0x9b, 0xef}, //
		    { 0x9c, 0xfe}, //	
                  {0xff  ,  0xff}
	},

	{
		    { 0x90, 0x1b}, //
		    { 0x91, 0x35}, //
		    { 0x92, 0x48}, //
		    { 0x93, 0x63}, //
		    { 0x94, 0x77}, //
		    { 0x95, 0x88}, //
		    { 0x96, 0x96}, //
		    { 0x97, 0xa3}, //
		    { 0x98, 0xba}, //
		    { 0x99, 0xce}, //
		    { 0x9a, 0xe0}, //
		    { 0x9b, 0xf0}, //
		    { 0x9c, 0xfe}, //	
                  {0xff  ,  0xff}
	},

	{
		    { 0x90, 0x1e}, //
		    { 0x91, 0x3c}, //
		    { 0x92, 0x50}, //
		    { 0x93, 0x6b}, //
		    { 0x94, 0x7f}, //
		    { 0x95, 0x8f}, //
		    { 0x96, 0x9d}, //
		    { 0x97, 0xa9}, //
		    { 0x98, 0xbf}, //
		    { 0x99, 0xd2}, //
		    { 0x9a, 0xe2}, //
		    { 0x9b, 0xf1}, //
		    { 0x9c, 0xfe}, //	
                  {0xff  ,  0xff}
	},

	{
		    { 0x90, 0x22}, //
		    { 0x91, 0x43}, //
		    { 0x92, 0x58}, //
		    { 0x93, 0x73}, //
		    { 0x94, 0x86}, //
		    { 0x95, 0x96}, //
		    { 0x96, 0xa3}, //
		    { 0x97, 0xaf}, //
		    { 0x98, 0xc3}, //
		    { 0x99, 0xd5}, //
		    { 0x9a, 0xe4}, //
		    { 0x9b, 0xf2}, //
		    { 0x9c, 0xfe}, //	
                  {0xff  ,  0xff}
	},

	{
		    { 0x90, 0x25}, //
		    { 0x91, 0x4a}, //
		    { 0x92, 0x5f}, //
		    { 0x93, 0x79}, //
		    { 0x94, 0x8c}, //
		    { 0x95, 0x9b}, //
		    { 0x96, 0xa8}, //
		    { 0x97, 0xb4}, //
		    { 0x98, 0xc7}, //
		    { 0x99, 0xd8}, //
		    { 0x9a, 0xe6}, //
		    { 0x9b, 0xf3}, //
		    { 0x9c, 0xfe}, //	
                  {0xff  ,  0xff}
	},  
};

LOCAL uint32 set_gc0306_ev(uint32 level)
{

    uint16 i;    
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)gc0306_ev_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);
    SCI_ASSERT(level < 7);

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        GC0306_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    
    //SCI_TRACE_LOW:"SENSOR: set_ev: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0306_705_112_2_18_0_30_39_751,(uint8*)"d", level);

    return 0;
}
/******************************************************************************/
// Description: anti 50/60 hz banding flicker
// Global resource dependence: 
// Author:
// Note:
//		level  must smaller than 8
/******************************************************************************/
LOCAL uint32 set_gc0306_anti_flicker(uint32 mode)
{
	//24m->65.6 us 12m->131us
    switch(mode)
    {
        case DCAMERA_FLICKER_50HZ:
                GC0306_WriteReg(0x01, 0xf8);    
                GC0306_WriteReg(0xd6, 0x76);    
            break;
            
        case DCAMERA_FLICKER_60HZ:
                GC0306_WriteReg(0x01, 0xf1);    
                GC0306_WriteReg(0xd6, 0x63);                      
            break;
            
        default:
            break;
    }
    
    OS_TickDelay(100); 
    //SCI_TRACE_LOW:"SENSOR: set_banding_mode: mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0306_734_112_2_18_0_30_39_752,(uint8*)"d", mode);
    return 0;
}
/******************************************************************************/
// Description: set video mode
// Global resource dependence: 
// Author:
// Note:
//		 
/******************************************************************************/
__align(4) const SENSOR_REG_T gc0306_video_mode_tab[][4]=
{
	#if 1
    // normal mode 15FPS
    {
	{0xd8, 0x03},  { 0x02, 0xff}, {0xff, 0xff}
    },    
    
    {
	//{0xd8, 0x02},  { 0x02, 0x66}, {0xff, 0xff}   		////vodeo mode	20FPS
	{0xd8, 0x02},  { 0x02, 0x10}, {0xff, 0xff} 			////vodeo mode	24FPS     
    }
    #else
    // normal mode 15FPS
    {
	{0xd8, 0x03}, { 0x01, 0xf8}, { 0x02, 0xff}, {0xff, 0xff}
    },    
    //vodeo mode	26FPS
    {
	{0xd8, 0x02}, { 0x01, 0x10},  { 0x02, 0x66}, {0xff, 0xff}      
    }
    #endif
};    
LOCAL uint32 set_gc0306_video_mode(uint32 mode)
{

    uint16 i;

    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)gc0306_video_mode_tab[mode];

    SCI_ASSERT(mode <= 1);
    SCI_ASSERT(PNULL != sensor_reg_ptr);

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
    	GC0306_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    //SCI_TRACE_LOW:"SENSOR: set_video_mode: mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0306_780_112_2_18_0_30_39_753,(uint8*)"d", mode);
    return 0;
}

/******************************************************************************/
// Description: set wb mode 
// Global resource dependence: 
// Author:
// Note:
//		
/******************************************************************************/
__align(4) const SENSOR_REG_T gc0306_awb_tab[][6]=

{

     //AUTO
    {
        {0x41, 0x3d},
        {0xff, 0xff}
    },    
    //INCANDESCENCE:
    {
        {0x41, 0x39},
        {0xca, 0x60},
        {0xcb, 0x40},
        {0xcc, 0x80},
        {0xff, 0xff}         
    },
    //U30 ?
    {
       {0x41, 0x39},
        {0xca, 0x60},
        {0xcb, 0x40},
        {0xcc, 0x50},
        {0xff, 0xff}      
    },  
    //CWF ?
    {
        {0x41, 0x39},
        {0xca, 0x60},
        {0xcb, 0x40},
        {0xcc, 0x50},
        {0xff, 0xff}            
    },    
    //FLUORESCENT:
    {
        {0x41, 0x39},
        {0xca, 0x50},
        {0xcb, 0x40},
        {0xcc, 0x70},
        {0xff, 0xff}          
    },
    //SUN:
    {
        {0x41, 0x39},
        {0xca, 0x5a},
        {0xcb, 0x40},
        {0xcc, 0x58},
        {0xff, 0xff}           
    },
    //CLOUD:
    {
        {0x41, 0x39},
        {0xca, 0x68},
        {0xcb, 0x40},
        {0xcc, 0x50},
        {0xff, 0xff}            
    },
};

/*
// enum: preview while balance mode
typedef enum
{
	DCAMERA_WB_MODE_AUTO = 0x00,			//自动
	DCAMERA_WB_MODE_INCANDESCENCE,		//白炽灯
	DCAMERA_WB_MODE_U30,				//商用光源
	DCAMERA_WB_MODE_CWF,				//冷荧光
	DCAMERA_WB_MODE_FLUORESCENT,		//日光灯
	DCAMERA_WB_MODE_SUN,				//晴天
	DCAMERA_WB_MODE_CLOUD,				//阴天
	DCAMERA_WB_MODE_MAX
}DCAMERA_PARAM_WB_MODE_E;
*/
LOCAL uint32 set_gc0306_awb(uint32 mode)
{

	uint16 temp_reg;
	//SCI_TRACE_LOW:"SENSOR: set_gc0306_awb: mode = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0306_865_112_2_18_0_30_40_754,(uint8*)"d", mode);
	SCI_Sleep(200); 	
	temp_reg = GC0306_ReadReg(0x41);
	if(gc_enter_effect)
		return 0;

	switch(mode)
	{
		case DCAMERA_WB_MODE_AUTO:  // normal	
			GC0306_WriteReg(0x41, temp_reg | 0x04);
			break;
		case DCAMERA_WB_MODE_INCANDESCENCE:  // INCANDESCENCE	
			GC0306_WriteReg(0x41, temp_reg & ~ 0x04);
			GC0306_WriteReg(0xca, 0x60);
			GC0306_WriteReg(0xcb, 0x40);
			GC0306_WriteReg(0xcc, 0x80);
			break;
		case DCAMERA_WB_MODE_U30:  // U30	
			GC0306_WriteReg(0x41, temp_reg & ~ 0x04);
			GC0306_WriteReg(0xca, 0x60);
			GC0306_WriteReg(0xcb, 0x40);
			GC0306_WriteReg(0xcc, 0x80);
			break;
		case DCAMERA_WB_MODE_CWF:  // CWF	
			GC0306_WriteReg(0x41, temp_reg & ~ 0x04);
			GC0306_WriteReg(0xca, 0x60);
			GC0306_WriteReg(0xcb, 0x40);
			GC0306_WriteReg(0xcc, 0x80);
			break;
		case DCAMERA_WB_MODE_FLUORESCENT:  // FLUORESCENT	
			GC0306_WriteReg(0x41, temp_reg & ~ 0x04);
			GC0306_WriteReg(0xca, 0x50);
			GC0306_WriteReg(0xcb, 0x40);
			GC0306_WriteReg(0xcc, 0x70);
			break;
   		case DCAMERA_WB_MODE_SUN:  // SUN	
			GC0306_WriteReg(0x41, temp_reg & ~ 0x04);
			GC0306_WriteReg(0xca, 0x5a);
			GC0306_WriteReg(0xcb, 0x40);
			GC0306_WriteReg(0xcc, 0x58);
			break;
   		case DCAMERA_WB_MODE_CLOUD:  // CLOUD	
			GC0306_WriteReg(0x41, temp_reg & ~ 0x04);
			GC0306_WriteReg(0xca, 0x68);
			GC0306_WriteReg(0xcb, 0x40);
			GC0306_WriteReg(0xcc, 0x50);
			break;
		default:
			//SCI_TRACE_LOW:"set_gc0306_awb: not supported "
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0306_913_112_2_18_0_30_40_755,(uint8*)"");
			break;
       	
	}
	return 0;
}

__align(4) const SENSOR_REG_T gc0306_contrast_tab[][2]=
{
	{{0xa1, 0x10},{0xff,0xff}},
	{{0xa1, 0x20},{0xff,0xff}},
	{{0xa1, 0x30},{0xff,0xff}},	
	{{0xa1, 0x40},{0xff,0xff}},
	{{0xa1, 0x50},{0xff,0xff}},
	{{0xa1, 0x60},{0xff,0xff}},
	{{0xa1, 0x70},{0xff,0xff}},
};

LOCAL uint32 set_contrast(uint32 level)
{
    uint16 i;
    SENSOR_REG_T* sensor_reg_ptr;

    sensor_reg_ptr = (SENSOR_REG_T*)gc0306_contrast_tab[level];

    SCI_ASSERT(level < 7 );
    SCI_ASSERT(PNULL != sensor_reg_ptr);

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value); i++)
    {
        GC0306_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    OS_TickDelay(20);
    //SCI_TRACE_LOW:"set_contrast: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0306_946_112_2_18_0_30_40_756,(uint8*)"d", level);
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
	//SCI_TRACE_LOW:"set_preview_mode: preview_mode = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0306_973_112_2_18_0_30_40_757,(uint8*)"d", preview_mode);
	
	
	switch (preview_mode)
	{
		case DCAMERA_ENVIRONMENT_NORMAL: 
		{
			GC0306_set_work_mode(0);
			break;
		}
		case DCAMERA_ENVIRONMENT_NIGHT:
		{
			GC0306_set_work_mode(1);
			break;
		}
		case DCAMERA_ENVIRONMENT_SUNNY:
		{
			GC0306_set_work_mode(0);
			break;
		}
		default:
		{
			break;
		}
			
	}
	
	SCI_Sleep(250);
	
	return 0;
}
/*
typedef enum
{
	DCAMERA_EFFECT_NORMAL = 0x00,
	DCAMERA_EFFECT_BLACKWHITE,			// 黑白
	DCAMERA_EFFECT_RED,					// 偏红
	DCAMERA_EFFECT_GREEN,				// 偏绿
	DCAMERA_EFFECT_BLUE,				// 偏蓝
	DCAMERA_EFFECT_YELLOW,				// 偏黄
	DCAMERA_EFFECT_NEGATIVE,			// 底片
	DCAMERA_EFFECT_CANVAS,				// 帆布
	DCAMERA_EFFECT_RELIEVOS,			// 浮雕	
	DCAMERA_EFFECT_MAX
}DCAMERA_PARAM_EFFECT_E;
*/
LOCAL uint32 set_image_effect(uint32 effect_type)
{
	switch(effect_type)
	{
		case DCAMERA_EFFECT_NORMAL:  // normal
			GC0306_WriteReg(0x40, 0x7C);
			GC0306_WriteReg(0x41, 0x3D);
			GC0306_WriteReg(0x42, 0x30);
			GC0306_WriteReg(0x8C, 0x12);
			//GC0306_WriteReg(0xA1, 0x40);
			GC0306_WriteReg(0xA0, 0x50);
			GC0306_WriteReg(0xCD, 0x40);
			GC0306_WriteReg(0xCE, 0x40);
			GC0306_WriteReg(0xCF, 0x40);
			gc_enter_effect = SCI_FALSE;
			break;
		case DCAMERA_EFFECT_BLACKWHITE:  // B/W
			GC0306_WriteReg(0x40, 0x7C);
			GC0306_WriteReg(0x41, 0x3D);
			GC0306_WriteReg(0x42, 0x30);
			GC0306_WriteReg(0x8C, 0x12);
			//GC0306_WriteReg(0xA1, 0x40);
			GC0306_WriteReg(0xA0, 0x00);
			GC0306_WriteReg(0xCD, 0x40);
			GC0306_WriteReg(0xCE, 0x40);
			GC0306_WriteReg(0xCF, 0x40);
			gc_enter_effect = SCI_TRUE;
			break;		
		case DCAMERA_EFFECT_RED:  // sepia red
			GC0306_WriteReg(0x40, 0x7C);
			GC0306_WriteReg(0x41, 0x3D);
			GC0306_WriteReg(0x42, 0x30);
			GC0306_WriteReg(0x8C, 0x12);
			//GC0306_WriteReg(0xA1, 0x40);
			GC0306_WriteReg(0xA0, 0x50);
			GC0306_WriteReg(0xCD, 0x60);
			GC0306_WriteReg(0xCE, 0x30);
			GC0306_WriteReg(0xCF, 0x30);
			gc_enter_effect = SCI_TRUE;
			break;
		case DCAMERA_EFFECT_GREEN:  // sepia green
			GC0306_WriteReg(0x40, 0x7C);
			GC0306_WriteReg(0x41, 0x3D);
			GC0306_WriteReg(0x42, 0x30);
			GC0306_WriteReg(0x8C, 0x12);
			//GC0306_WriteReg(0xA1, 0x40);
			GC0306_WriteReg(0xA0, 0x50);
			GC0306_WriteReg(0xCD, 0x30);
			GC0306_WriteReg(0xCE, 0x60);
			GC0306_WriteReg(0xCF, 0x30);
			gc_enter_effect = SCI_TRUE;
			break;	
		case DCAMERA_EFFECT_BLUE:  // sepia blue
			GC0306_WriteReg(0x40, 0x7C);
			GC0306_WriteReg(0x41, 0x3D);
			GC0306_WriteReg(0x42, 0x30);
			GC0306_WriteReg(0x8C, 0x12);
			//GC0306_WriteReg(0xA1, 0x40);
			GC0306_WriteReg(0xA0, 0x50);
			GC0306_WriteReg(0xCD, 0x30);
			GC0306_WriteReg(0xCE, 0x30);
			GC0306_WriteReg(0xCF, 0x60);
			gc_enter_effect = SCI_TRUE;
			break;	
		case DCAMERA_EFFECT_YELLOW:  // convas
			GC0306_WriteReg(0x40, 0x7C);
			GC0306_WriteReg(0x41, 0x3D);
			GC0306_WriteReg(0x42, 0x30);
			GC0306_WriteReg(0x8C, 0x12);
			//GC0306_WriteReg(0xA1, 0x40);
			GC0306_WriteReg(0xA0, 0x50);
			GC0306_WriteReg(0xCD, 0x70);
			GC0306_WriteReg(0xCE, 0x58);
			GC0306_WriteReg(0xCF, 0x30);
			gc_enter_effect = SCI_TRUE;
			break;	
		case DCAMERA_EFFECT_NEGATIVE:  // NEGATIVE	
			SCI_Sleep(400); 		
			GC0306_WriteReg(0x41, 0x45);
			SCI_Sleep(200); 
			GC0306_WriteReg(0x40, 0x3c);
			GC0306_WriteReg(0x42, 0x30);
			SCI_Sleep(300); 
			GC0306_WriteReg(0x8C, 0x12);
			//GC0306_WriteReg(0xA1, 0x40);
			GC0306_WriteReg(0xA0, 0x50);
			GC0306_WriteReg(0xCD, 0x40);
			GC0306_WriteReg(0xCE, 0x40);
			GC0306_WriteReg(0xCF, 0x40);
			gc_enter_effect = SCI_TRUE;
			break;
		case DCAMERA_EFFECT_CANVAS:  // ANTIQUE
			GC0306_WriteReg(0x40, 0x7C);
			GC0306_WriteReg(0x41, 0x3D);
			GC0306_WriteReg(0x42, 0x30);
			GC0306_WriteReg(0x8C, 0x12);
			//GC0306_WriteReg(0xA1, 0x40);
			GC0306_WriteReg(0xA0, 0x50);
			GC0306_WriteReg(0xCD, 0x60);
			GC0306_WriteReg(0xCE, 0x50);
			GC0306_WriteReg(0xCF, 0x40);
			gc_enter_effect = SCI_TRUE;
			break;	
		case DCAMERA_EFFECT_RELIEVOS:  // CARVING
			SCI_Sleep(400); 			
			GC0306_WriteReg(0x41, 0x34);
			SCI_Sleep(200); 
			GC0306_WriteReg(0x40, 0x3c);
			GC0306_WriteReg(0x42, 0x34);
			SCI_Sleep(300); 
			GC0306_WriteReg(0x8C, 0x1f);
			//GC0306_WriteReg(0xA1, 0x60);
			GC0306_WriteReg(0xA0, 0x00);
			GC0306_WriteReg(0xCD, 0x40);
			GC0306_WriteReg(0xCE, 0x40);
			GC0306_WriteReg(0xCF, 0x40);
			gc_enter_effect = SCI_TRUE;
			break;
		default:
			//SCI_TRACE_LOW:"set_image_effect: not supported!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0306_1137_112_2_18_0_30_40_758,(uint8*)"");
			break;
			
	}
	return 0;
}


/*
LOCAL uint32 GC0306_After_Snapshot(uint32 param)
{

	Sensor_SetMCLK(24);
	
	GC0306_WriteReg(0x41,GC0306_ReadReg(0x41) | 0xf7);
	SCI_Sleep(200);
	return 0;
    
}

LOCAL uint32 GC0306_BeforeSnapshot(uint32 param)
{

    uint16 shutter = 0x00;
    uint16 temp_reg = 0x00;
    uint16 temp_r =0x00;
    uint16 temp_g =0x00;
    uint16 temp_b =0x00;    
    BOOLEAN b_AEC_on;
    

    //SCI_TRACE_LOW:"GC0306_BeforeSnapshot "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0306_1167_112_2_18_0_30_40_759,(uint8*)"");
    	if(GC0306_ReadReg(0X41)  & 0x08 == 0x08)  //AEC on
    		b_AEC_on = SCI_TRUE;
    	else
    		b_AEC_on = SCI_FALSE;

	temp_reg = GC0306_ReadReg(0xdb);
	temp_r = GC0306_ReadReg(0xcd);
	temp_g = GC0306_ReadReg(0xce);
	temp_b = GC0306_ReadReg(0xcf);

	shutter = (GC0306_ReadReg(0x03)<<8)  | (GC0306_ReadReg(0x04)&0x00ff) ;
	shutter = shutter /2;

	if(b_AEC_on)
		GC0306_WriteReg(0x41,GC0306_ReadReg(0x41) & 0xc5); //0x01);
	SCI_Sleep(300); 

///12m
	Sensor_SetMCLK(12);
	
	GC0306_WriteReg(0x03,shutter/256);
	GC0306_WriteReg(0x04,shutter & 0x00ff);	
   	//SCI_TRACE_LOW("GC0306_BeforeSnapshot, temp_r=%x,temp_reg=%x, final = %x ",temp_r,temp_reg, temp_r*temp_reg/ 0x80);    

	temp_r = (temp_r*temp_reg) / 0x80;
	temp_g = (temp_g*temp_reg) / 0x80;
	temp_b = (temp_b*temp_reg) / 0x80;
	if(b_AEC_on)
	{
		GC0306_WriteReg(0xcd, temp_r);
		GC0306_WriteReg(0xce, temp_g);
		GC0306_WriteReg(0xcf , temp_b);
	}
   	//SCI_TRACE_LOW("GC0306_BeforeSnapshot, temp_r=%x,temp_g=%x, temp_b = %x ",temp_r,temp_g,temp_b);    

	SCI_Sleep(300); 
    	return 0;
    
}
*/
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
	//GC0306_WriteReg( 0xd8, uint8 data );
	return 0;
}

/******************************************************************************/
// Description:
// Global resource dependence: 
// Author:
// Note:
//		mode 0:normal;	 1:night 
/******************************************************************************/
__align(4) const SENSOR_REG_T gc0306_mode_tab[][8]=
{
	//LCD的GAMMA值需要细调，不然会有一圈圈的光晕
	//Fps 12.5 YUV open auto frame function, 展讯的jpeg编码不行太大和太快，因此将帧率限制在12.5fps
	{/*{0xa0, 0x50},*/{0xd8, 0x03},{0x82, 0x34},{0x87, 0x93},{0x88, 0x0a},{0x89, 0x04},{0xFF, 0xFF},},
	//Fps 12.5->3.125 YUV open auto frame function
	{/*{0xa0, 0x40},*/{0xd8, 0x0a},{0x82, 0x11},{0x87, 0x91},{0x88, 0x10},{0x89, 0x08},{0xFF, 0xFF},},
};

LOCAL uint32 GC0306_set_work_mode(uint32 mode)
{
	uint16 i;
	SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)gc0306_mode_tab[mode];

	SCI_ASSERT(mode <= 1);
	SCI_ASSERT(PNULL != sensor_reg_ptr);
	
	for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value); i++)
	{
		GC0306_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
	}

	//SCI_TRACE_LOW:"set_work_mode: mode = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GC0306_1274_112_2_18_0_30_40_760,(uint8*)"d", mode);
	return 0;
}
