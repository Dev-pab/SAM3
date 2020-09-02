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

void CLOCK_init();
void FLASH_init();
void GPIO_init();
void NVIC_init();
void EXTI_init();

/*----------------------------------------------------------------------------
 *        Main: Initialize 
 *---------------------------------------------------------------------------*/
 
void EXTI0_IRQHandler(void) 
{
	 GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(1^GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8)));
	 EXTI_ClearITPendingBit(EXTI_Line0);
}

int main (void) 
{                     /* program execution starts here       */
   
	CLOCK_init();
  GPIO_init();
	EXTI_init(); 
	NVIC_init();
	 
	//GPIO_ResetBits(GPIOB, GPIO_Pin_All);
	
  while (1)
	{
		//GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(1^GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8)));
	}
}

void CLOCK_init()
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
	} 
	else while (1);
	
}

void FLASH_init()
{
	FLASH_SetLatency(FLASH_Latency_2);
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
}

void GPIO_init() 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PinSource0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void EXTI_init() 
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); /* Selects PA.1 as EXTI */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0; /* Line 1 */
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
}

void NVIC_init() 
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* Configure 2 bits for preemption priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQChannel; /* Enable the EXTI1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/


