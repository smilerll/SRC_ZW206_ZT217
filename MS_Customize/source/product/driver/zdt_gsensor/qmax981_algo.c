/******************************************************************************
 ** File Name:      msensor_mir3da.c                                          *
 ** Author:          Yc.Wang                                                *
 ** DATE:            2015.08.31                                                *
 ** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2015.08.31   Yc.Wang            Create.                               *
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "i2c_api.h"
#include "os_api.h"
#include "gpio_prod_api.h"
#include "gpio_prod_cfg.h"
#include "Power_cfg.h"
#include "mira_std.h"
#include "zdt_gsensor.h"
#include "zdt_gsensor_qmax981.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" 
{
#endif

#define QMAX981_DEBUG
#if defined(QMAX981_DEBUG)
#define QMAX981_ALGO_INFO				SCI_TRACE_LOW   
#define QMAX981_ALGO_ERROR			SCI_TRACE_LOW
#else
#ifndef WIN32
#define QMAX981_ALGO_INFO(fmt, arg...)			do {} while (0)
#define QMAX981_ALGO_ERROR(fmt, arg...)			do {} while (0)
#endif
#endif

extern BOOLEAN qmax981_i2c_read_bytes(uint8 reg_no, uint8* buffer_name, uint32 length);
extern BOOLEAN gsensor_i2c_write_byte(uint8 ucBufferIndex, uint8 pucData);


#if defined(QMAX981_CHECK_ABNORMAL_DATA)
typedef struct
{
	int last_data;
	int curr_data;
	int more_data[3];
}qmaX981_data_check;

#define QMAX981_DIFF(x, y)		((x)>=(y)?((x)-(y)):((x)+65535-(y)))
#define QMAX981_ABNORMAL_DIFF		30
static qmaX981_data_check g_qmaX981_data_c;
#endif

#if defined(QMAX981_STEP_COUNTER_USE_INT)
#define STEP_INT_START_VLUE		4
#define STEP_DUMMY_VLUE			10

#define STEP_END	FALSE
#define STEP_START	TRUE

struct qmaX981_stepcount{
	//int stepcounter_pre_start;
	int stepcounter_pre_end;  
	int stepcounter_next_start;
	int stepcounter_next_end;  
	int stepcounter_pre;
	int stepcounter_pre_fix;
	BOOLEAN stepcounter_statu;
	BOOLEAN stepcounter_start_int_update_diff;
	int back;
	int step_diff;
	BOOLEAN int_statu_flag;
};

static struct qmaX981_stepcount step_count_index;
#endif


#if defined(QMAX981_STEP_COUNTER_USE_INT)
void qmaX981_step_debounce_reset(void)
{
	memset(&step_count_index, 0, sizeof(step_count_index));
}

int qmaX981_step_debounce_int_work(int data, unsigned char irq_level)
{
	int temp = 0;

	if(irq_level == 0)
	{	
		step_count_index.int_statu_flag = FALSE;
		step_count_index.stepcounter_next_end = data;
		step_count_index.stepcounter_statu = STEP_END;
		#ifndef WIN32
		QMAX981_ALGO_INFO( "step_int stepcounter_next_end = %d stepcounter_next_start = %d\n", step_count_index.stepcounter_next_end,step_count_index.stepcounter_next_start);
		#endif
		if(step_count_index.stepcounter_next_end < step_count_index.stepcounter_next_start)
		{
			temp = step_count_index.stepcounter_next_end - step_count_index.stepcounter_next_start+65536;
		}
		else
		{
			temp = step_count_index.stepcounter_next_end - step_count_index.stepcounter_next_start;
		}
		#ifndef WIN32
		QMAX981_ALGO_INFO( "step_int_end  temp =%d\n" ,temp);
		#endif
		if (temp < STEP_DUMMY_VLUE)
		{
			step_count_index.step_diff += (temp+STEP_INT_START_VLUE);
			// add by yangzhiqiang for step_diff
			if(step_count_index.step_diff > data)
			{
				step_count_index.step_diff = data;
			}
			// yangzhiqiang for step_diff
			step_count_index.stepcounter_pre_end = step_count_index.stepcounter_next_end;
			
		}
		else
		{
			step_count_index.stepcounter_pre_end = step_count_index.stepcounter_next_end;
		}		
		//irq_set_irq_type(g_QMA6981_ptr->irq,IRQF_TRIGGER_RISING);
		#ifndef WIN32
		QMAX981_ALGO_INFO( "step_int_end\n" );
		#endif
	}
	else
	{
		step_count_index.int_statu_flag = TRUE;
		step_count_index.stepcounter_next_start= data;
		step_count_index.stepcounter_statu = STEP_START;
		#ifndef WIN32
		QMAX981_ALGO_INFO( "step_int stepcounter_next_start = %d stepcounter_pre_end = %d\n", step_count_index.stepcounter_next_start,step_count_index.stepcounter_pre_end);
		#endif
		if (step_count_index.stepcounter_next_start < step_count_index.stepcounter_pre_end)
		{
			temp = step_count_index.stepcounter_next_start - step_count_index.stepcounter_pre_end+65536;
		}
		else
		{
			temp = step_count_index.stepcounter_next_start - step_count_index.stepcounter_pre_end;
		}
		#ifndef WIN32
		QMAX981_ALGO_INFO( "step_int_start temp =%d\n" ,temp);
		#endif
		if (temp >STEP_INT_START_VLUE)
		{
			step_count_index.step_diff += (temp - STEP_INT_START_VLUE);
		}
		//res = request_irq(g_QMA6981_ptr->irq, QMA6981_eint_handler, IRQF_TRIGGER_FALLING, "gsensor-eint", NULL);
		//irq_set_irq_type(g_QMA6981_ptr->irq,IRQF_TRIGGER_FALLING);
		#ifndef WIN32
		QMAX981_ALGO_INFO( "step_int_start\n" );
		#endif
	}

	return step_count_index.int_statu_flag;
}


int qmaX981_step_debounce_read_data(int result)
{
	int tempp = 0;
	int data = 0;

	if (result < step_count_index.stepcounter_pre)
	{
		step_count_index.back++;
		data = result- step_count_index.stepcounter_pre + 65536;
		step_count_index.stepcounter_pre = result;
	}
	else
	{
		//nothing
		step_count_index.stepcounter_pre = result;
		data = result;
	}

	if (step_count_index.stepcounter_statu == STEP_START)
	{
	/*
		if (data >= step_count_index.stepcounter_pre_end)
		{
			tempp = data - step_count_index.stepcounter_pre_end;
		}
		else
		{
			tempp = data - step_count_index.stepcounter_pre_end +65536;
		}
	*/
		if (data >= step_count_index.stepcounter_next_start)
		{
			tempp = data - step_count_index.stepcounter_next_start + 4;
		}
		else
		{
			tempp = data - step_count_index.stepcounter_next_start +65540;
		}

		#ifndef WIN32
		QMAX981_ALGO_INFO( "ReadStepCounter_running data= %d,stepcounter_next_start = %d,tempp = %d,stepcounter_pre_end =%d \n",data,step_count_index.stepcounter_next_start,tempp,step_count_index.stepcounter_pre_end);
		#endif
		
		if (tempp < (STEP_INT_START_VLUE+STEP_DUMMY_VLUE))
		{
			data = step_count_index.stepcounter_pre_fix;
			#ifndef WIN32
			QMAX981_ALGO_INFO( "ReadStepCounter_running stepcounter_pre_fix = %d\n",step_count_index.stepcounter_pre_fix);
			#endif
		}
		else
		{
			if (step_count_index.step_diff >data)
			{
				step_count_index.step_diff = 0;
			}
			else
			{
				 data = data -  step_count_index.step_diff;
				 step_count_index.stepcounter_pre_fix = data;
				 #ifndef WIN32
				 QMAX981_ALGO_INFO( "ReadStepCounter_running stepcounter_pre_fix = %d\n",step_count_index.stepcounter_pre_fix);
				 #endif
			}
		}
	
	}
	else 
	{
		// add by yangzhiqiang for step_diff
		if(step_count_index.step_diff > data)
		{
			step_count_index.step_diff = data;
		}
		// yangzhiqiang for step_diff		
#if 1//defined(QMA6981_STEP_TO_ZERO)
		step_count_index.stepcounter_pre_end = data;
#endif
		data  = data -  step_count_index.step_diff;
		 step_count_index.stepcounter_pre_fix = data;
		 #ifndef WIN32
		 QMAX981_ALGO_INFO( "ReadStepCounter_end stepcounter_pre_fix = %d\n",step_count_index.stepcounter_pre_fix);
		 #endif
	}
	//mutex_unlock(&qma6981_mutex); // removed by yangzhiqiang
	#ifndef WIN32
	QMAX981_ALGO_INFO( "ReadStepCounter=%d, step_diff= %d\n",data,step_count_index.step_diff );
	#endif

	return data;
}
#endif


