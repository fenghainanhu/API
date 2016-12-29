#ifndef _LED_H_
#define _LED_H_
#include "stm32f10x.h"

/******************************************************************************
							�궨��
*******************************************************************************/ 
#define LED2_GPIO	GPIOB
#define LED2_Pin	GPIO_Pin_0

#define LED2_ON 	LED2_GPIO->BRR  = LED2_Pin //ͨ��ָʾ��
#define LED2_OFF  	LED2_GPIO->BSRR = LED2_Pin

#define LED3_ON  	TIM4->CCR1 = 1999//������Χ��ҹ���	
#define LED3_OFF 	TIM4->CCR1 = 0		

/******************************************************************************
							ȫ�ֺ�������
*******************************************************************************/ 
void LED_Init(void);
void Breath_LED(float speed);
void Delay_led(u16 n);
void LED_ON_OFF(void);
void LED_FLASH(void);
void LED3_ON_Frequency(uint8_t Light_Frequency);
	
#endif
