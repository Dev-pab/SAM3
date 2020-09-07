/*----------------------------------------------------------------------------
 *      E x a m p l e
 *----------------------------------------------------------------------------
 *      Name:    EN_TETE.C
 *      Purpose: Fichier de démarrage
 *      Rev.:    V3.20
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2008 KEIL - An ARM Company. All rights reserved.
 *-----------------------------------------------------------------------------
 * FONCTION		:  
 *
 *------------------------------------------------------------------------------
 * AUTEUR				:  A. MARQUES
 * Développé le 		:  13 - 03 - 09
 * Dernière modif le 	:
 *------------------------------------------------------------------------------ */

// |=(1<<2) set bit
// &=~(1<<2) reset bit

#include "stm32f10x_lib.h"            /* STM32F10x Library Definitions       */

void CLOCK_init(void);
void FLASH_init(void);
void GPIO_init(void);
void EXTI_init (void);
void NVIC_init(void);
void DMA_init(void);
void ADC_init (void);
void timer_init(void);
void TIM2_IRQHandler (void);

	//uc8 message1[10000]=" Centre de microelectronique de provence - George Charpak";
	//u8 message2[10000]="";
	u16 ADCvalue[512];

/*----------------------------------------------------------------------------
 *        Main: Initialize 
 *---------------------------------------------------------------------------*/
int main (void) 
{                     /* program execution starts here       */
  //int i;

	CLOCK_init();
  GPIO_init();
	//EXTI_init ();
	NVIC_init();
	//DMA_init();
	//ADC_init();
	timer_init();
	
	//GPIO_ResetBits(GPIOB, GPIO_Pin_All);
	GPIO_ResetBits (GPIOB, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	/*
	for (i=0 ; i<10000 ; i++)
	{
		message2[i] = message1[i];
	}
	*/ 
	
	//DMA_Cmd(DMA1_Channel1, ENABLE );
	//ADC_SoftwareStartConvCmd( ADC1, ENABLE );
	
  while (1)
	{
		//GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(1^GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8)));
		//ADCvalue = ADC_GetConversionValue(ADC1);
	}
}

void CLOCK_init(void)
{
	ErrorStatus HSEStartUpStatus;
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStartUpStatus = RCC_WaitForHSEStartUp(); /* Wait till HSE is ready */
	
	if(HSEStartUpStatus == SUCCESS) 
	{
		FLASH_init();
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		RCC_PLLCmd(ENABLE);
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div2);
		RCC_PCLK2Config(RCC_HCLK_Div1);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SRAM|RCC_AHBPeriph_FLITF, ENABLE );
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	} 
	else while (1);
	
}

void FLASH_init(void)
{
	FLASH_SetLatency(FLASH_Latency_2);
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
}

void GPIO_init(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitTypeDef GPIOA_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIOA_InitStructure.GPIO_Pin = GPIO_PinSource1;
	GPIOA_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIOA_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIOA_InitStructure);
}

void EXTI_init (void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1); /* Selects PA.1 as EXTI */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0; /* Line 1 */
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

void NVIC_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* Configure 2 bits for preemption priority */
	//NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQChannel; /* Enable the EXTI1 Interrupt */
	//NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQChannel; /* Enable the DMA1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel; /* Enable TIM2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM2_IRQHandler (void)
{
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(1^GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8)));
}

void EXTI0_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line0); // Acquitement
	GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(1^GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8)));
}

void DMAChannel1_IRQHandler(void)
{
	DMA_ClearFlag(DMA1_FLAG_TC1);
	GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)(1^GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_9)));
}

void DMA_init(void)
{
	DMA_InitTypeDef DMA_InitStruct;
	//DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&message1;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32) &ADC1->DR;
	//DMA_InitStruct.DMA_MemoryBaseAddr = (u32)&message2;
	DMA_InitStruct.DMA_MemoryBaseAddr = (u32) &ADCvalue;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStruct.DMA_BufferSize = 512;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	//DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;
	DMA_Init( DMA1_Channel1, &DMA_InitStruct );
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
}

void ADC_init (void)
{
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None ;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init( ADC1, &ADC_InitStructure);
	ADC_RegularChannelConfig( ADC1, ADC_Channel_1, 1, ADC_SampleTime_1Cycles5);
	ADC_DMACmd(ADC1, ENABLE); // à chaque fin de conversion, demande de transfert DMA.
	ADC_Cmd(ADC1, ENABLE );
	ADC_ResetCalibration(ADC1); /* Enable ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1)); /* Check the end of ADC1 reset calibration register */
	ADC_StartCalibration(ADC1); /* Start ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1)); /* Check the end of ADC1 calibration */
	//ADC_SoftwareStartConvCmd( ADC1, ENABLE );
}

void timer_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	u16 TIM_period = 2000; //ARR nb of cnt | multiply ARR by PSC to set the total cnt time
	u16 TIM_Prescaler = 35999; //PSC divider of the clk (72MHz), PSC=ftim2/(1/timeOFcnt)
	
	/* Time base configuration */
	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseStructure.TIM_Period = TIM_period;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM_Prescaler; /* Fpwm = ????? */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_UpdateDisableConfig(TIM2, DISABLE);
	TIM_UpdateRequestConfig ( TIM2, TIM_UpdateSource_Regular);
	TIM_Cmd ( TIM2, ENABLE );
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/


