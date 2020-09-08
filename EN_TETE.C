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


#include "stm32f10x_lib.h"            /* STM32F10x Library Definitions       */

/*----------------------------------------------------------------------------
 *        Protoype
 *---------------------------------------------------------------------------*/

//int End_MEssage = false;

void clockInit(void);
void flashInit(void);
void GPIOInit(void);
void NVIC_init(void);
void Timer_init(void);
void EXTI_init(void);

u16 number[512];
int moy, moyf;

int main (void) 
{                     /* program execution starts here       */
   	
	 clockInit();
	 GPIOInit();
	 EXTI_init();
	 NVIC_init();
	 Timer_init();
	
	TIM_Cmd ( TIM2, ENABLE );
	 
	while (1)
	{
				
	}
}

void clockInit(void)
{
		ErrorStatus HSEStartUpStatus;
		RCC_HSEConfig(RCC_HSE_ON);
		HSEStartUpStatus = RCC_WaitForHSEStartUp(); /* Wait till HSE is ready */
		
	if(HSEStartUpStatus == SUCCESS) 
		{
			
			flashInit();
			
			RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
			RCC_PLLCmd(ENABLE);
			RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
			RCC_HCLKConfig(RCC_SYSCLK_Div1);
			RCC_PCLK1Config(RCC_HCLK_Div2);
			RCC_PCLK2Config(RCC_HCLK_Div1);
			
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SRAM | RCC_AHBPeriph_FLITF, ENABLE);
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1, ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
		} 
		else  while (1);
}

void flashInit(void)
{
		FLASH_SetLatency(FLASH_Latency_2);
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
}

void GPIOInit(void) 
{
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_0 | GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void EXTI_init (void) 
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); /* Selects PA.1 as EXTI */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0; /* Line 1 */
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13); /* Selects PA.1 as EXTI */
	EXTI_InitStructure.EXTI_Line = EXTI_Line13; /* Line 1 */
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

void NVIC_init(void) 
{
  NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* Configure 2 bits for preemption priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQChannel ; /* Enable the EXTI1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* Configure 2 bits for preemption priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQChannel; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* Configure 2 bits for preemption priority */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	
 
}

void EXTI0_IRQHandler(void) 
{
	EXTI_ClearITPendingBit(EXTI_Line0); // Acquitement
}

int EXTI15_10_IRQHandler(void)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)(1^GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_10)));
	EXTI_ClearITPendingBit(EXTI_Line13); // Acquitement
	
	return 0;
}

void TIM2_IRQHandler(void)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(1^GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8)));
	GPIO_WriteBit(GPIOA, GPIO_Pin_2, (BitAction)(1^GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2)));
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

void Timer_init(void) 
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	u16 TIM_period = 500;
	u16 TIM_Prescaler = 36001;
	/* Time base configuration */
	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseStructure.TIM_Period = TIM_period;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM_Prescaler; /* Fpwm = ????? */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}
/* end file */