#if defined(QMAX981_CHECK_ABNORMAL_DATA)
int qmaX981_check_abnormal_data(int data_in, int *data_out)
{
	BOOLEAN ret = 0;
	int diff;
	unsigned char data[2];

	g_qmaX981_data_c.curr_data = data_in;
	diff = QMAX981_DIFF(g_qmaX981_data_c.curr_data, g_qmaX981_data_c.last_data);
	if(diff > QMAX981_ABNORMAL_DIFF)
	{
		// read data 1
		//data[0] = QMAX981_STEP_CNT_L;
		//ret = qmaX981_RxData(data, 2);
		ret = qmax981_i2c_read_bytes(QMAX981_STEP_CNT_L, data, 2);
		if(ret == FALSE)
			g_qmaX981_data_c.more_data[0] = -1;
		else
			g_qmaX981_data_c.more_data[0] = ((data[1]<<8) |( data[0]));
		
		// read data 2
		ret = qmax981_i2c_read_bytes(QMAX981_STEP_CNT_L, data, 2);
		if(ret == FALSE)
			g_qmaX981_data_c.more_data[1] = -1;
		else
			g_qmaX981_data_c.more_data[1] = ((data[1]<<8) |( data[0]));


		// read data 3
		ret = qmax981_i2c_read_bytes(QMAX981_STEP_CNT_L, data, 2);
		if(ret == FALSE)
			g_qmaX981_data_c.more_data[2] = -1;
		else
			g_qmaX981_data_c.more_data[2] = ((data[1]<<8) |( data[0]));


		if((g_qmaX981_data_c.more_data[0]<0)||(g_qmaX981_data_c.more_data[1]<0)||(g_qmaX981_data_c.more_data[2]<0))
		{
			return 0;
		}
		
		//if((QMAX981_ABS(g_qmaX981_data_c.more_data[0]-g_qmaX981_data_c.curr_data) > 1)
		//	||(QMAX981_ABS(g_qmaX981_data_c.more_data[1]-g_qmaX981_data_c.curr_data) > 1)
		//	||(QMAX981_ABS(g_qmaX981_data_c.more_data[2]-g_qmaX981_data_c.curr_data) > 1)
		//	)
		if((g_qmaX981_data_c.more_data[0]==g_qmaX981_data_c.more_data[1])
			||(g_qmaX981_data_c.more_data[1]==g_qmaX981_data_c.more_data[2]))
		{		
			*data_out = g_qmaX981_data_c.more_data[0];
			g_qmaX981_data_c.last_data = g_qmaX981_data_c.more_data[0];
		}
		else
		{		
			return 0;
		}
	}
	else
	{
		g_qmaX981_data_c.last_data = g_qmaX981_data_c.curr_data;
	}

	return 1;
}
#endif

