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
void EXTI_init ();
void NVIC_init(void);
void DMA_init(void);

	uc8 message1[10000]=" Centre de microelectronique de provence - George Charpak";
	u8 message2[10000]="";

/*----------------------------------------------------------------------------
 *        Main: Initialize 
 *---------------------------------------------------------------------------*/
int main (void) 
{                     /* program execution starts here       */
  int i;

	CLOCK_init();
  GPIO_init();
	EXTI_init ();
	NVIC_init();
	DMA_init();
	
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_All);
	
	for (i=0 ; i<10000 ; i++)
	{
		message2[i] = message1[i];
	}
	 
	DMA_Cmd(DMA1_Channel1, ENABLE );
	
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
		
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
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

void EXTI_init ()
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
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQChannel; /* Enable the DMA1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
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
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&message1;
	DMA_InitStruct.DMA_MemoryBaseAddr = (u32)&message2;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStruct.DMA_BufferSize = 10000;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;
	DMA_Init( DMA1_Channel1, &DMA_InitStruct );
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/


