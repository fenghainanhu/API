#ifndef _USART3_H
#define _USART3_H
#include "stm32f10x.h"

/******************************************************************************
							ȫ�ֺ�������
*******************************************************************************/ 
void Uart3_Init(uint32_t baud);
void USART3_PrintHexU8(uint8_t data);

#endif
