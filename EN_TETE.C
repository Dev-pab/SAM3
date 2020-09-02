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

/*----------------------------------------------------------------------------
 *        Main: Initialize 
 *---------------------------------------------------------------------------*/
int main (void) 
{                     /* program execution starts here       */
   
	CLOCK_init();
  GPIO_init();
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_All);
	 
  while (1)
	{
		GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(1^GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8)));
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
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/