#if defined(QMAX981_HAND_LIGHT)
extern BOOLEAN qmaX981_get_acc_mg(int32 *x_adc, int32 *y_adc, int32 *z_adc);

#define ACC_HANDLIGHT_THRESHOLD		18000	//19000 mm/s2
#define ACC_STATIC_THRESHOLD				1200
#define QMAX981_ABS(X) ((X) < 0 ? (-1 * (X)) : (X))

typedef enum
{
	HANDLIGHT_FREE,
	HANDLIGHT_RUN,
	HANDLIGHT_TOTAL
}HandLightStatus;

typedef struct
{
	int			xyz_sum[2];
	int			debounce;
	int			count;
	HandLightStatus		status;
}HandLight;

static HandLight	g_handlight;

int qmaX981_hand_light_algo(void)
{
	int	xyz_diff = 0;
	int acc[3];
	BOOLEAN ret;

	ret = qmaX981_get_acc_mg(&acc[0], &acc[1], &acc[2]);
	if(ret == FALSE)
	{
		return 0;
	}
	switch(g_handlight.status)
	{
		case HANDLIGHT_FREE:			
			g_handlight.xyz_sum[0] = QMAX981_ABS(acc[0])+QMAX981_ABS(acc[1]);	//+QMAX981_ABS(acc[2]);
			//QMAX981_ALGO_INFO( " raise_hand start run acc_abs_total=%d \n", g_handlight.xyz_sum[0]);
			if((g_handlight.xyz_sum[0] > ACC_HANDLIGHT_THRESHOLD))
			{			
				//QMAX981_ALGO_INFO( " raise_hand start run acc_abs_total=%d \n", g_handlight.xyz_sum[0]);
				g_handlight.status = HANDLIGHT_RUN;
			}
			g_handlight.count = 0;
			g_handlight.debounce = 0;
			break;
		case HANDLIGHT_RUN:
			g_handlight.count++;
			if(g_handlight.count > 18)		//125
			{
				g_handlight.status = HANDLIGHT_FREE;
			}
			else
			{
				g_handlight.xyz_sum[1] = g_handlight.xyz_sum[0];
				g_handlight.xyz_sum[0] = QMAX981_ABS(acc[0])+QMAX981_ABS(acc[1]);//+QMAX981_ABS(acc[2]);
				xyz_diff = QMAX981_ABS((g_handlight.xyz_sum[0]-g_handlight.xyz_sum[1]));
				//QMAX981_ALGO_INFO( " raise_hand start run xyz_diff=%d, acc[]=%d %d %d \n", xyz_diff, acc[0],acc[1],acc[2]);
				if(g_handlight.xyz_sum[0] > ACC_HANDLIGHT_THRESHOLD)
				{
					g_handlight.count = 0;
					g_handlight.debounce = 0;
				}
				else if((xyz_diff < ACC_STATIC_THRESHOLD))
				{
					g_handlight.debounce++;
					if(g_handlight.debounce > 3)		// 65
					{
						if((QMAX981_ABS(acc[0])<3000) && (acc[1]>0&&acc[1]<10000))
						{
							#ifndef WIN32
							QMAX981_ALGO_INFO( " raise_hand hand up report to turn on LCM! \n");
							#endif
							return 1;
						}
						else if((QMAX981_ABS(acc[1])<4000) && (QMAX981_ABS(acc[0])>7000))
						{	
							#ifndef WIN32
							QMAX981_ALGO_INFO( " raise_hand hand down report to turn off LCM! \n");
							#endif
							return 2;
						}
						// to do 
						g_handlight.status = HANDLIGHT_FREE;
					}
				}
			}

			break;
		default:
			break;
	}

	return 0;
}

#endif

#ifdef WIN32
#define __func__ 
#endif

#ifdef __cplusplus
}
#endif  // End of file


