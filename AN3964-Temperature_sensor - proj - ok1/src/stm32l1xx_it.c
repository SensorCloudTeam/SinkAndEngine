/**
  ******************************************************************************
  * @file    Project/STM32L1xx_StdPeriph_Template/stm32l1xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    31-December-2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_it.h"
#include "main.h"
#include "string.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//USART3
#define MAX_BUF 1000							// 从Engine接收的消息最大长度
char temp;												// 用于记录接收char的临时变量
char buf[MAX_BUF];								// 记录从Engine接收的消息的数组
int count = 0;										// 消息char长度
int end = 0;											// 数组记录的消息最后位置


//USART2
#define type_c		5								// 传感器类型数量
#define ID_c			5								// 传感器ID数量
#define data_BUF	10							// 历史数据长度
#define FRAME_LONG 15							// 帧长度

enum type{PH1,PH2,Light,Tem,Hum};		// 传感器类型enum定义

char f_buf[FRAME_LONG];						// 存储帧数组
int f_count;									// 帧已存储char长度
int d_count[ID_c];								// Sensordatas已记录char长度
int d_current[ID_c],d_first[ID_c],d_end[ID_c];	// 指向新数据位置、历史数据最新位置、历史数据最久位置
int sum_temp;											// 存储计算后得到的校验和临时变量
int id_temp;											// 存储ID临时变量
float val_temp;										// 存储val临时变量
float cyc_temp;											// 存储cycle临时变量
int k;

float SensorDatas[ID_c][data_BUF];	// 记录从变频传感器接收到的数据信息
/*
维数：传感器ID、缓冲大小
SensorDatas[0][]	ID:0
SensorDatas[1][]	ID:1
SensorDatas[2][]	ID:2
SensorDatas[3][]	ID:3
SensorDatas[4][]	ID:4
每个数组保存10个该种类型数据值
*/

int SensorType[ID_c]={PH1,PH2,Light,Tem,Hum};	// ID 与 type 对应
/*
ID0:PH
ID1:Light
ID2:Tem
ID3:Hum
ID4:O
*/

float sensorCycle[ID_c];	// 存储周期


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    TimingDelay_Decrement();
}
/******************************************************************************/
/*                 STM32L1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l1xx_md.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles DMA Transfer Complete interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Channel1_IRQHandler    (void)
{
  DMA_ClearFlag(DMA1_IT_TC1);
  setADCDMA_TransferComplete();  /* set flag_ADCDMA_TransferComplete global flag */
}


/**
  * @brief  This function handles RTC Auto wake-up interrupt request.
  * @param  None
  * @retval None
  */
void RTC_WKUP_IRQHandler(void)
{
  if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
  {
    /* Clear RTC and EXTI interrupt flags */
  	RTC_ClearITPendingBit(RTC_IT_WUT);
  	EXTI_ClearITPendingBit(EXTI_Line20);    
  } 
}

/**
  * @brief  This function handles external interrupts generated by UserButton.
  * @param  None
  * @retval None
  */

void EXTI0_IRQHandler(void)
{
	LCD_GLASS_Clear();
	LCD_GLASS_DisplayString(" 111");
	Delay(5000);
  /* set UserButton Flag */
  setUserButtonFlag(); 

  EXTI_ClearITPendingBit(EXTI_Line0);
}

// Service <---> Engine
void USART3_IRQHandler(void)
{
	if( USART_GetITStatus( USART3, USART_IT_RXNE ) != RESET )
	{
		
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		
		if(count>=MAX_BUF)
		{
			LCD_GLASS_Clear();
			LCD_GLASS_DisplayString(" Error");
			return;
		}
		buf[end++] = USART_ReceiveData(USART3);
		
		count++;
	}
	
	if(USART_GetFlagStatus(USART3,USART_FLAG_ORE) == SET)
	{
		USART_ClearFlag(USART3,USART_FLAG_ORE);
		USART_ReceiveData(USART3);
	}
}

// Sensor <---> Service
void USART2_IRQHandler(void)
{
	if( USART_GetITStatus( USART2, USART_IT_RXNE ) != RESET )
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		
		if(f_count>=FRAME_LONG)
		{
		}
		else{
			f_buf[f_count] = USART_ReceiveData(USART2);
			if(f_buf[f_count]==0xFD)	//帧的开始
			{
				f_count++;
			}
			else if(f_count>0)
			{
				f_count++;
			}
		}
		
		//收集完一帧数据，开始分析
		if(f_count==FRAME_LONG)
		{
			// 校验和
			for(k=5; k<14; k++)
				sum_temp += f_buf[k];
			if(sum_temp==f_buf[4])// 校验和正确
			{
				// 1.得到 ID
				id_temp = f_buf[5]*10+f_buf[6];
				// 2.得到 Val
				val_temp = f_buf[7]*10+f_buf[8]+f_buf[9]*0.10+f_buf[10]*0.01;
				// 3.得到 周期
				cyc_temp = f_buf[11]*10+f_buf[12];
				
				sensorCycle[id_temp]=cyc_temp;
				
				// 4.采用循环数组的方式存储历史数据
				// int SensorType[ID_c]={PH,Light,Tem,Hum,O};
				// int SensorDatas[ID_c][data_BUF];
				if(d_count[id_temp]>=data_BUF)
				{
					SensorDatas[id_temp][ (d_current[id_temp])%10 ] = val_temp;
					//d_current[id_temp]++;
					d_current[id_temp]=(d_current[id_temp]+1)%10;
					//d_first[id_temp]++;
					d_first[id_temp]=(d_first[id_temp]+1)%10;
					//d_end[id_temp]++;
					d_end[id_temp]=(d_end[id_temp]+1)%10;
					//d_count[id_temp]++;
				}
				else if(d_count[id_temp]==0)
				{
					SensorDatas[id_temp][ d_current[id_temp] ] = val_temp;
					//d_current[id_temp]++;
					d_current[id_temp]=(d_current[id_temp]+1)%10;
					d_count[id_temp]++;
				}
				else if(0<d_count[id_temp]&&d_count[id_temp]<data_BUF)
				{
					SensorDatas[id_temp][ d_current[id_temp] ] = val_temp;
					//d_current[id_temp]++;
					d_current[id_temp]=(d_current[id_temp]+1)%10;
					//d_first[id_temp]++;
					d_first[id_temp]=(d_first[id_temp]+1)%10;
					d_count[id_temp]++;
				}
			}
			f_count=0;	// 处理完一帧后将帧长计数清零
			sum_temp=0;
			for(k=0;k<15;k++)
				f_buf[k]=0;
		}
		
	}
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET)
	{
		USART_ClearFlag(USART2,USART_FLAG_ORE);
		USART_ReceiveData(USART2);
	}
	

}


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
